/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright(c) 2020 - 2023 Allwinner Technology Co.,Ltd. All rights reserved. */
/*
 * linux-4.9/drivers/media/platform/sunxi-vfe/utility/sensor_info.h
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
 * sensor_info.h
 *
 * Hawkview ISP - sensor_info.h module
 *
 * Copyright (c) 2014 by Allwinnertech Co., Ltd.  http://www.allwinnertech.com
 *
 * Version		  Author         Date		    Description
 *
 *   2.0		  Yang Feng	2014/02/24	      Second Version
 *
 ****************************************************************************************
 */
#ifndef _SENSOR_INFO_H_
#define _SENSOR_INFO_H_

enum sensor_type_t {
	SENSOR_YUV             = 0,
	SENSOR_RAW            = 1,
	SENSOR_YUV_USE_ISP             = 2,
};
enum sensor_size_t {
	PIXEL_NUM_0_3M		 = 0,
	PIXEL_NUM_1M,
	PIXEL_NUM_2M,
	PIXEL_NUM_3M,
	PIXEL_NUM_4M,
	PIXEL_NUM_5M,
	PIXEL_NUM_8M,
	PIXEL_NUM_12M,
	PIXEL_NUM_13M,
	PIXEL_NUM_16M,
	PIXEL_NUM_20M,
	PIXEL_NUM_22M,
};

struct sensor_item {
	char sensor_name[32];
	int i2c_addr;
	enum sensor_type_t  sensor_type;
	enum sensor_size_t  sensor_size;
	int core_clk_for_sensor;
};

int get_sensor_info(char *sensor_name, struct sensor_item *sensor_info);



#endif /*_SENSOR_INFO_H_*/
