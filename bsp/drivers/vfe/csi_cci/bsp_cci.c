/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright(c) 2020 - 2023 Allwinner Technology Co.,Ltd. All rights reserved. */
/*
 * linux-4.9/drivers/media/platform/sunxi-vfe/csi_cci/bsp_cci.c
 *
 * Copyright (c) 2007-2017 Allwinnertech Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */


/*
 ***************************************************************************************
 *
 * bsp_cci.c
 *
 * Hawkview ISP - bsp_cci.c module
 *
 * Copyright (c) 2014 by Allwinnertech Co., Ltd.  http://www.allwinnertech.com
 *
 * Version		  Author         Date		    Description
 *
 *   2.0		  Yang Feng	2014/04/23	      Second Version
 *
 ****************************************************************************************
 */

#include "bsp_cci.h"
#include "../platform_cfg.h"
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/mutex.h>
#include <linux/semaphore.h>
#define MAX_CCI_DEVICE 2

wait_queue_head_t wait[MAX_CCI_DEVICE];
static int status_err_flag[MAX_CCI_DEVICE] = {0};
struct mutex            cci_mutex[MAX_CCI_DEVICE];
struct semaphore		cci_sema[MAX_CCI_DEVICE];
int cci_cnt_done;
int cci_cnt_irq;

int bsp_csi_cci_set_base_addr(unsigned int sel, unsigned long addr)
{
	cci_cnt_done = 0;
	cci_cnt_irq = 0;
	init_waitqueue_head(&wait[sel]);
	mutex_init(&cci_mutex[sel]);
	sema_init(&cci_sema[sel], 1);
	return csi_cci_set_base_addr(sel, addr);
}

static void cci_cal_div(unsigned int clk, unsigned char *div_coef)
{
	unsigned char clk_m = 0;
	unsigned char clk_n = 0;
	unsigned int src_clk = CCI_HCLK/10;
	unsigned int sclk_req = clk;
	unsigned char _2_pow_clk_n = 1;
	unsigned int sclk_real = 0;
	unsigned int divider = src_clk/sclk_req;

	if (divider == 0) {
		clk_m = 1;
		goto out;
	}

	while (clk_n < 8) {
		clk_m = (divider / _2_pow_clk_n)-1;
		while (clk_m < 16) {
			sclk_real = src_clk/(clk_m+1)/_2_pow_clk_n;
			if (sclk_real <= sclk_req)
				goto out;
			else
				clk_m++;
		}

		clk_n++;
		_2_pow_clk_n *= 2;
	}

out:

	div_coef[0] = clk_m;
	div_coef[1] = clk_n;
}

void bsp_csi_cci_init(unsigned int sel)
{
	unsigned char div_coef[2];

	csi_cci_reset(sel);
	csi_cci_enable(sel);
	cci_cal_div(400*1000, div_coef);
	csi_cci_set_clk_div(sel, div_coef);
	csi_cci_set_pkt_interval(sel, 16);
	csi_cci_set_ack_timeout(sel, 16);
	csi_cci_set_trig_dly(sel, 0);
	cci_pad_en(sel);
}

void bsp_csi_cci_exit(unsigned int sel)
{
	bsp_cci_int_disable(sel, CCI_INT_ALL);
	csi_cci_reset(sel);
	csi_cci_disable(sel);
}

void bsp_cci_set_tx_mode(unsigned int sel, struct cci_tx_mode *tx_mode)
{
	csi_cci_set_tx_buf_mode(sel, &tx_mode->tx_buf_mode);
	csi_cci_set_trig_mode(sel, &tx_mode->tx_trig_mode);
	csi_cci_set_trig_line_cnt(sel, tx_mode->tx_trig_line_cnt);
}

