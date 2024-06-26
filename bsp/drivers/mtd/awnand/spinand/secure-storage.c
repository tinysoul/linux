// SPDX-License-Identifier: GPL-2.0
/* Copyright(c) 2020 - 2023 Allwinner Technology Co.,Ltd. All rights reserved. */

//#define pr_fmt(fmt) "sunxi-spinand: " fmt
#define SUNXI_MODNAME "sunxi-spinand"
#include <sunxi-log.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/spi/spi.h>
#include <linux/mtd/aw-spinand.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/mtd.h>

#define SEC_STO_MAGIC 0x5CAA
#define SEC_STO_ITEM_BYTES (4096)

struct sec_sto_oob {
	__u8 badflag;
	__u16 magic;
	__u32 checksum;
} __packed;

static inline bool is_init_end(struct aw_spinand_sec_sto *sec_sto)
{
	return !!sec_sto->init_end;
}

static int is_secure_storage_block(struct aw_spinand_chip *chip,
		unsigned int blk, unsigned int page)
{
	struct aw_spinand_chip_request req = {0};
	struct aw_spinand_chip_ops *ops = chip->ops;
	struct sec_sto_oob oob;
	int ret;

	req.block = blk;
	req.page = page;
	req.oobbuf = &oob;
	req.ooblen = sizeof(struct sec_sto_oob);
	ret = ops->phy_read_page(chip, &req);
	if (ret) {
		sunxi_err(NULL, "check valid secure storage failed with %d back\n", ret);
		return ret;
	}

	return le16_to_cpu(oob.magic) == (u16)SEC_STO_MAGIC;
}

static int get_blocks_for_secure_storage(struct aw_spinand_sec_sto *sec_sto)
{
	struct aw_spinand_chip *chip = sec_sto->chip;
	struct aw_spinand_chip_ops *ops = chip->ops;
	struct aw_spinand_chip_request req = {0};
	unsigned int i = 0, blk[2] = {0};
	unsigned int start, end;
	int ret;

	start = sec_sto->startblk;
	end = sec_sto->endblk;
	/* find the frist two valid blocks */
	for (; start < end; start++) {
		req.block = start;

		ret = ops->phy_is_bad(chip, &req);
		if (ret == true)
			continue;

		blk[i++] = start;
		if (i >= 2)
			break;
	}
	if (i < 2) {
		sunxi_err(NULL, "no enough good blk between [%u %u) for secure storage\n",
				start, end);
		return -ENOSPC;
	}

	sec_sto->blk[0] = blk[0];
	sec_sto->blk[1] = blk[1];
	return 0;
}

static unsigned int secure_storage_checksum(char *_buf, unsigned int len)
{
	unsigned int sum = 0;
	unsigned int i;
	unsigned char *buf = (unsigned char *)_buf;

	for (i = 0; i < len; i++)
		sum += buf[i];
	return sum;
}

static int read_check_secure_storage_page(struct aw_spinand_chip *chip,
		unsigned int blk, int page, char *mbuf, unsigned int len,
		struct sec_sto_oob *oob)
{
	struct aw_spinand_chip_ops *ops = chip->ops;
	struct aw_spinand_chip_request req = {0};
	struct sec_sto_oob _oob;
	unsigned int checksum;
	int ret;

	req.block = blk;
	req.page = page;
	req.databuf = mbuf;
	req.datalen = len;
	req.oobbuf = oob ? oob : &_oob;
	req.ooblen = sizeof(struct sec_sto_oob);

	ret = ops->phy_read_page(chip, &req);
	if (ret)
		return ret;

	checksum = secure_storage_checksum(mbuf, len);
	if (oob)
		return (unsigned int)oob->checksum != checksum;
	else
		return (unsigned int)_oob.checksum != checksum;
}

static int write_check_secure_storage_page(struct aw_spinand_chip *chip,
		unsigned int blk, int page, char *mbuf, unsigned int len,
		struct sec_sto_oob *oob)
{
	struct aw_spinand_chip_ops *ops = chip->ops;
	struct aw_spinand_chip_request req = {0};
	int ret;

	req.block = blk;
	req.page = page;
	req.databuf = mbuf;
	req.datalen = len;
	req.oobbuf = oob;
	req.ooblen = oob ? sizeof(struct sec_sto_oob) : 0;

	if (req.page == 0) {
		ret = ops->phy_erase_block(chip, &req);
		if (ret)
			return ret;
	}

