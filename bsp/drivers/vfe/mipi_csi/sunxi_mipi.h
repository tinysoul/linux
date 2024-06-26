/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright(c) 2020 - 2023 Allwinner Technology Co.,Ltd. All rights reserved. */
/*
 * linux-4.9/drivers/media/platform/sunxi-vfe/mipi_csi/sunxi_mipi.h
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
 * sunxi_mipi.h
 *
 * Hawkview ISP - sunxi_mipi.h module
 *
 * Copyright (c) 2015 by Allwinnertech Co., Ltd.  http://www.allwinnertech.com
 *
 * Version		  Author         Date		    Description
 *
 *   3.0		  Yang Feng	2015/02/27	ISP Tuning Tools Support
 *
 ****************************************************************************************
 */
#ifndef _SUNXI_MIPI__H_
#define _SUNXI_MIPI__H_

#include "../platform_cfg.h"

enum{
	MIPI_CSI_CLK = 0,
	MIPI_DPHY_CLK,
	MIPI_CSI_CLK_SRC,
	MIPI_DPHY_CLK_SRC,
	MIPI_CLK_NUM,
};

#define NOCLK			0xff

struct mipi_dev {
	int use_cnt;
	struct v4l2_subdev subdev;
	struct platform_device  *pdev;
	struct list_head	mipi_list;
	unsigned int id;
	spinlock_t slock;
	wait_queue_head_t   wait;
	void __iomem	*base;
	struct mipi_para        mipi_para;
	struct mipi_fmt         mipi_fmt;
	struct clk				*clock[MIPI_CLK_NUM];
};

int sunxi_mipi_get_subdev(struct v4l2_subdev **sd, int sel);
int sunxi_mipi_put_subdev(struct v4l2_subdev **sd, int sel);
int sunxi_mipi_register_subdev(struct v4l2_device *v4l2_dev, struct v4l2_subdev *sd);
void sunxi_mipi_unregister_subdev(struct v4l2_subdev *sd);
int sunxi_mipi_platform_register(void);
void sunxi_mipi_platform_unregister(void);

#endif /*_SUNXI_MIPI__H_*/
