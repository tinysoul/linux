/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright(c) 2020 - 2023 Allwinner Technology Co.,Ltd. All rights reserved. */
/*
 * The driver of SUNXI SecuritySystem controller.
 *
 * Copyright (C) 2013 Allwinner.
 *
 * Mintow <duanmintao@allwinnertech.com>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/spinlock.h>
#include <linux/platform_device.h>
#include <crypto/internal/hash.h>
#include <crypto/internal/rng.h>

#include <linux/dmaengine.h>
#include <linux/dma-mapping.h>

#include "../sunxi_ce.h"
#include "../sunxi_ce_proc.h"
#include "sunxi_ce_reg.h"

/* Callback of DMA completion. */
static void ss_dma_cb(void *data)
{
	ss_aes_req_ctx_t *req_ctx = (ss_aes_req_ctx_t *)data;

	SS_DBG("DMA transfer data complete!\n");

	complete(&req_ctx->done);
}

/* request dma channel and set callback function */
static int ss_dma_prepare(ss_dma_info_t *info)
{
	dma_cap_mask_t mask;

	/* Try to acquire a generic DMA engine slave channel */
	dma_cap_zero(mask);
	dma_cap_set(DMA_SG, mask);
	dma_cap_set(DMA_MEMCPY, mask);
	info->chan = dma_request_channel(mask, NULL, NULL);
	if (info->chan == NULL) {
		SS_ERR("Request DMA() failed!\n");
		return -EINVAL;
	}
	return 0;
}

/* set dma start flag, if queue, it will auto restart to transfer next queue */
static void ss_dma_start(ss_dma_info_t *info)
{
	dma_async_issue_pending(info->chan);
}

/* Make a sg_table based on sg[] of crypto request. */
static int ss_sg_table_init(struct sg_table *sgt,
	struct scatterlist *sg, int len, char *vbase, dma_addr_t pbase)
{
	int i;
	int npages = 0;
	int offset = 0;
	struct scatterlist *src_sg = sg;
	struct scatterlist *dst_sg = NULL;

	npages = ss_sg_cnt(sg, len);
	WARN_ON(npages == 0);

	if (sg_alloc_table(sgt, npages, GFP_KERNEL)) {
		SS_ERR("sg_alloc_table(%d) failed!\n", npages);
		WARN_ON(1);
	}

	dst_sg = sgt->sgl;
	for (i = 0; i < npages; i++) {
		sg_set_buf(dst_sg, vbase + offset, sg_dma_len(src_sg));
		offset += sg_dma_len(src_sg);
		src_sg = sg_next(src_sg);
		dst_sg = sg_next(dst_sg);
	}
	return 0;
}

static int ss_dma_dst_config(sunxi_ss_t *sss,
	void *ctx, ss_aes_req_ctx_t *req_ctx, int len, int cb)
{
	int flow = ((ss_comm_ctx_t *)ctx)->flow;
	ss_dma_info_t *info = &req_ctx->dma_dst;
	struct dma_slave_config dma_conf = {0};
	struct dma_async_tx_descriptor *dma_desc = NULL;

	info->dir = DMA_MEM_TO_MEM;
	dma_conf.direction = info->dir;
#ifdef SS_CTR_MODE_ENABLE
	if (req_ctx->mode == SS_AES_MODE_CTR)
		dma_conf.src_addr_width = dma_conf.dst_addr_width
			= DMA_SLAVE_BUSWIDTH_1_BYTE;
	else
#endif
	dma_conf.src_addr_width = dma_conf.dst_addr_width
			= DMA_SLAVE_BUSWIDTH_4_BYTES;

	dma_conf.src_maxburst = 1;
	dma_conf.dst_maxburst = 1;
	dma_conf.slave_id = sunxi_slave_id(DRQDST_SDRAM, DRQSRC_SDRAM);
	dmaengine_slave_config(info->chan, &dma_conf);

	ss_sg_table_init(&info->sgt_for_cp, info->sg, len,
		sss->flows[flow].buf_dst, sss->flows[flow].buf_dst_dma);

	info->nents = info->sgt_for_cp.nents;
	SS_DBG("flow: %d, sg num: %d, total len: %d\n", flow, info->nents, len);

	dma_map_sg(&sss->pdev->dev, info->sg, info->nents, info->dir);
	dma_map_sg(&sss->pdev->dev, info->sgt_for_cp.sgl,
		info->nents, info->dir);

	dma_desc = info->chan->device->device_prep_dma_sg(info->chan,
			info->sg, info->nents,
			info->sgt_for_cp.sgl, info->nents,
			DMA_PREP_INTERRUPT | DMA_CTRL_ACK);
	if (!dma_desc) {
		SS_ERR("dmaengine_prep_slave_sg() failed!\n");
		return -EINVAL;
	}

	if (cb == 1) {
		dma_desc->callback = ss_dma_cb;
		dma_desc->callback_param = (void *)req_ctx;
	}
	dmaengine_submit(dma_desc);

	return 0;
}