void bsp_cci_tx_start(unsigned int sel, struct cci_msg *msg)
{
	unsigned int i, j, pkt_len, max_pkt_num, pkt_num;
	unsigned char *buf;

	csi_cci_set_bus_fmt(sel, &msg->bus_fmt);

	buf = msg->pkt_buf;
	pkt_num = msg->pkt_num;
	pkt_len = msg->bus_fmt.addr_len + msg->bus_fmt.data_len;
	max_pkt_num = FIFO_DEPTH / pkt_len;

	while (pkt_num != 0) {
		if (pkt_num < max_pkt_num) {
			j = pkt_num;
			pkt_num = 0;
		} else {
			j = max_pkt_num;
			pkt_num = pkt_num - max_pkt_num;
		}

		for (i = 0; i < j; i++) {
			csi_cci_wr_tx_buf(sel, buf, pkt_len);
			buf += pkt_len;
		}
		csi_cci_trans_start(sel, SINGLE);
	}
}

void bsp_cci_tx_data_rb(unsigned int sel, struct cci_msg *msg)
{
	unsigned int i, j, pkt_len, max_pkt_num, pkt_num;
	unsigned char *buf;

	csi_cci_set_bus_fmt(sel, &msg->bus_fmt);
	buf = msg->pkt_buf;
	pkt_num = msg->pkt_num;
	pkt_len = msg->bus_fmt.addr_len + msg->bus_fmt.data_len;
	max_pkt_num = FIFO_DEPTH / pkt_len;

	while (pkt_num != 0) {
		if (pkt_num < max_pkt_num) {
			j = pkt_num;
			pkt_num = 0;
		} else {
			j = max_pkt_num;
			pkt_num = pkt_num - max_pkt_num;
		}

		for (i = 0; i < j; i++) {
			csi_cci_fifo_pt_add(sel, msg->bus_fmt.addr_len);
			csi_cci_rd_tx_buf(sel, buf+msg->bus_fmt.addr_len, msg->bus_fmt.data_len);
			buf += pkt_len;
		}
	}
	csi_cci_fifo_pt_reset(sel);
}

static void bsp_cci_error_process(unsigned int sel)
{
	cci_int_clear_status(sel, CCI_INT_ERROR);
	bsp_cci_bus_error_process(sel);
	bsp_csi_cci_exit(sel);
	bsp_csi_cci_init_helper(sel);
}

static int bsp_cci_tx_start_wait_done_unlocked(unsigned int sel, struct cci_msg *msg)
{
#ifdef CCI_IRQ
	int ret = 0;
	unsigned long timeout = 0;

	bsp_cci_tx_start(sel, msg);
#ifdef FPGA_VER
	timeout = wait_event_timeout(wait[sel], csi_cci_get_trans_done(sel) == 0, HZ/50);
	/* cci_print_info(sel); */
#else
	timeout = wait_event_timeout(wait[sel], csi_cci_get_trans_done(sel) == 0, HZ);
	/* cci_print_info(sel); */
	if (timeout == 0) {
		pr_info("[VFE CCI_%d ERR] timeout error at addr_8bit = %x, wr_flag = %d, val = %x\n",
				sel, msg->bus_fmt.saddr_7bit << 1, msg->bus_fmt.wr_flag, *(int *)msg->pkt_buf);
		cci_cnt_done--;
		ret = -1;
	}
#endif
	if (status_err_flag[sel] == 1) {
		pr_info("[VFE CCI_%d ERR] Status error at addr_8bit = %x, wr_flag = %d, val = %x\n",
				sel, msg->bus_fmt.saddr_7bit << 1, msg->bus_fmt.wr_flag, *(int *)msg->pkt_buf);
		ret = -1;
	}

	if (msg->bus_fmt.wr_flag == 1)
		bsp_cci_tx_data_rb(sel, msg);

	return ret;
#else
	struct cci_int_status status;
	int ret = 0;

	bsp_cci_tx_start(sel, msg);
	usleep_range(100, 120);
	while (1) {
		if (csi_cci_get_trans_done(sel) == 0)
			break;
		else
			usleep_range(80, 100);
	};
	cci_int_get_status(sel, &status);
	if (status.error) {
		cci_int_clear_status(sel, CCI_INT_ERROR);
		pr_info("[VFE CCI_%d ERR] Status error at addr_8bit = %x, wr_flag = %d\n",
				sel, msg->bus_fmt.saddr_7bit << 1, msg->bus_fmt.wr_flag);
		bsp_cci_bus_error_process(sel);
		bsp_csi_cci_exit(sel);
		bsp_csi_cci_init_helper(sel);
		ret = -1;
	}
	if (status.complete)
		cci_int_clear_status(sel, CCI_INT_FINISH);
	if (msg->bus_fmt.wr_flag == 1)
		bsp_cci_tx_data_rb(sel, msg);

	return ret;
#endif
}

