/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright(c) 2020 - 2023 Allwinner Technology Co.,Ltd. All rights reserved. */
/*
 * A V4L2 driver for nvp6158c yuv cameras.
 *
 * Copyright (c) 2019 by Allwinnertech Co., Ltd.  http://www.allwinnertech.com
 *
 * Authors:  Zheng Zequn<zequnzheng@allwinnertech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include "../../../utility/vin_log.h"
#include <linux/string.h>
#include <linux/delay.h>
#include "common.h"
#include "../sensor_helper.h"
#include "motion.h"

extern unsigned int nvp6158_iic_addr[4];
#define SENSOR_NAME "nvp6158"

/**************************************************************************************
* @desc
* RAPTOR3's
*
* @param_in (motion_mode *)p_param->channel                  FW Update channel
*
* @return void None
*
* ioctl : IOC_VDEC_MOTION_SET
***************************************************************************************/
void motion_nvp6158_detection_get(motion_mode *motion_set)
{
	/* BANK2_MOTION */
	unsigned char ReadVal = 0;
	unsigned char ch_mask = 1;
	unsigned char ch = motion_set->ch;
	unsigned char ret = 0;

	ch_mask = ch_mask<<ch;

	gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0xFF, 0x00);
	ReadVal = gpio_i2c_read(nvp6158_iic_addr[motion_set->devnum], 0xA9);

	ret = ReadVal&ch_mask;
	motion_set->set_val = ret;

/* sensor_dbg("motion_detection_get:: %x\n", motion_set->set_val); */

}

void motion_nvp6158_onoff_set(motion_mode *motion_set)
{
	/* BANK2_MOTION */
	gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0xFF, 0x02);

	if (motion_set->fmtdef == TVI_3M_18P || motion_set->fmtdef == TVI_5M_12_5P || motion_set->fmtdef == TVI_5M_20P) {
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x00 + (0x07 * motion_set->ch), 0x0C);
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x02 + (0x07 * motion_set->ch), 0x23);
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x28 + (0x06 * motion_set->ch), 0x11);

		if (motion_set->fmtdef == TVI_3M_18P) {
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x29 + (0x06 * motion_set->ch), 0x78);
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2A + (0x06 * motion_set->ch), 0x40);
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2C + (0x06 * motion_set->ch), 0x72);
		} else if (motion_set->fmtdef == TVI_5M_12_5P) {
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x29 + (0x06 * motion_set->ch), 0xA2);
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2A + (0x06 * motion_set->ch), 0x51);
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2C + (0x06 * motion_set->ch), 0x9c);
		} else if (motion_set->fmtdef == TVI_5M_20P) {
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x29 + (0x06 * motion_set->ch), 0xA0);
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2A + (0x06 * motion_set->ch), 0x51);
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2C + (0x06 * motion_set->ch), 0x9a);
		}

		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2B + (0x06 * motion_set->ch), 0x6);

		sensor_dbg("[DRV_Motion_OnOff] ch(%d) fmtdef(%d)\n", motion_set->ch, motion_set->fmtdef);
	} else if (motion_set->fmtdef == AHD20_1080P_15P_EX || motion_set->fmtdef == AHD20_1080P_12_5P_EX) {
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x00 + (0x07 * motion_set->ch), 0x0C);
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x02 + (0x07 * motion_set->ch), 0x23);
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x28 + (0x06 * motion_set->ch), 0x11);
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x29 + (0x06 * motion_set->ch), 0xf0);
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2A + (0x06 * motion_set->ch), 0x2d);
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2B + (0x06 * motion_set->ch), 0x06);
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2C + (0x06 * motion_set->ch), 0xea);
	} else
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x28 + (0x06 * motion_set->ch), 0x00);

	if (motion_set->set_val < 0 || motion_set->set_val > 1) {
		sensor_dbg("[DRV_Motion_OnOff]Error!! ch(%d) Setting Value Over:%x!! Only 0 or 1\n", motion_set->ch, motion_set->set_val);
		return;
	}

	switch (motion_set->set_val) {
	case FUNC_OFF:
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], (0x00 + (0x07 * motion_set->ch)), 0x0D);
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x28 + (0x06 * motion_set->ch), 0x00);
		break;
	case FUNC_ON:
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], (0x00 + (0x07 * motion_set->ch)), 0x0C);
		break;
	}


}