static int ss_dma_src_config(sunxi_ss_t *sss,
	void *ctx, ss_aes_req_ctx_t *req_ctx, int len, int cb)
{
	int flow = ((ss_comm_ctx_t *)ctx)->flow;
	ss_dma_info_t *info = &req_ctx->dma_src;
	struct dma_slave_config dma_conf = {0};
	struct dma_async_tx_descriptor *dma_desc = NULL;

	info->dir = DMA_MEM_TO_MEM;
	dma_conf.direction = info->dir;
#ifdef SS_CTR_MODE_ENABLE
	if (req_ctx->mode == SS_AES_MODE_CTR)
		dma_conf.src_addr_width = dma_conf.dst_addr_width
			= DMA_SLAVE_BUSWIDTH_1_BYTE;
	else
#endif
	dma_conf.src_addr_width = dma_conf.dst_addr_width
		= DMA_SLAVE_BUSWIDTH_4_BYTES;

	dma_conf.src_maxburst = 1;
	dma_conf.dst_maxburst = 1;
	dma_conf.slave_id = sunxi_slave_id(DRQDST_SDRAM, DRQSRC_SDRAM);
	dmaengine_slave_config(info->chan, &dma_conf);

	ss_sg_table_init(&info->sgt_for_cp, info->sg, len,
		sss->flows[flow].buf_src, sss->flows[flow].buf_src_dma);
	SS_DBG("chan: 0x%p, info->sgt_for_cp.sgl: 0x%p\n",
		info->chan, info->sgt_for_cp.sgl);

	info->nents = info->sgt_for_cp.nents;
	SS_DBG("flow: %d, sg num: %d, total len: %d\n", flow, info->nents, len);

	dma_map_sg(&sss->pdev->dev, info->sg, info->nents, info->dir);
	dma_map_sg(&sss->pdev->dev, info->sgt_for_cp.sgl,
		info->nents, info->dir);

	if (SS_METHOD_IS_HASH(req_ctx->type)) {
		/* Total len is too small, so there is no data for DMA. */
		if (len < SHA1_BLOCK_SIZE)
			return 1;

		ss_hash_padding_sg_prepare(&info->sg[info->nents-1], len);
		ss_hash_padding_sg_prepare(&info->sgt_for_cp.sgl[info->nents-1],
			len);
	}

	dma_desc = info->chan->device->device_prep_dma_sg(info->chan,
			info->sgt_for_cp.sgl, info->nents,
			info->sg, info->nents, DMA_PREP_INTERRUPT|DMA_CTRL_ACK);
	if (!dma_desc) {
		SS_ERR("dmaengine_prep_slave_sg() failed!\n");
		return -EINVAL;
	}

	if (cb == 1) {
		dma_desc->callback = ss_dma_cb;
		dma_desc->callback_param = (void *)req_ctx;
	}
	dmaengine_submit(dma_desc);
	return 0;
}

static void ss_dma_release(sunxi_ss_t *sss, ss_dma_info_t *info)
{
	dma_unmap_sg(&sss->pdev->dev, info->sgt_for_cp.sgl,
		info->nents, info->dir);
	sg_free_table(&info->sgt_for_cp);
	dma_unmap_sg(&sss->pdev->dev, info->sg, info->nents, info->dir);
	dma_release_channel(info->chan);
}