int bsp_cci_tx_start_wait_done(unsigned int sel, struct cci_msg *msg)
{
	int ret = -1;

	mutex_lock(&cci_mutex[sel]);
	if (down_trylock(&cci_sema[sel])) {
		pr_info("down_trylock fail!!!!!!!!!!!!!\n");
		return -1;
	}
	ret = bsp_cci_tx_start_wait_done_unlocked(sel, msg);
	up(&cci_sema[sel]);
	mutex_unlock(&cci_mutex[sel]);

	return ret;
}

void bsp_cci_int_enable(unsigned int sel, enum cci_int_sel interrupt)
{
	cci_int_enable(sel, interrupt);
}

void bsp_cci_int_disable(unsigned int sel, enum cci_int_sel interrupt)
{
	cci_int_disable(sel, interrupt);
}

void CCI_INLINE_FUNC bsp_cci_int_get_status(unsigned int sel, struct cci_int_status *status)
{
	cci_int_get_status(sel, status);
}

void CCI_INLINE_FUNC bsp_cci_int_clear_status(unsigned int sel, enum cci_int_sel interrupt)
{
	cci_int_clear_status(sel, interrupt);
}

enum cci_bus_status CCI_INLINE_FUNC bsp_cci_get_bus_status(unsigned int sel)
{
	return cci_get_bus_status(sel);
}

int bsp_cci_irq_process(unsigned int sel)
{
	struct cci_int_status status;
	unsigned int ret = 0;

	cci_cnt_irq++;
	cci_int_get_status(sel, &status);
	if (status.error) {
		bsp_cci_error_process(sel);
		status_err_flag[sel] = 1;
		ret = -1;
	}
	if (status.complete) {
		status_err_flag[sel] = 0;
		cci_int_clear_status(sel, CCI_INT_FINISH);
	}
	wake_up(&wait[sel]);

	return ret;
}

void bsp_cci_bus_error_process(unsigned int sel)
{
	cci_stop(sel);
	cci_sck_cycles(sel, 16);
}

void bsp_csi_cci_init_helper(unsigned int sel)
{
	struct cci_tx_mode tx_mode;

	bsp_csi_cci_init(sel);
	tx_mode.tx_buf_mode.buf_src = FIFO;
	tx_mode.tx_buf_mode.pkt_mode = COMPACT;
	tx_mode.tx_buf_mode.pkt_cnt = 1;
	tx_mode.tx_trig_mode.trig_src = NO_TRIG;
	tx_mode.tx_trig_mode.trig_con = TRIG_DEFAULT;
	tx_mode.tx_trig_line_cnt = 0;
	bsp_cci_set_tx_mode(sel, &tx_mode);
	bsp_cci_int_clear_status(sel, CCI_INT_ALL);
	bsp_cci_int_enable(sel, CCI_INT_ALL);
}


int cci_wr_8_8(unsigned int sel, unsigned char reg, unsigned char data, unsigned char slv)
{
	struct cci_msg msg;
	unsigned char buf[2];

	buf[0] = reg;
	buf[1] = data;

	msg.bus_fmt.saddr_7bit = slv;
	msg.bus_fmt.wr_flag = 0;
	msg.bus_fmt.rs_start = START_WITH_ID_W;
	msg.bus_fmt.rs_mode = STOP_START;
	msg.bus_fmt.addr_len = 1;
	msg.bus_fmt.data_len = 1;
	msg.pkt_buf = buf;
	msg.pkt_num = 1;

	return bsp_cci_tx_start_wait_done(sel, &msg);
}