void motion_nvp6158_pixel_all_onoff_set(motion_mode *motion_set)
{
	int ii = 0;
	unsigned char addr = 0;

	/* BANK2_MOTION */
	gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0xFF, 0x02);

	if (motion_set->fmtdef == TVI_3M_18P || motion_set->fmtdef == TVI_5M_12_5P || motion_set->fmtdef == TVI_5M_20P) {
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x00 + (0x07 * motion_set->ch), 0x0C);
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x02 + (0x07 * motion_set->ch), 0x23);
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x28 + (0x06 * motion_set->ch), 0x11);

		if (motion_set->fmtdef == TVI_3M_18P) {
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x29 + (0x06 * motion_set->ch), 0x78);
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2A + (0x06 * motion_set->ch), 0x40);
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2C + (0x06 * motion_set->ch), 0x72);
		} else if (motion_set->fmtdef == TVI_5M_12_5P) {
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x29 + (0x06 * motion_set->ch), 0xA2);
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2A + (0x06 * motion_set->ch), 0x51);
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2C + (0x06 * motion_set->ch), 0x9c);
		} else if (motion_set->fmtdef == TVI_5M_20P) {
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x29 + (0x06 * motion_set->ch), 0xA0);
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2A + (0x06 * motion_set->ch), 0x51);
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2C + (0x06 * motion_set->ch), 0x9a);
		}
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2B + (0x06 * motion_set->ch), 0x6);

		sensor_dbg("[DRV_Motion_OnOff] ch(%d) fmtdef(%d)\n", motion_set->ch, motion_set->fmtdef);
	} else  if (motion_set->fmtdef == AHD20_1080P_15P_EX || motion_set->fmtdef == AHD20_1080P_12_5P_EX) {
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x00 + (0x07 * motion_set->ch), 0x0C);
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x02 + (0x07 * motion_set->ch), 0x23);
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x28 + (0x06 * motion_set->ch), 0x11);
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x29 + (0x06 * motion_set->ch), 0xf0);
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2A + (0x06 * motion_set->ch), 0x2d);
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2B + (0x06 * motion_set->ch), 0x06);
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2C + (0x06 * motion_set->ch), 0xea);
	} else
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x28 + (0x06 * motion_set->ch), 0x00);

	for (ii = 0; ii < 24; ii++) {
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], (0x40 + (0x18 * motion_set->ch)) + ii, motion_set->set_val);
		addr = (0x40 + (0x18 * motion_set->ch)) + ii;
	}
}