static int ss_aes_start(ss_aes_ctx_t *ctx, ss_aes_req_ctx_t *req_ctx, int len)
{
	int ret = 0;
	int flow = ctx->comm.flow;

	ss_pending_clear(flow);
	ss_irq_enable(flow);
	ss_flow_enable(flow);

	ss_method_set(req_ctx->dir, req_ctx->type);
	ss_aes_mode_set(req_ctx->mode);

	SS_DBG("Flow: %d, Dir: %d, Method: %d, Mode: %d, len: %d\n",
		flow, req_ctx->dir, req_ctx->type, req_ctx->mode, len);

	init_completion(&req_ctx->done);

	/* 1. Copy data from user space to sss->flows[flow].buf_src. */
	if (ss_dma_prepare(&req_ctx->dma_src))
		return -EBUSY;
#ifdef SS_CTR_MODE_ENABLE
	if ((req_ctx->mode == SS_AES_MODE_CTR) && ((len%AES_BLOCK_SIZE) != 0))
		memset(&ss_dev->flows[flow].buf_src[len], 0, AES_BLOCK_SIZE);
#endif
	ss_dma_src_config(ss_dev, ctx, req_ctx, len, 1);
	ss_dma_start(&req_ctx->dma_src);
	ret = wait_for_completion_timeout(&req_ctx->done,
		msecs_to_jiffies(SS_WAIT_TIME));
	if (ret == 0) {
		SS_ERR("Timed out\n");
		return -ETIMEDOUT;
	}

	/* 2. Start the SS. */
	ss_data_src_set(ss_dev->flows[flow].buf_src_dma);
	ss_data_dst_set(ss_dev->flows[flow].buf_dst_dma);
	SS_DBG("ss_dev->buf_dst_dma = %#x\n", ss_dev->flows[flow].buf_dst_dma);
#ifdef SS_CTS_MODE_ENABLE
	if (req_ctx->mode == SS_AES_MODE_CTS) {
		ss_data_len_set(len);
		/* A bad way to determin the last packet of CTS mode. */
		/* if (len < SZ_4K) */
			ss_cts_last();
	} else
#endif
	ss_data_len_set(DIV_ROUND_UP(len, AES_BLOCK_SIZE)*4);

	ss_ctrl_start();

	ret = wait_for_completion_timeout(&ss_dev->flows[flow].done,
		msecs_to_jiffies(SS_WAIT_TIME));
	if (ret == 0) {
		SS_ERR("Timed out\n");
		ss_reset();
		return -ETIMEDOUT;
	}

	/* 3. Copy the result from sss->flows[flow].buf_dst to user space. */
	if (ss_dma_prepare(&req_ctx->dma_dst))
		return -EBUSY;
	ss_dma_dst_config(ss_dev, ctx, req_ctx, len, 1);
	ss_dma_start(&req_ctx->dma_dst);
	ret = wait_for_completion_timeout(&req_ctx->done,
		msecs_to_jiffies(SS_WAIT_TIME));
	if (ret == 0) {
		SS_ERR("Timed out\n");
		return -ETIMEDOUT;
	}

	ss_ctrl_stop();
	ss_irq_disable(flow);
	ss_dma_release(ss_dev, &req_ctx->dma_src);
	ss_dma_release(ss_dev, &req_ctx->dma_dst);

	return 0;
}

int ss_aes_key_valid(struct crypto_ablkcipher *tfm, int len)
{
	if (unlikely(len > SS_RSA_MAX_SIZE)) {
		SS_ERR("Unsupported key size: %d\n", len);
		return -EINVAL;
	}
	return 0;
}