int cci_wr_16_8(unsigned int sel, unsigned short reg, unsigned char data, unsigned char slv)
{
	struct cci_msg msg;
	unsigned char buf[3];

	buf[0] = (reg&0xff00)>>8;
	buf[1] = (reg&0x00ff);
	buf[2] = data;

	msg.bus_fmt.saddr_7bit = slv;
	msg.bus_fmt.wr_flag = 0;
	msg.bus_fmt.rs_start = START_WITH_ID_W;
	msg.bus_fmt.rs_mode = STOP_START;
	msg.bus_fmt.addr_len = 2;
	msg.bus_fmt.data_len = 1;
	msg.pkt_buf = buf;
	msg.pkt_num = 1;

	return bsp_cci_tx_start_wait_done(sel, &msg);
}

int cci_wr_16_16(unsigned int sel, unsigned short reg, unsigned short data, unsigned char slv)
{
	struct cci_msg msg;
	unsigned char buf[4];

	buf[0] = (reg&0xff00)>>8;
	buf[1] = (reg&0x00ff);
	buf[2] = (data&0xff00)>>8;
	buf[3] = (data&0x00ff);

	msg.bus_fmt.saddr_7bit = slv;
	msg.bus_fmt.wr_flag = 0;
	msg.bus_fmt.rs_start = START_WITH_ID_W;
	msg.bus_fmt.rs_mode = STOP_START;
	msg.bus_fmt.addr_len = 2;
	msg.bus_fmt.data_len = 2;
	msg.pkt_buf = buf;
	msg.pkt_num = 1;

	return bsp_cci_tx_start_wait_done(sel, &msg);
}

int cci_wr_8_16(unsigned int sel, unsigned char reg, unsigned short data, unsigned char slv)
{
	struct cci_msg msg;
	unsigned char buf[3];

	buf[0] = reg;
	buf[1] = (data&0xff00)>>8;
	buf[2] = (data&0x00ff);

	msg.bus_fmt.saddr_7bit = slv;
	msg.bus_fmt.wr_flag = 0;
	msg.bus_fmt.rs_start = START_WITH_ID_W;
	msg.bus_fmt.rs_mode = STOP_START;
	msg.bus_fmt.addr_len = 1;
	msg.bus_fmt.data_len = 2;
	msg.pkt_buf = buf;
	msg.pkt_num = 1;

	return bsp_cci_tx_start_wait_done(sel, &msg);
}

int cci_wr_0_16(unsigned int sel, unsigned short data, unsigned char slv)
{
	struct cci_msg msg;
	unsigned char buf[2];

	buf[0] = (data&0xff00)>>8;
	buf[1] = (data&0x00ff);

	msg.bus_fmt.saddr_7bit = slv;
	msg.bus_fmt.wr_flag = 0;
	msg.bus_fmt.rs_start = START_WITHOUT_ID_W;
	msg.bus_fmt.rs_mode = STOP_START;
	msg.bus_fmt.addr_len = 0;
	msg.bus_fmt.data_len = 2;
	msg.pkt_buf = buf;
	msg.pkt_num = 1;

	return bsp_cci_tx_start_wait_done(sel, &msg);
}

int cci_rd_8_8(unsigned int sel, unsigned char reg, unsigned char *data, unsigned char slv)
{
	struct cci_msg msg;
	unsigned char buf[2];
	int ret;

	buf[0] = reg;

	msg.bus_fmt.saddr_7bit = slv;
	msg.bus_fmt.wr_flag = 1;
	msg.bus_fmt.rs_start = START_WITH_ID_W;
	msg.bus_fmt.rs_mode = STOP_START;
	msg.bus_fmt.addr_len = 1;
	msg.bus_fmt.data_len = 1;
	msg.pkt_buf = buf;
	msg.pkt_num = 1;

	ret = bsp_cci_tx_start_wait_done(sel, &msg);
	*data = buf[1];
	return ret;
}

