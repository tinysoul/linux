/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright(c) 2020 - 2023 Allwinner Technology Co.,Ltd. All rights reserved. */
/*
 * linux-4.9/drivers/media/platform/sunxi-vfe/isp_cfg/isp_cfg.c
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
 * isp_cfg.c
 *
 * Hawkview ISP - isp_cfg.c module
 *
 * Copyright (c) 2015 by Allwinnertech Co., Ltd.  http://www.allwinnertech.com
 *
 * Version		  Author         Date		    Description
 *
 *   3.0		  Yang Feng	2015/01/18	ISP Tuning Tools Support
 *
 ****************************************************************************************
 */


#include <linux/kernel.h>
#include <linux/string.h>
#include "isp_cfg.h"
#include "SENSOR_H/ov2710_mipi_isp_cfg.h"
#include "SENSOR_H/hm2131_default_f35.h"
#include "SENSOR_H/ov2775_mipi_default_v3.h"
#include "SENSOR_H/gc1034_mipi_default_v3.h"
#define ISP_CFG_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

struct isp_cfg_item isp_cfg_array[] = {
	{"ov2710_mipi", &ov2710_mipi_isp_cfg,},
	{"hm2131", &hm2131_default_f35,},
	{"ov2775_mipi",	&ov2775_mipi_default_v3,},
	{"gc1034_mipi",	&gc1034_mipi_default_v3,},
};

int get_isp_cfg(char *isp_cfg_name, struct isp_cfg_item *isp_cfg_info)
{
	int i;

	for (i = 0; i < ISP_CFG_ARRAY_SIZE(isp_cfg_array); i++) {
		if (strcmp(isp_cfg_name, isp_cfg_array[i].isp_cfg_name) == 0) {
			*isp_cfg_info = isp_cfg_array[i];

			return 0;
		}
	}
	pr_info("[VFE_WARN]NOT found this item:%s, you can add this ISP Config in the isp_cfg_array!\n", isp_cfg_name);
	return -1;
}