static int ss_rng_start(ss_aes_ctx_t *ctx, u8 *rdata, u32 dlen, u32 trng)
{
	int ret = 0;
	int flow = ctx->comm.flow;
	u32 len = dlen;

	if (len > SS_RNG_MAX_LEN) {
		SS_ERR("The RNG length is too large: %d\n", len);
		len = SS_RNG_MAX_LEN;
	}

	ss_pending_clear(flow);
	ss_irq_enable(flow);
	ss_flow_enable(flow);

	if (trng) {
		ss_method_set(SS_DIR_ENCRYPT, SS_METHOD_TRNG);
		ss_trng_osc_enable();
	} else
		ss_method_set(SS_DIR_ENCRYPT, SS_METHOD_PRNG);

	ss_rng_mode_set(SS_RNG_MODE_CONTINUE);

	ss_data_dst_set(ss_dev->flows[flow].buf_dst_dma);
	if (trng)
		/* align with 32 Bytes */
		ss_data_len_set(DIV_ROUND_UP(len, 32)*(32>>2));
	else
		/* align with 20 Bytes */
		ss_data_len_set(DIV_ROUND_UP(len, 20)*(20>>2));

	SS_DBG("Flow: %d, Request: %d, Aligned: %d\n",
		flow, len, DIV_ROUND_UP(len, 20)*5);
	dma_map_single(&ss_dev->pdev->dev, ss_dev->flows[flow].buf_dst,
		SS_DMA_BUF_SIZE, DMA_DEV_TO_MEM);

	ss_ctrl_start();
	ret = wait_for_completion_timeout(&ss_dev->flows[flow].done,
		msecs_to_jiffies(SS_WAIT_TIME));
	if (ret == 0) {
		SS_ERR("Timed out\n");
		ss_reset();
		return -ETIMEDOUT;
	}

	dma_unmap_single(&ss_dev->pdev->dev, ss_dev->flows[flow].buf_dst_dma,
		SS_DMA_BUF_SIZE, DMA_DEV_TO_MEM);
	memcpy(rdata, ss_dev->flows[flow].buf_dst, len);

	if (trng)
		ss_trng_osc_disable();

	ss_irq_disable(flow);
	ss_ctrl_stop();
	ret = len;
	return ret;
}

int ss_rng_get_random(struct crypto_rng *tfm, u8 *rdata, u32 dlen, u32 trng)
{
	int ret = 0;
	u8 *data = rdata;
	u32 len = dlen;
	ss_aes_ctx_t *ctx = crypto_rng_ctx(tfm);

	SS_DBG("flow = %d, data = %p, len = %d, trng = %d\n",
		ctx->comm.flow, data, len, trng);
	if (ss_dev->suspend) {
		SS_ERR("SS has already suspend.\n");
		return -EAGAIN;
	}

#ifdef SS_TRNG_POSTPROCESS_ENABLE
	if (trng) {
		len = DIV_ROUND_UP(dlen, SHA256_DIGEST_SIZE)*SHA256_BLOCK_SIZE;
		data = kzalloc(len, GFP_KERNEL);
		if (data == NULL) {
			SS_ERR("Failed to malloc(%d)\n", len);
			return -ENOMEM;
		}
		SS_DBG("In fact, flow = %d, data = %p, len = %d\n",
			ctx->comm.flow, data, len);
	}
#endif

	ss_dev_lock();

	/* Must set the seed addr in PRNG/TRNG. */
	ss_key_set(ctx->key, ctx->key_size);
	ctx->comm.flags &= ~SS_FLAG_NEW_KEY;
	dma_map_single(&ss_dev->pdev->dev, ctx->key, ctx->key_size,
		DMA_MEM_TO_DEV);

	ret = ss_rng_start(ctx, data, len, trng);
	ss_dev_unlock();

	SS_DBG("Get %d byte random.\n", ret);

	dma_unmap_single(&ss_dev->pdev->dev,
		virt_to_phys(ctx->key), ctx->key_size, DMA_MEM_TO_DEV);

#ifdef SS_TRNG_POSTPROCESS_ENABLE
	if (trng) {
		ss_trng_postprocess(rdata, dlen, data, len);
		ret = dlen;
		kfree(data);
	}
#endif

	return ret;
}