int cci_rd_16_8(unsigned int sel, unsigned short reg, unsigned char *data, unsigned char slv)
{
	struct cci_msg msg;
	unsigned char buf[3];
	int ret;

	buf[0] = (reg&0xff00)>>8;
	buf[1] = (reg&0x00ff);

	msg.bus_fmt.saddr_7bit = slv;
	msg.bus_fmt.wr_flag = 1;
	msg.bus_fmt.rs_start = START_WITH_ID_W;
	msg.bus_fmt.rs_mode = STOP_START;
	msg.bus_fmt.addr_len = 2;
	msg.bus_fmt.data_len = 1;
	msg.pkt_buf = buf;
	msg.pkt_num = 1;

	ret = bsp_cci_tx_start_wait_done(sel, &msg);
	*data = buf[2];
	return ret;
}

int cci_rd_16_16(unsigned int sel, unsigned short reg, unsigned short *data, unsigned char slv)
{
	struct cci_msg msg;
	unsigned char buf[4];
	int ret;

	buf[0] = (reg&0xff00)>>8;
	buf[1] = (reg&0x00ff);

	msg.bus_fmt.saddr_7bit = slv;
	msg.bus_fmt.wr_flag = 1;
	msg.bus_fmt.rs_start = START_WITH_ID_W;
	msg.bus_fmt.rs_mode = STOP_START;
	msg.bus_fmt.addr_len = 2;
	msg.bus_fmt.data_len = 2;
	msg.pkt_buf = buf;
	msg.pkt_num = 1;

	ret = bsp_cci_tx_start_wait_done(sel, &msg);
	*data = buf[2]*256 + buf[3];

	return ret;
}

int cci_rd_8_16(unsigned int sel, unsigned char reg, unsigned short *data, unsigned char slv)
{
	struct cci_msg msg;
	unsigned char buf[3];
	int ret;

	buf[0] = reg;

	msg.bus_fmt.saddr_7bit = slv;
	msg.bus_fmt.wr_flag = 1;
	msg.bus_fmt.rs_start = START_WITH_ID_W;
	msg.bus_fmt.rs_mode = STOP_START;
	msg.bus_fmt.addr_len = 1;
	msg.bus_fmt.data_len = 2;
	msg.pkt_buf = buf;
	msg.pkt_num = 1;

	ret = bsp_cci_tx_start_wait_done(sel, &msg);
	*data = buf[1]*256 + buf[2];
	return ret;
}

int cci_rd_0_16(unsigned int sel, unsigned short *data, unsigned char slv)
{
	struct cci_msg msg;
	unsigned char buf[2];
	int ret;

	msg.bus_fmt.saddr_7bit = slv;
	msg.bus_fmt.wr_flag = 1;
	msg.bus_fmt.rs_start = START_WITHOUT_ID_W;
	msg.bus_fmt.rs_mode = STOP_START;
	msg.bus_fmt.addr_len = 0;
	msg.bus_fmt.data_len = 2;
	msg.pkt_buf = buf;
	msg.pkt_num = 1;

	ret = bsp_cci_tx_start_wait_done(sel, &msg);
	*data = buf[0]*256 + buf[1];
	return ret;
}

#define CCI_MAX_MSG_LEN 12

int cci_wr_a16_d8_continuous(unsigned int sel, unsigned short reg, unsigned char *data, unsigned char slv, int size)
{
	struct cci_msg msg;
	unsigned char buf[2+CCI_MAX_MSG_LEN];
	int ret = 0, i;

	while (size > 0) {
		int len = size > CCI_MAX_MSG_LEN ? CCI_MAX_MSG_LEN : size;

		buf[0] = (reg&0xff00)>>8;
		buf[1] = (reg&0x00ff);
		for (i = 2; i < 2+len; i++)
			buf[i] = *data++;

		msg.bus_fmt.saddr_7bit = slv;
		msg.bus_fmt.wr_flag = 0;
		msg.bus_fmt.rs_start = START_WITH_ID_W;
		msg.bus_fmt.rs_mode = STOP_START;
		msg.bus_fmt.addr_len = 2;
		msg.bus_fmt.data_len = len;
		msg.pkt_buf = buf;
		msg.pkt_num = 1;
		ret = bsp_cci_tx_start_wait_done(sel, &msg);
		if (ret >= 0) {
			ret = 0;
		} else if (ret < 0) {
			pr_info("[CCI] sensor_write error!\n");
			break;
		}
		reg += len;
		size -= len;
	}

	return ret;
}