	ret = ops->phy_write_page(chip, &req);
	if (ret)
		sunxi_err(NULL, "write secure storage failed: %d\n", ret);

	return ret;
}

#define BOTH_SEC_BLKS_GOOD 0
#define FIRST_SEC_BLK_GOOD 1
#define SECOND_SEC_BLK_GOOD 2
static int check_secure_storage(struct aw_spinand_sec_sto *sec_sto)
{
	struct aw_spinand_chip *chip = sec_sto->chip;
	struct aw_spinand_info *info = chip->info;
	int ret, ret1, ret2, i, blk1, blk2;
	unsigned int pagecnt;
	char *buf;

	buf = kmalloc(info->phy_page_size(chip), GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	blk1 = sec_sto->blk[0];
	blk2 = sec_sto->blk[1];
	pagecnt = info->phy_block_size(chip) / info->phy_page_size(chip);
	for (i = 0; i < pagecnt; i++) {
		ret1 = read_check_secure_storage_page(chip, blk1, i, buf,
				info->phy_page_size(chip), NULL);
		ret2 = read_check_secure_storage_page(chip, blk2, i, buf,
				info->phy_page_size(chip), NULL);
		if (ret1 != 0 || ret2 != 0)
			break;
	}

	/*
	 * retX negative: read failed
	 * retX 0: read and check good
	 * retX 1: read good but check failed
	 */
	if (ret1 == 0 && ret2 == 0)
		ret = BOTH_SEC_BLKS_GOOD;
	else if (ret1 == 0 && ret2 != 0)
		ret = FIRST_SEC_BLK_GOOD;
	else if (ret2 == 0 && ret1 != 0)
		ret = SECOND_SEC_BLK_GOOD;
	else if (ret1 == 1 && ret2 == 1)
		ret = BOTH_SEC_BLKS_GOOD;
	else
		ret = -EINVAL;

	if (ret < 0)
		sunxi_err(NULL, "nand secure storage check page %u fail\n", i);
	kfree(buf);
	return ret;
}

static int repair_secure_storage(struct aw_spinand_sec_sto *sec_sto,
		int good_blk_index)
{
	struct aw_spinand_chip *chip = sec_sto->chip;
	struct aw_spinand_chip_ops *ops = chip->ops;
	unsigned int from, to;
	int ret;

	if (good_blk_index == FIRST_SEC_BLK_GOOD) {
		from = sec_sto->blk[0];
		to = sec_sto->blk[1];
	} else {
		from = sec_sto->blk[1];
		to = sec_sto->blk[0];
	}

	ret = ops->phy_copy_block(chip, from, to);
	if (ret)
		sunxi_err(NULL, "copy phy block from %u to %u failed: %d\n", from, to, ret);
	return ret;
}

static int init_secure_storage(struct aw_spinand_sec_sto *sec_sto, int update)
{
	int ret;

	ret = get_blocks_for_secure_storage(sec_sto);
	if (ret)
		return ret;
	sunxi_info(NULL, "spinand secure storage ok for phy blk %u and %u\n",
			sec_sto->blk[0], sec_sto->blk[1]);

	/*
	 * both of blk1 and blk2 has no valid data, it may be no any data had
	 * been writen to secure storage.
	 */
	ret = is_secure_storage_block(sec_sto->chip, sec_sto->blk[0], 0);
	if (ret == false) {
		ret = is_secure_storage_block(sec_sto->chip, sec_sto->blk[1], 0);
		if (ret == false) {
			sunxi_info(NULL, "secure storage blks have never used before\n");
			goto end;
		}
	}

	ret = check_secure_storage(sec_sto);
	if (ret < 0) {
		sunxi_err(NULL, "check secure storage failed with %d\n", ret);
		return ret;
	} else if (ret > 0) {
		sunxi_info(NULL, "try to repair secure storage from block %u\n",
				sec_sto->blk[ret - 1]);
		ret = repair_secure_storage(sec_sto, ret);
		if (ret) {
			sunxi_err(NULL, "repair secure storage failed with %d\n", ret);
			return ret;
		}
	}
end:
	sunxi_debug(NULL, "init secure storage ok\n");
	sec_sto->init_end = true;
	return 0;
}

int aw_spinand_secure_storage_read(struct aw_spinand_sec_sto *sec_sto,
		int item, char *buf, unsigned int len)
{
	struct aw_spinand_chip *chip;
	struct aw_spinand_info *info;
	int ret = 0, pages_cnt_per_item, minlen, i;
	char *pagebuf;
	unsigned int page, pagesize;

	sunxi_debug(NULL, "try to read item %d with len %d\n", item, len);
	if (!sec_sto || !sec_sto->chip)
		return -EINVAL;

	chip = sec_sto->chip;
	info = chip->info;

	if (len % 1024) {
		sunxi_err(NULL, "secure storage need len (%d) align to 1024B\n", len);
		return -EINVAL;
	}

	if (!is_init_end(sec_sto)) {
		ret = init_secure_storage(sec_sto, 1);
		if (ret)
			return ret;
	}

	pagebuf = kzalloc(info->phy_page_size(chip), GFP_KERNEL);
	if (!pagebuf) {
		sunxi_err(NULL, "no memory for reading secure storage page\n");
		return -ENOMEM;
	}

	pagesize = info->phy_page_size(chip);
	pages_cnt_per_item = DIV_ROUND_UP(len, pagesize);
	for (i = 0; i < pages_cnt_per_item; i++) {
		page = item * pages_cnt_per_item + i;

		ret = read_check_secure_storage_page(chip, sec_sto->blk[0],
				page, pagebuf, pagesize, NULL);
		if (ret)
			ret = read_check_secure_storage_page(chip,
					sec_sto->blk[1], page, pagebuf,
					pagesize, NULL);

		if (ret < 0) {
			sunxi_err(NULL, "read secure storage page failed with %d back\n", ret);
			break;
		} else if (ret == true) {
			sunxi_info(NULL, "secure storage has no valid data on item %d\n", item);
			break;
		}

		minlen = min(len, pagesize);
		memcpy(buf + i * pagesize, pagebuf, minlen);
		len -= minlen;
	}

	kfree(pagebuf);
	return ret;
}
EXPORT_SYMBOL(aw_spinand_secure_storage_read);

int aw_spinand_secure_storage_write(struct aw_spinand_sec_sto *sec_sto,
		int item, char *buf, unsigned int len)
{
	struct aw_spinand_chip *chip;
	struct aw_spinand_info *info;
	int ret = 0, pages_cnt_per_item, minlen;
	char *pagebuf;
	struct sec_sto_oob oob;
	unsigned int pagenum, pagesize, pagecnt;

	sunxi_debug(NULL, "try to write item %d with len %d\n", item, len);
	if (!sec_sto || !sec_sto->chip)
		return -EINVAL;

	chip = sec_sto->chip;
	info = chip->info;

	if (len % 1024) {
		sunxi_err(NULL, "secure storage need len (%d) align to 1024B\n", len);
		return -EINVAL;
	}

	if (!is_init_end(sec_sto)) {
		ret = init_secure_storage(sec_sto, 1);
		if (ret)
			return ret;
	}

	pagesize = info->phy_page_size(chip);
	pagecnt = info->phy_block_size(chip) / info->phy_page_size(chip);
	pagebuf = kzalloc(pagesize, GFP_KERNEL);
	if (!pagebuf)
		return -ENOMEM;

	pages_cnt_per_item = DIV_ROUND_UP(len, pagesize);
	for (pagenum = 0; pagenum < pagecnt; pagenum++) {
		if (pagenum / pages_cnt_per_item == item) {
			unsigned int off;

			memset(pagebuf, 0xFF, pagesize);
			minlen = min(len, pagesize);
			off = pagesize * (pagenum % pages_cnt_per_item);
			len -= minlen;

			memcpy(pagebuf, buf + off, minlen);

			oob.badflag = 0xFF;
			oob.magic = cpu_to_le16(SEC_STO_MAGIC);
			oob.checksum = secure_storage_checksum(pagebuf, pagesize);
		} else {
			memset(pagebuf, 0, pagesize);
			ret = read_check_secure_storage_page(chip,
					sec_sto->blk[0], pagenum,
					pagebuf, pagesize, &oob);
			if (ret < 0)
				goto out;
		}
		ret = write_check_secure_storage_page(chip, sec_sto->blk[1],
				pagenum, pagebuf, pagesize, &oob);
		if (ret)
			goto out;
	}

	ret = repair_secure_storage(sec_sto, SECOND_SEC_BLK_GOOD);
	if (!ret)
		sunxi_info(NULL, "write secure storage itme %d ok\n", item);
out:
	kfree(pagebuf);
	return ret;
}
EXPORT_SYMBOL(aw_spinand_secure_storage_write);