u32 ss_hash_start(ss_hash_ctx_t *ctx,
		ss_aes_req_ctx_t *req_ctx, u32 len, u32 last)
{
	int ret = 0;
	int flow = ctx->comm.flow;
	int md_map_flag = 0;

	ss_pending_clear(flow);
	ss_irq_enable(flow);
	ss_flow_enable(flow);
	ss_method_set(req_ctx->dir, req_ctx->type);

	SS_DBG("Flow: %d, Dir: %d, Method: %d, Mode: %d, len: %d/%d\n", flow,
		req_ctx->dir, req_ctx->type, req_ctx->mode, len, ctx->cnt);

	SS_DBG("IV address = 0x%p, size = %d\n", ctx->md, ctx->md_size);
	ss_iv_set(ctx->md, ctx->md_size);
	ss_iv_mode_set(SS_IV_MODE_ARBITRARY);

	init_completion(&req_ctx->done);

	if (ss_dma_prepare(&req_ctx->dma_src))
		return -EBUSY;
	ret = ss_dma_src_config(ss_dev, ctx, req_ctx, len, 1);
	if (ret == 0) {
		/* 1. Copy data from user space to sss->flows[flow].buf_src. */
		ss_dma_start(&req_ctx->dma_src);
		ret = wait_for_completion_timeout(&req_ctx->done,
			msecs_to_jiffies(SS_WAIT_TIME));
		if (ret == 0) {
			SS_ERR("Timed out\n");
			return -ETIMEDOUT;
		}
		/* 2. Start the SS. */
		ss_data_src_set(ss_dev->flows[flow].buf_src_dma);
		ss_data_dst_set(ss_dev->flows[flow].buf_dst_dma);
		SS_DBG("buf_dst_dma: %#x\n", ss_dev->flows[flow].buf_dst_dma);
		ss_data_len_set((len - len%SHA1_BLOCK_SIZE)/4);

#ifdef SS_SHA_SWAP_MID_ENABLE
		if (req_ctx->type != SS_METHOD_MD5)
			ss_hash_swap(ctx->md, ctx->md_size);
#endif

		dma_map_single(&ss_dev->pdev->dev, ctx->md, ctx->md_size,
			DMA_MEM_TO_DEV);
		md_map_flag = 1;

		SS_DBG("Before SS, CTRL: 0x%08x\n", ss_reg_rd(SS_REG_CTL));
		memset(ss_dev->flows[flow].buf_dst, 0xF0, ctx->md_size);
		dma_map_single(&ss_dev->pdev->dev, ss_dev->flows[flow].buf_dst,
			ctx->md_size, DMA_DEV_TO_MEM);
		ss_ctrl_start();

		ret = wait_for_completion_timeout(&ss_dev->flows[flow].done,
			msecs_to_jiffies(SS_WAIT_TIME));
		if (ret == 0) {
			SS_ERR("Timed out\n");
			ss_reset();
			return -ETIMEDOUT;
		}
		SS_DBG("After SS, CTRL: 0x%08x\n", ss_reg_rd(SS_REG_CTL));
		SS_DBG("After SS, dst data:\n");
		ss_print_hex(ss_dev->flows[flow].buf_dst, 32,
			ss_dev->flows[flow].buf_dst);

		/* 3. Copy the MD from sss->buf_dst to ctx->md. */
		memcpy(ctx->md, ss_dev->flows[flow].buf_dst, ctx->md_size);
	}

	ss_ctrl_stop();
	ss_irq_disable(flow);
	if (md_map_flag == 1) {
		dma_unmap_single(&ss_dev->pdev->dev,
			ss_dev->flows[flow].buf_dst_dma, ctx->md_size,
			DMA_DEV_TO_MEM);
		dma_unmap_single(&ss_dev->pdev->dev,
			virt_to_phys(ctx->md), ctx->md_size, DMA_MEM_TO_DEV);
	}
	ss_dma_release(ss_dev, &req_ctx->dma_src);

	ctx->cnt += len;
	return 0;
}