void motion_nvp6158_pixel_onoff_set(motion_mode *motion_set)
{
	unsigned char val = 0x80;
	unsigned char ReadVal;
	unsigned char on;

	unsigned char ch      = motion_set->ch;
	unsigned char SetPix  = motion_set->set_val/8;
	unsigned char SetVal  = motion_set->set_val%8;

	val = val >> SetVal;

	/* BANK2_MOTION */
	gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0xFF, 0x02);

	if (motion_set->fmtdef == TVI_3M_18P || motion_set->fmtdef == TVI_5M_12_5P || motion_set->fmtdef == TVI_5M_20P) {
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x00 + (0x07 * motion_set->ch), 0x0C);
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x02 + (0x07 * motion_set->ch), 0x23);
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x28 + (0x06 * motion_set->ch), 0x11);

		if (motion_set->fmtdef == TVI_3M_18P) {
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x29 + (0x06 * motion_set->ch), 0x78);
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2A + (0x06 * motion_set->ch), 0x40);
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2C + (0x06 * motion_set->ch), 0x72);
		} else if (motion_set->fmtdef == TVI_4M_15P) {
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x29 + (0x06 * motion_set->ch), 0xA0);
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2A + (0x06 * motion_set->ch), 0x3C);
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2C + (0x06 * motion_set->ch), 0x9a);
		} else if (motion_set->fmtdef == TVI_5M_12_5P) {
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x29 + (0x06 * motion_set->ch), 0xA2);
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2A + (0x06 * motion_set->ch), 0x51);
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2C + (0x06 * motion_set->ch), 0x9c);
		} else if (motion_set->fmtdef == TVI_5M_20P) {
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x29 + (0x06 * motion_set->ch), 0xA0);
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2A + (0x06 * motion_set->ch), 0x51);
			gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2C + (0x06 * motion_set->ch), 0x9a);
		}

		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2B + (0x06 * motion_set->ch), 0x6);

		sensor_dbg("[DRV_Motion_OnOff] ch(%d) fmtdef(%d)\n", motion_set->ch, motion_set->fmtdef);
	} else if (motion_set->fmtdef == AHD20_1080P_15P_EX || motion_set->fmtdef == AHD20_1080P_12_5P_EX) {
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x00 + (0x07 * motion_set->ch), 0x0C);
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x02 + (0x07 * motion_set->ch), 0x23);
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x28 + (0x06 * motion_set->ch), 0x11);
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x29 + (0x06 * motion_set->ch), 0xf0);
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2A + (0x06 * motion_set->ch), 0x2d);
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2B + (0x06 * motion_set->ch), 0x06);
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x2C + (0x06 * motion_set->ch), 0xea);
	} else
		gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0x28 + (0x06 * motion_set->ch), 0x00);

	ReadVal = gpio_i2c_read(nvp6158_iic_addr[motion_set->devnum], (0x40 + (0x18 * ch)) + SetPix);
	on = val&ReadVal;
	if (on) {
		val = ~val;
		val = val&ReadVal;
	} else {
		val = val|ReadVal;
	}
	gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], (0x40 + (0x18 * ch)) + SetPix, val);
}

void motion_nvp6158_pixel_onoff_get(motion_mode *motion_set)
{
	unsigned char val = 0x80;
	unsigned char ReadVal;
	unsigned char on;

	unsigned char Ch      = motion_set->ch;
	unsigned char SetPix  = motion_set->set_val/8;
	unsigned char SetVal  = motion_set->set_val%8;

	val = val >> SetVal;

	/* BANK2_MOTION */
	gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0xFF, 0x02);
	ReadVal = gpio_i2c_read(nvp6158_iic_addr[motion_set->devnum], (0x40 + (0x18 * Ch)) + SetPix);

	on = val&ReadVal;

	if (on)
		motion_set->set_val = 1;
	else
		motion_set->set_val = 0;
}

void motion_nvp6158_tsen_set(motion_mode *motion_set)
{
	unsigned char ch = motion_set->ch;
	unsigned char SetVal = motion_set->set_val;

	/* BANK2_MOTION */
	gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0xFF, 0x02);
	gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], (0x01 + (0x07 * ch)), SetVal);
	sensor_dbg("[DRV_Motion]ch(%d), TSEN Val(%x)\n", ch, SetVal);
}

void motion_nvp6158_psen_set(motion_mode *motion_set)
{
	unsigned char msb_mask = 0xf0;
	unsigned char lsb_mask = 0x07;
	unsigned char ch = motion_set->ch;
	unsigned char SetVal = motion_set->set_val;
	unsigned char ReadVal;

	/* BANK2_MOTION */
	gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], 0xFF, 0x02);
	ReadVal = gpio_i2c_read(nvp6158_iic_addr[motion_set->devnum], (0x02 + (0x07 * ch)));

	msb_mask = msb_mask&ReadVal;
	SetVal = lsb_mask&SetVal;

	SetVal = SetVal|msb_mask;

	gpio_i2c_write(nvp6158_iic_addr[motion_set->devnum], (0x02 + (0x07 * ch)), SetVal);
	sensor_dbg("[DRV_Motion]ch(%d), readVal(%x), SetVal(%x)\n", ch, ReadVal, SetVal);
}
