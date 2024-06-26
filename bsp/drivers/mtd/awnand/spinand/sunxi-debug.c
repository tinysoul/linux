// SPDX-License-Identifier: GPL-2.0
/* Copyright(c) 2020 - 2023 Allwinner Technology Co.,Ltd. All rights reserved. */

//#define pr_fmt(fmt) "sunxi-spinand: " fmt
#define SUNXI_MODNAME "sunxi-spinand"
#include <sunxi-log.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sysfs.h>
#include <linux/mtd/aw-spinand.h>
#include <linux/mtd/partitions.h>

#include "sunxi-spinand.h"

static ssize_t aw_spinand_sysfs_show(struct kobject *kobj,
		struct attribute *attr, char *buf);
static ssize_t aw_spinand_sysfs_store(struct kobject *kobj,
		struct attribute *attr, const char *buf, size_t count);
static ssize_t aw_spinand_show_arch(struct aw_spinand *spinand, char *buf);
static ssize_t aw_spinand_show_nanddbg(struct aw_spinand *spinand, char *buf);
static ssize_t aw_spinand_show_version(struct aw_spinand *spinand, char *buf);
static ssize_t aw_spinand_show_badblk(struct aw_spinand *spinand, char *buf);

static struct attribute attr_debug = {
	.name = "nand_debug",
	.mode = S_IRUGO,
};

static struct attribute attr_arch = {
	.name = "arch",
	.mode = S_IRUGO,
};

static struct attribute attr_badblk = {
	.name = "badblock",
	.mode = S_IRUGO,
};

static struct attribute attr_version = {
	.name = "version",
	.mode = S_IRUGO,
};

static struct attribute *sysfs_attrs[] = {
	&attr_debug,
	&attr_arch,
	&attr_badblk,
	&attr_version,
	NULL,
};

struct aw_spinand_attr {
	struct attribute *attr;
	ssize_t (*show)(struct aw_spinand *spinand, char *buf);
	ssize_t (*store)(struct aw_spinand *spinand, const char *buf,
			size_t cnt);
};

static struct aw_spinand_attr attr_ops_array[] = {
	{
		.attr = &attr_debug,
		.show = aw_spinand_show_nanddbg,
	},
	{
		.attr = &attr_arch,
		.show = aw_spinand_show_arch,
	},
	{
		.attr = &attr_version,
		.show = aw_spinand_show_version,
	},
	{
		.attr = &attr_badblk,
		.show = aw_spinand_show_badblk,
	},
};

static const struct sysfs_ops sysfs_ops = {
	.show = aw_spinand_sysfs_show,
	.store = aw_spinand_sysfs_store,
};

static struct kobj_type sysfs_type = {
	.sysfs_ops = &sysfs_ops,
	.default_attrs = sysfs_attrs,
};

static struct kobject aw_spinand_debug_kobj;

static int __init aw_spinand_debug_init(void)
{
	int ret;
	struct aw_spinand *spinand = get_aw_spinand();

	if (!spinand)
		return -EBUSY;

	ret = kobject_init_and_add(&aw_spinand_debug_kobj, &sysfs_type, NULL,
				"nand_driver0");
	if (ret) {
		sunxi_err(NULL, "init nand sysfs fail!\n");
		return ret;
	}
	return 0;
}
module_init(aw_spinand_debug_init);

static void __exit aw_spinand_debug_exit(void)
{
	struct aw_spinand *spinand = get_aw_spinand();

	if (spinand)
		kobject_del(&aw_spinand_debug_kobj);
}
module_exit(aw_spinand_debug_exit);

static ssize_t aw_spinand_sysfs_show(struct kobject *kobj, struct attribute *attr,
		char *buf)
{
	int index;
	struct aw_spinand_attr *spinand_attr;
	struct aw_spinand *spinand = get_aw_spinand();

	BUG_ON(!spinand);

	for (index = 0; index < ARRAY_SIZE(attr_ops_array); index++) {
		spinand_attr = &attr_ops_array[index];
		if (attr == spinand_attr->attr)
			break;
	}

	if (unlikely(index == ARRAY_SIZE(attr_ops_array))) {
		sunxi_err(NULL, "not found attr_ops for %s\n", attr->name);
		return -EINVAL;
	}

	if (spinand_attr->show)
		return spinand_attr->show(spinand, buf);
	return -EINVAL;
}

static ssize_t aw_spinand_sysfs_store(struct kobject *kobj, struct attribute *attr,
		const char *buf, size_t count)
{
	int index;
	struct aw_spinand_attr *spinand_attr;
	struct aw_spinand *spinand = get_aw_spinand();

	BUG_ON(!spinand);

	for (index = 0; index < ARRAY_SIZE(attr_ops_array); index++) {
		spinand_attr = &attr_ops_array[index];
		if (attr == spinand_attr->attr)
			break;
	}

	if (unlikely(index == ARRAY_SIZE(attr_ops_array))) {
		sunxi_err(NULL, "not found attr_ops for %s\n", attr->name);
		return -EINVAL;
	}

	if (spinand_attr->store)
		return spinand_attr->store(spinand, buf, count);
	return -EINVAL;
}