int ss_aes_one_req(sunxi_ss_t *sss, struct ablkcipher_request *req)
{
	int ret = 0;
	struct crypto_ablkcipher *tfm = NULL;
	ss_aes_ctx_t *ctx = NULL;
	ss_aes_req_ctx_t *req_ctx = NULL;
	int key_map_flag = 0;
	int iv_map_flag = 0;

	SS_ENTER();
	if (!req->src || !req->dst) {
		SS_ERR("Invalid sg: src = %p, dst = %p\n", req->src, req->dst);
		return -EINVAL;
	}

	ss_dev_lock();

	tfm = crypto_ablkcipher_reqtfm(req);
	req_ctx = ablkcipher_request_ctx(req);
	ctx = crypto_ablkcipher_ctx(tfm);

	/* A31 SS need update key each cycle in decryption. */
	if ((ctx->comm.flags & SS_FLAG_NEW_KEY)
		|| (req_ctx->dir == SS_DIR_DECRYPT)) {
		SS_DBG("KEY address: %p, size: %d\n", ctx->key, ctx->key_size);
		ss_key_set(ctx->key, ctx->key_size);
		dma_map_single(&sss->pdev->dev, ctx->key,
			ctx->key_size, DMA_MEM_TO_DEV);
		key_map_flag = 1;
		ctx->comm.flags &= ~SS_FLAG_NEW_KEY;
	}

#ifdef SS_CTS_MODE_ENABLE
	if (((req_ctx->mode == SS_AES_MODE_CBC)
		|| (req_ctx->mode == SS_AES_MODE_CTS)) && (req->info != NULL)) {
#else
	if ((req_ctx->mode == SS_AES_MODE_CBC) && (req->info != NULL)) {
#endif
		SS_DBG("IV address = %p, size = %d\n", req->info,
			crypto_ablkcipher_ivsize(tfm));
		memcpy(ctx->iv, req->info, crypto_ablkcipher_ivsize(tfm));
		ss_iv_set(ctx->iv, crypto_ablkcipher_ivsize(tfm));
		dma_map_single(&sss->pdev->dev, ctx->iv,
			crypto_ablkcipher_ivsize(tfm), DMA_MEM_TO_DEV);
		iv_map_flag = 1;
	}

#ifdef SS_CTR_MODE_ENABLE
	if ((req_ctx->mode == SS_AES_MODE_CTR) && (req->info != NULL)) {
		SS_DBG("Cnt address = %p, size = %d\n", req->info,
			crypto_ablkcipher_ivsize(tfm));
		if (ctx->cnt == 0)
			memcpy(ctx->iv, req->info,
				crypto_ablkcipher_ivsize(tfm));

		SS_DBG("CNT: %08x %08x %08x %08x\n",
			*(int *)&ctx->iv[0], *(int *)&ctx->iv[4],
			*(int *)&ctx->iv[8], *(int *)&ctx->iv[12]);
		ss_cnt_set(ctx->iv, crypto_ablkcipher_ivsize(tfm));
		dma_map_single(&sss->pdev->dev, ctx->iv,
			crypto_ablkcipher_ivsize(tfm), DMA_MEM_TO_DEV);
		iv_map_flag = 1;
	}
#endif

	SS_DBG("The current IV:\n");
	ss_print_hex(ctx->iv, 16, ctx->iv);

	if (req_ctx->type == SS_METHOD_RSA)
		ss_rsa_width_set(crypto_ablkcipher_ivsize(tfm));

	req_ctx->dma_src.sg = req->src;
	req_ctx->dma_dst.sg = req->dst;

	ret = ss_aes_start(ctx, req_ctx, req->nbytes);
	if (ret < 0)
		SS_ERR("ss_aes_start fail(%d)\n", ret);

	ss_dev_unlock();

	if (key_map_flag == 1)
		dma_unmap_single(&ss_dev->pdev->dev, virt_to_phys(ctx->key),
			ctx->key_size, DMA_MEM_TO_DEV);
	if (iv_map_flag == 1)
		dma_unmap_single(&sss->pdev->dev, virt_to_phys(ctx->iv),
			crypto_ablkcipher_ivsize(tfm), DMA_MEM_TO_DEV);

#ifdef SS_CTR_MODE_ENABLE
	if (req_ctx->mode == SS_AES_MODE_CTR) {
		ss_cnt_get(ctx->comm.flow,
			ctx->iv, crypto_ablkcipher_ivsize(tfm));
		SS_DBG("CNT: %08x %08x %08x %08x\n",
			*(int *)&ctx->iv[0], *(int *)&ctx->iv[4],
			*(int *)&ctx->iv[8], *(int *)&ctx->iv[12]);
	}
#endif

	ctx->cnt += req->nbytes;
	return ret;
}

irqreturn_t sunxi_ss_irq_handler(int irq, void *dev_id)
{
	sunxi_ss_t *sss = (sunxi_ss_t *)dev_id;
	int pending = 0;

	pending = ss_pending_get();
	SS_DBG("SS pending %#x\n", pending);
	if (pending&SS_REG_ICR_FLOW0_PENDING_MASK) {
		ss_pending_clear(0);
		complete(&sss->flows[0].done);
	}
	if (pending&SS_REG_ICR_FLOW1_PENDING_MASK) {
		ss_pending_clear(1);
		complete(&sss->flows[1].done);
	}
	SS_DBG("SS pending %#x, CTRL: 0x%08x\n",
		ss_pending_get(), ss_reg_rd(SS_REG_CTL));

	return IRQ_HANDLED;
}
