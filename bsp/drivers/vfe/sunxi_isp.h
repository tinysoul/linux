/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright(c) 2020 - 2023 Allwinner Technology Co.,Ltd. All rights reserved. */
/*
 * linux-4.9/drivers/media/platform/sunxi-vfe/sunxi_isp.h
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
 * sunxi_isp.h
 *
 * Hawkview ISP - sunxi_isp.h module
 *
 * Copyright (c) 2014 by Allwinnertech Co., Ltd.  http://www.allwinnertech.com
 *
 * Version		  Author         Date		    Description
 *
 *   3.0		  Yang Feng	2014/12/11	ISP Tuning Tools Support
 *
 ****************************************************************************************
 */


#ifndef _SUNXI_ISP_H_
#define _SUNXI_ISP_H_
#include <linux/videodev2.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-subdev.h>
#include "vfe.h"
/* for internal driver debug */
#define isp_dbg(l, x, arg...) ({if (isp_dbg_en && l <= isp_dbg_lv) pr_debug("[ISP_DEBUG]"x, ##arg); })
/* print when error happens */
#define isp_err(x, arg...) pr_err("[ISP_ERR]"x, ##arg)
#define isp_warn(x, arg...) pr_warn("[ISP_WARN]"x, ##arg)
/* print unconditional, for important info */
#define isp_print(x, arg...) pr_info("[ISP]"x, ##arg)

#define VIDIOC_SUNXI_ISP_MAIN_CH_CFG		1
#define VIDIOC_SUNXI_ISP_SUB_CH_CFG		2
#define VIDIOC_SUNXI_ISP_ROT_CH_CFG		3

struct main_channel_cfg {
	enum bus_pixeltype  bus_code;
	struct sensor_win_size win_cfg;
	struct v4l2_pix_format pix;
};

struct isp_pix_fmt {
	char	*name;
	u32	mbus_code;
	u32	fourcc;
	u32	color;
	u16	memplanes;
	u16	colplanes;
	u32	depth[VIDEO_MAX_PLANES];
	u16	mdataplanes;
	u16	flags;
};

struct isp_yuv_size_addr_info {
	unsigned int isp_byte_size;
	unsigned int line_stride_y;
	unsigned int line_stride_c;
	unsigned int buf_height_y;
	unsigned int buf_height_cb;
	unsigned int buf_height_cr;

	unsigned int valid_height_y;
	unsigned int valid_height_cb;
	unsigned int valid_height_cr;
	struct isp_yuv_channel_addr yuv_addr;
};

struct sunxi_isp_ctrls {
	struct v4l2_ctrl_handler handler;

	struct v4l2_ctrl *hflip;
	struct v4l2_ctrl *vflip;
	struct v4l2_ctrl *rotate;
};
struct isp_fmt_cfg {
	int rot_angle;
	int rot_ch;
	enum bus_pixeltype  bus_code;
	enum pixel_fmt isp_fmt[ISP_MAX_CH_NUM];
	struct isp_size isp_size[ISP_MAX_CH_NUM];
	/* struct sensor_win_size win_cfg; */
	struct isp_size ob_black_size;
	struct isp_size ob_valid_size;
	struct coor ob_start;
};

struct isp_dev {
	int use_cnt;
	struct v4l2_subdev subdev;
	struct platform_device  *pdev;
	struct list_head	isp_list;
	struct sunxi_isp_ctrls ctrls;
	int vflip;
	int hflip;
	int rotate;
	unsigned int id;
	spinlock_t slock;
	struct mutex subdev_lock;
	/*int irq;*/
	wait_queue_head_t   wait;
	void __iomem      *base;
	struct resource *ioarea;
	struct vfe_mm isp_load_reg_mm;
	struct vfe_mm isp_save_reg_mm;
	int rotation_en;
	struct isp_fmt_cfg isp_fmt;
	struct v4l2_mbus_framefmt format;
	enum enable_flag flip_en_glb[ISP_MAX_CH_NUM];
	int plannar_uv_exchange_flag[ISP_MAX_CH_NUM];
	struct isp_yuv_size_addr_info isp_yuv_size_addr[ISP_MAX_CH_NUM];
};

void sunxi_isp_set_fmt(struct isp_dev *isp, enum bus_pixeltype type, enum pixel_fmt *fmt);
void sunxi_isp_set_flip(struct isp_dev *isp, enum isp_channel ch, enum enable_flag on_off);
void sunxi_isp_set_mirror(enum isp_channel ch, enum enable_flag on_off);
unsigned int sunxi_isp_set_size(struct isp_dev *isp, enum pixel_fmt *fmt, struct isp_size_settings *size_settings);
void sunxi_isp_set_output_addr(struct v4l2_subdev *sd, unsigned long buf_base_addr);

void sunxi_isp_dump_regs(struct v4l2_subdev *sd);
int sunxi_isp_get_subdev(struct v4l2_subdev **sd, int sel);
int sunxi_isp_put_subdev(struct v4l2_subdev **sd, int sel);
int sunxi_isp_register_subdev(struct v4l2_device *v4l2_dev, struct v4l2_subdev *sd);
void sunxi_isp_unregister_subdev(struct v4l2_subdev *sd);
int  sunxi_isp_platform_register(void);
void  sunxi_isp_platform_unregister(void);

#endif /*_SUNXI_ISP_H_*/
