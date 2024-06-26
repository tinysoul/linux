/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright(c) 2020 - 2023 Allwinner Technology Co.,Ltd. All rights reserved. */
/*
 * A V4L2 driver for nvp6324 cameras and AHD Coax protocol.
 *
 * Copyright (c) 2017 by Allwinnertech Co., Ltd.  http://www.allwinnertech.com
 *
 * Authors:  Li Huiyu <lihuiyu@allwinnertech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include "../../../utility/vin_log.h"
#include <linux/string.h>
#include <linux/delay.h>
#include "jaguar1_common.h"
#include "jaguar1_mipi.h"
#include "jaguar1_mipi_table.h"
#include "../sensor_helper.h"

#define SENSOR_NAME "nvp6324_mipi"


static unsigned char mipi_dtype, arb_dtype, en_param;

/*-------------------------------------------------------------------

Arbiter function

-------------------------------------------------------------------*/

static void arb_scale_set(video_input_init *dev_ch_info, unsigned char val)
{
	int devnum = dev_ch_info->ch / 4;
	unsigned char arb_scale = 0;

	gpio_i2c_write(jaguar1_i2c_addr[devnum], 0xFF, 0x20);

	arb_scale = gpio_i2c_read(jaguar1_i2c_addr[devnum], 0x01);
	arb_scale &= ~(0x3<<(dev_ch_info->ch*2));
	arb_scale |= val<<(dev_ch_info->ch*2);

	gpio_i2c_write(jaguar1_i2c_addr[devnum], 0x01, arb_scale);
}

static void arb_enable(int dev_num)
{
	if ((dev_num < 0) || (dev_num > 3)) {
		sensor_dbg("[DRV] %s input channel Error (%d)\n", __func__, dev_num);
		return;
	}

	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0xff, 0x20);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x00, en_param);
	sensor_dbg("VDEC_ARBITER_INIT done 0x%X\n", en_param);
}


static void arb_disable(int dev_num)
{
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0xff, 0x20);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x00, 0x00);
}

void arb_init(int dev_num)
{
	arb_disable(dev_num);

	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0xff, 0x20);

	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x40, 0x01);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x0F, arb_dtype);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x0D, 0x01);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x40, 0x00);

	arb_enable(dev_num);
}


/*-------------------------------------------------------------------

MIPI function

-------------------------------------------------------------------*/

static void mipi_frame_opt_set(video_input_init *dev_ch_info, unsigned char val)
{
	int devnum = dev_ch_info->ch / 4;
	unsigned char mipi_frame_opt;

	gpio_i2c_write(jaguar1_i2c_addr[devnum], 0xFF, 0x21);

	switch (dev_ch_info->ch) {
	case 0:
			mipi_frame_opt = gpio_i2c_read(jaguar1_i2c_addr[devnum], 0x3E);
			mipi_frame_opt = (mipi_frame_opt & 0xF0) | val;
			gpio_i2c_write(jaguar1_i2c_addr[devnum], 0x3E, mipi_frame_opt);
			break;
	case 1:
			mipi_frame_opt = gpio_i2c_read(jaguar1_i2c_addr[devnum], 0x3E);
			mipi_frame_opt = (mipi_frame_opt & 0x0F) | val;
			gpio_i2c_write(jaguar1_i2c_addr[devnum], 0x3E, mipi_frame_opt);
			break;
	case 2:
			mipi_frame_opt = gpio_i2c_read(jaguar1_i2c_addr[devnum], 0x3F);
			mipi_frame_opt = (mipi_frame_opt & 0xF0) | val;
			gpio_i2c_write(jaguar1_i2c_addr[devnum], 0x3F, mipi_frame_opt);
			break;
	case 3:
			mipi_frame_opt = gpio_i2c_read(jaguar1_i2c_addr[devnum], 0x3F);
			mipi_frame_opt = (mipi_frame_opt & 0x0F) | val;
			gpio_i2c_write(jaguar1_i2c_addr[devnum], 0x3F, mipi_frame_opt);
			break;
	}
}

void mipi_video_format_set(video_input_init *dev_ch_info)
{
	mipi_vdfmt_set_s mipi_vd_fmt = (mipi_vdfmt_set_s)decoder_mipi_fmtdef[dev_ch_info->format];

	if (dev_ch_info->interface != _DISABLE) {
		en_param |= 0x11<<(dev_ch_info->ch);
	}

	mipi_frame_opt_set(dev_ch_info, mipi_vd_fmt.mipi_frame_opt);
	arb_scale_set(dev_ch_info, mipi_vd_fmt.arb_scale);
}

int mipi_datatype_set(unsigned char data_type)
{
	int ret = 0;

	switch (data_type) {
		sensor_dbg("data type %d\n", data_type);
	case VD_DATA_TYPE_YUV422:
			mipi_dtype = 0x1E;
			arb_dtype = 0x00;
			break;
	case VD_DATA_TYPE_YUV420:
			mipi_dtype = 0x18;
			arb_dtype = 0xAA;
			break;
	case VD_DATA_TYPE_LEGACY420:
			mipi_dtype = 0x1A;
			arb_dtype = 0x55;
			break;
	default:
			sensor_dbg("[DRV]%s : invalid data type [0x%X]\n", __func__,  data_type);
			ret = -1;
			break;
	}

	return ret;
}

void mipi_tx_init(int dev_num)
{

	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0xFF, 0x21);
#if 1
	/* SET_MIPI_1242MHZ 1080P */
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x40, 0xBC);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x41, 0x00);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x42, 0x03);

	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x43, 0x43);

	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x11, 0x08);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x10, 0x13);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x12, 0x0B);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x13, 0x12);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x17, 0x02);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x18, 0x12);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x15, 0x07);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x14, 0x2D);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x16, 0x0B);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x19, 0x09);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x1A, 0x15);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x1B, 0x11);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x1C, 0x0E);


	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x44, 0x00);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x49, 0xF3);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x49, 0xF0);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x44, 0x02);

	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x08, 0x40);

	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x0F, 0x01);

	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x38, mipi_dtype);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x39, mipi_dtype);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x3A, mipi_dtype);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x3B, mipi_dtype);

	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x07, 0x0F);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x2D, 0x01);
#else
	/* 720P */
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0xFF, 0x21);

	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x40, 0xDC);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x41, 0x10);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x42, 0x03);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x43, 0x43);

	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x10, 0x0C);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x11, 0x05);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x12, 0x07);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x13, 0x0B);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x14, 0x1C);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x15, 0x04);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x16, 0x07);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x17, 0x01);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x18, 0x0E);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x19, 0x06);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x1A, 0x0D);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x1B, 0x0B);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x1C, 0x09);

	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x44, 0x00);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x49, 0xF3);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x49, 0xF0);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x44, 0x02);

	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x08, 0x40);

	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x0F, 0x01);

	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x38, mipi_dtype);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x39, mipi_dtype);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x3A, mipi_dtype);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x3B, mipi_dtype);

	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0x07, 0x0F);
#endif
	sensor_dbg("VDEC_MIPI_TX_INIT done\n");
}

void disable_parallel(int dev_num)
{
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0xFF, 0x01);

	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0xC8, 0x00);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0xC9, 0x00);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0xCA, 0x00);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0xCB, 0x00);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0xCC, 0x00);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0xCD, 0x00);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0xCE, 0x00);
	gpio_i2c_write(jaguar1_i2c_addr[dev_num], 0xCF, 0x00);
	sensor_dbg("Parallel block Disable\n");
}