static ssize_t aw_spinand_show_arch(struct aw_spinand *spinand, char *buf)
{
	struct aw_spinand_chip *chip = spinand_to_chip(spinand);
	struct aw_spinand_info *info = chip->info;
	unsigned char ids[MAX_ID_LEN];
	unsigned int id, pagesize;
	ssize_t ret = 0;

	pagesize = 1 << PAGE_SHIFT;
	info->nandid(chip, ids, MAX_ID_LEN);
	id = ids[0] << 0 | ids[1] << 8 | ids[2] << 16 | ids[3] << 24;

	ret += snprintf(buf + ret, pagesize - ret, "Model: %s\n",
			info->model(chip));
	ret += snprintf(buf + ret, pagesize - ret, "NandID: 0x%x\n", id);
	ret += snprintf(buf + ret, pagesize - ret, "Size: %dM\n",
			info->total_size(chip) / SZ_1M);
	ret += snprintf(buf + ret, pagesize - ret, "DieCntPerChip: %d\n",
			info->die_cnt(chip));
	ret += snprintf(buf + ret, pagesize - ret, "SectCntPerPage: %d\n",
			info->phy_page_size(chip) >> SECTOR_SHIFT);
	ret += snprintf(buf + ret, pagesize - ret, "PageCntPerBlk: %d\n",
			info->phy_block_size(chip) / info->phy_page_size(chip));
	ret += snprintf(buf + ret, pagesize - ret, "BlkCntPerDie: %d\n",
			info->total_size(chip) / info->phy_block_size(chip));
	ret += snprintf(buf + ret, pagesize - ret, "OperationOpt: 0x%x\n",
			info->operation_opt(chip));
	ret += snprintf(buf + ret, pagesize - ret, "MaxEraseTimes: %d\n",
			info->max_erase_times(chip));
	ret += snprintf(buf + ret, pagesize - ret, "Manufacture: %s\n",
			info->manufacture(chip));
	return ret;
}

static ssize_t aw_spinand_show_version(struct aw_spinand *spinand, char *buf)
{
	return snprintf(buf, 1 << PAGE_SHIFT, "%x.%x %x\n",
			AW_MTD_SPINAND_VER_MAIN, AW_MTD_SPINAND_VER_SUB,
			AW_MTD_SPINAND_VER_DATE);
}

/*
 * It is ok to loop for much times to check bad block, because spinand has bad
 * block table on ddr. It will not take so long.
 */
static ssize_t aw_spinand_show_badblk(struct aw_spinand *spinand, char *buf)
{
	struct aw_spinand_chip *chip = spinand_to_chip(spinand);
	struct aw_spinand_info *info = chip->info;
	struct aw_spinand_chip_ops *ops = chip->ops;
	struct aw_spinand_chip_request req = {0};
	unsigned int cnt = 0, blk, blkcnt, pagesize;
	ssize_t ret = 0;

	pagesize = 1 << PAGE_SHIFT;
	blkcnt = info->total_size(chip) / info->block_size(chip);

	/* logic bad block count */
	for (blk = 0; blk < blkcnt; blk++) {
		req.block = blk;
		if (ops->is_bad(chip, &req) == true)
			cnt++;
	}
	ret += snprintf(buf + ret, pagesize - ret, "cnt: %u\n", cnt);

	/* logic bad block num */
	ret += snprintf(buf + ret, pagesize - ret, "blk:");
	for (blk = 0; blk < blkcnt; blk++) {
		req.block = blk;
		if (ops->is_bad(chip, &req) == true)
			ret += snprintf(buf + ret, pagesize - ret, " %u", blk);
	}
	ret += snprintf(buf + ret, pagesize - ret, "\n");

	/* physic bad block count */
	blkcnt = info->total_size(chip) / info->phy_block_size(chip);
	for (cnt = 0, blk = 0; blk < blkcnt; blk++) {
		req.block = blk;
		if (ops->phy_is_bad(chip, &req) == true)
			cnt++;
	}
	ret += snprintf(buf + ret, pagesize - ret, "phy cnt: %u\n", cnt);

	/* logic bad block num */
	ret += snprintf(buf + ret, pagesize - ret, "phy blk:");
	for (blk = 0; blk < blkcnt; blk++) {
		req.block = blk;
		if (ops->phy_is_bad(chip, &req) == true)
			ret += snprintf(buf + ret, pagesize - ret, " %u", blk);
	}
	ret += snprintf(buf + ret, pagesize - ret, "\n");

	return ret;
}

static ssize_t aw_spinand_show_nanddbg(struct aw_spinand *spinand, char *buf)
{
	struct aw_spinand_chip *chip = spinand_to_chip(spinand);
	struct aw_spinand_info *info = chip->info;
	struct aw_spinand_chip_ops *ops = chip->ops;
	struct aw_spinand_chip_request req = {0};
	unsigned int pagesize = 1 << PAGE_SHIFT, blkcnt, cnt = 0, blk;
	ssize_t ret = 0;

	/* arch */
	ret += aw_spinand_show_arch(spinand, buf);
	/* bad block */
	blkcnt = info->total_size(chip) / info->block_size(chip);
	for (blk = 0; blk < blkcnt; blk++) {
		req.block = blk;
		if (ops->is_bad(chip, &req) == true)
			cnt++;
	}
	ret += snprintf(buf + ret, pagesize - ret, "BadBlkCnt: %u\n", cnt);
	/* version */
	ret += snprintf(buf + ret, pagesize - ret, "NandVersion: %x.%x %x\n",
			AW_MTD_SPINAND_VER_MAIN, AW_MTD_SPINAND_VER_SUB,
			AW_MTD_SPINAND_VER_DATE);
	return ret;
}
