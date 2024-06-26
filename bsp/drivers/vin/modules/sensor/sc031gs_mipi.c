/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright(c) 2020 - 2023 Allwinner Technology Co.,Ltd. All rights reserved. */
/*
 * A V4L2 driver for sc031gs_mipi cameras.
 *
 * Copyright (c) 2019 by Allwinnertech Co., Ltd.  http://www.allwinnertech.com
 *
 * Authors:  Zheng ZeQun <zequnzheng@allwinnertech.com>
 *    Liang WeiJie <liangweijie@allwinnertech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include "../../utility/vin_log.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/videodev2.h>
#include <linux/clk.h>
#include <media/v4l2-device.h>
#include <media/v4l2-mediabus.h>
#include <linux/io.h>

#include "camera.h"
#include "sensor_helper.h"

MODULE_AUTHOR("lwj");
MODULE_DESCRIPTION("A low-level driver for sc031gs sensors");
MODULE_LICENSE("GPL");

#define MCLK              (24*1000*1000)
#define V4L2_IDENT_SENSOR 0x0031

/*
 * Our nominal (default) frame rate.
 */
#define SENSOR_FRAME_RATE 30

/*
 * The sc031gs_mipi sits on i2c with ID 0x60
 */
#define I2C_ADDR 0x60
#define SENSOR_NAME "sc031gs_mipi"
/*
 * The sc031gs_mipi lane select one or two
 */
#define MIPI_LANE_NUM 2

struct cfg_array { /* coming later */
	struct regval_list *regs;
	int size;
};

/*
 * The default register settings
 *
 */
static struct regval_list sensor_default_regs[] = {

};

/* 640x480 RAW 30fps 24MHz */
/*
static struct regval_list sensor_VGA_30fps_1lane_regs[] = {
	{0x0103, 0x01},
	{REG_DLY, 0x10},
	{0x0100, 0x00},
	{0x3000, 0x00},
	{0x3001, 0x00},
	{0x300f, 0x0f},
	{0x3018, 0x13},
	{0x3019, 0xfe},
	{0x301c, 0x78},
	{0x3031, 0x0a},
	{0x3037, 0x20},
	{0x303f, 0x01},
	{0x320c, 0x03},
	{0x320d, 0x6e},
	/* 30fps
	{0x320e, 0x0a},
	{0x320f, 0xac},
	{0x3252, 0x0a},
	{0x3253, 0xa7},

	{0x3220, 0x10},
	{0x3250, 0xc0},
	{0x3251, 0x02},
	{0x3254, 0x02},
	{0x3255, 0x07},
	{0x3304, 0x48},
	{0x3306, 0x38},
	{0x3309, 0x68},
	{0x330b, 0xe0},
	{0x330c, 0x18},
	{0x330f, 0x20},
	{0x3310, 0x10},
	{0x3314, 0x1e},
	{0x3315, 0x38},
	{0x3316, 0x40},
	{0x3317, 0x10},
	{0x3329, 0x34},
	{0x332d, 0x34},
	{0x332f, 0x38},
	{0x3335, 0x3c},
	{0x3344, 0x3c},
	{0x335b, 0x80},
	{0x335f, 0x80},
	{0x3366, 0x06},
	{0x3385, 0x31},
	{0x3387, 0x51},
	{0x3389, 0x01},
	{0x33b1, 0x03},
	{0x33b2, 0x06},
	{0x3621, 0xa4},
	{0x3622, 0x05},
	{0x3624, 0x47},
	{0x3630, 0x46},
	{0x3631, 0x48},
	{0x3633, 0x52},
	{0x3635, 0x18},
	{0x3636, 0x25},
	{0x3637, 0x89},
	{0x3638, 0x0f},
	{0x3639, 0x08},
	{0x363a, 0x00},
	{0x363b, 0x48},
	{0x363c, 0x06},
	{0x363d, 0x00},
	{0x363e, 0xf8},
	{0x3640, 0x00},
	{0x3641, 0x01},
	{0x36e9, 0x00},
	{0x36ea, 0x3b},
	{0x36eb, 0x0e},
	{0x36ec, 0x0e},
	{0x36ed, 0x33},
	{0x36f9, 0x00},
	{0x36fa, 0x3a},
	{0x36fc, 0x01},
	{0x3908, 0x91},
	{0x3d08, 0x01},
	{0x3e01, 0x14},
	{0x3e02, 0x80},
	{0x3e06, 0x0c},
	{0x4500, 0x59},
	{0x4501, 0xc4},
	{0x4603, 0x00},
	{0x4800, 0x64},
	{0x4809, 0x01},
	{0x4837, 0x1b},
	{0x5011, 0x00},
	{0x5988, 0x02},
	{0x598e, 0x03},
	{0x598f, 0x3c},
	{0x3235, 0x00},
	{0x3236, 0x00},
	{0x3802, 0x01},
	{0x3361, 0x00},
	{0x0100, 0x01},

	{0x4418, 0x08},
	{0x4419, 0x8e},
};
*/
/* 640x480 RAW 120fps 24MHz */
/*
static struct regval_list sensor_VGA_120fps_1lane_regs[] = {
	{0x0103, 0x01},
	{REG_DLY, 0x10},
	{0x0100, 0x00},
	{0x3000, 0x00},
	{0x3001, 0x00},
	{0x300f, 0x0f},
	{0x3018, 0x13},
	{0x3019, 0xfe},
	{0x301c, 0x78},
	{0x3031, 0x0a},
	{0x3037, 0x20},
	{0x303f, 0x01},
	{0x320c, 0x03},
	{0x320d, 0x6e},
	/* 120fps
	{0x320e, 0x02},
	{0x320f, 0xab},
	{0x3252, 0x02},
	{0x3253, 0xa6},

	{0x3220, 0x10},
	{0x3250, 0xc0},
	{0x3251, 0x02},
	{0x3254, 0x02},
	{0x3255, 0x07},
	{0x3304, 0x48},
	{0x3306, 0x38},
	{0x3309, 0x68},
	{0x330b, 0xe0},
	{0x330c, 0x18},
	{0x330f, 0x20},
	{0x3310, 0x10},
	{0x3314, 0x1e},
	{0x3315, 0x38},
	{0x3316, 0x40},
	{0x3317, 0x10},
	{0x3329, 0x34},
	{0x332d, 0x34},
	{0x332f, 0x38},
	{0x3335, 0x3c},
	{0x3344, 0x3c},
	{0x335b, 0x80},
	{0x335f, 0x80},
	{0x3366, 0x06},
	{0x3385, 0x31},
	{0x3387, 0x51},
	{0x3389, 0x01},
	{0x33b1, 0x03},
	{0x33b2, 0x06},
	{0x3621, 0xa4},
	{0x3622, 0x05},
	{0x3624, 0x47},
	{0x3630, 0x46},
	{0x3631, 0x48},
	{0x3633, 0x52},
	{0x3635, 0x18},
	{0x3636, 0x25},
	{0x3637, 0x89},
	{0x3638, 0x0f},
	{0x3639, 0x08},
	{0x363a, 0x00},
	{0x363b, 0x48},
	{0x363c, 0x06},
	{0x363d, 0x00},
	{0x363e, 0xf8},
	{0x3640, 0x00},
	{0x3641, 0x01},
	{0x36e9, 0x00},
	{0x36ea, 0x3b},
	{0x36eb, 0x0e},
	{0x36ec, 0x0e},
	{0x36ed, 0x33},
	{0x36f9, 0x00},
	{0x36fa, 0x3a},
	{0x36fc, 0x01},
	{0x3908, 0x91},
	{0x3d08, 0x01},
	{0x3e01, 0x14},
	{0x3e02, 0x80},
	{0x3e06, 0x0c},
	{0x4500, 0x59},
	{0x4501, 0xc4},
	{0x4603, 0x00},
	{0x4800, 0x64},
	{0x4809, 0x01},
	{0x4837, 0x1b},
	{0x5011, 0x00},
	{0x5988, 0x02},
	{0x598e, 0x03},
	{0x598f, 0x3c},
	{0x3235, 0x00},
	{0x3236, 0x00},
	{0x3802, 0x01},
	{0x3361, 0x00},
	{0x0100, 0x01},

	{0x4418, 0x08},
	{0x4419, 0x8e},
};
*/

/* 640x480 RAW 120fps 24MHz */
static struct regval_list sensor_VGA_120fps_2lane_regs[] = {
	{0x0103, 0x01},
	{0x0100, 0x00},

	{0x3000, 0x00},
	{0x3001, 0x00},
	{0x300f, 0x0f},
	{0x3018, 0x33},
	{0x3019, 0x0c},
	{0x301c, 0x78},
	{0x3031, 0x0a},
	{0x3037, 0x20},
	{0x303f, 0x01},
	{0x320c, 0x03},/* hts=878 */
	{0x320d, 0x6e},
	/* 120fps */
	{0x320e, 0x02},/* vts=683 */
	{0x320f, 0xab},
	{0x3252, 0x02},
	{0x3253, 0xa6},

	{0x3220, 0x10},
	{0x3250, 0xc0},
	{0x3251, 0x02},
	{0x3254, 0x02},
	{0x3255, 0x07},
	{0x3304, 0x48},
	{0x3306, 0x38},
	{0x3309, 0x68},
	{0x330b, 0xe0},
	{0x330c, 0x18},
	{0x330f, 0x20},
	{0x3310, 0x10},
	{0x3314, 0x1e},
	{0x3315, 0x38},
	{0x3316, 0x40},
	{0x3317, 0x10},
	{0x3329, 0x34},
	{0x332d, 0x34},
	{0x332f, 0x38},
	{0x3335, 0x3c},
	{0x3344, 0x3c},
	{0x335b, 0x80},
	{0x335f, 0x80},
	{0x3366, 0x06},
	{0x3385, 0x31},
	{0x3387, 0x51},
	{0x3389, 0x01},
	{0x33b1, 0x03},
	{0x33b2, 0x06},
	{0x3621, 0xa4},
	{0x3622, 0x05},
	{0x3624, 0x47},
	{0x3630, 0x46},
	{0x3631, 0x48},
	{0x3633, 0x52},
	{0x3635, 0x18},
	{0x3636, 0x25},
	{0x3637, 0x89},
	{0x3638, 0x0f},
	{0x3639, 0x08},
	{0x363a, 0x00},
	{0x363b, 0x48},
	{0x363c, 0x06},
	{0x363d, 0x00},
	{0x363e, 0xf8},
	{0x3640, 0x00},
	{0x3641, 0x01},
	{0x36e9, 0x00},
	{0x36ea, 0x3b},
	{0x36eb, 0x0e},
	{0x36ec, 0x1e},
	{0x36ed, 0x33},
	{0x36f9, 0x00},
	{0x36fa, 0x3a},
	{0x36fc, 0x01},
	{0x3908, 0x91},
	{0x3d08, 0x01},
	{0x3e01, 0x14},
	{0x3e02, 0x80},
	{0x3e06, 0x0c},
	{0x4418, 0x08},
	{0x4419, 0x8e},
	{0x4500, 0x59},
	{0x4501, 0xc4},
	{0x4603, 0x00},
	{0x4809, 0x01},
	{0x4837, 0x37},
	{0x5011, 0x00},

	{0x0100, 0x01},
	/* edlay 10ms */
	{0x4418, 0x08},
	{0x4419, 0x8e},

};

/*
 * Here we'll try to encapsulate the changes for just the output
 * video format.
 *
 */
static struct regval_list sensor_fmt_raw[] = {

};

/*
 * Code for dealing with controls.
 * fill with different sensor module
 * different sensor module has different settings here
 * if not support the follow function ,retrun -EINVAL
 */

static int sensor_g_exp(struct v4l2_subdev *sd, __s32 *value)
{
	struct sensor_info *info = to_state(sd);

	*value = info->exp;
	sensor_dbg("sensor_get_exposure = %d\n", info->exp);

	return 0;
}

static int sc031gs_sensor_vts;
static int sensor_s_exp(struct v4l2_subdev *sd, unsigned int exp_val)
{
	data_type explow, exphigh;
	struct sensor_info *info = to_state(sd);
	/* struct vin_md *vind = dev_get_drvdata(sd->v4l2_dev->dev); */
	/* struct vin_core *vinc = vind->vinc[0]; */

	if (exp_val > ((sc031gs_sensor_vts - 6) << 4))
		exp_val = (sc031gs_sensor_vts - 6) << 4;
	if (exp_val < 16)
		exp_val = 16;

	exphigh = (unsigned char)(exp_val >> 8);
	explow = (unsigned char)(exp_val & 0xFF);
	sensor_write(sd, 0x3e01, exphigh);
	sensor_write(sd, 0x3e02, explow);
	sensor_dbg("sensor_s_exp info->exp %d\n", exp_val);
	info->exp = exp_val;

	return 0;
}

static int sensor_g_gain(struct v4l2_subdev *sd, __s32 *value)
{
	struct sensor_info *info = to_state(sd);

	*value = info->gain;
	sensor_dbg("sensor_get_gain = %d\n", info->gain);

	return 0;
}

static int sensor_s_gain(struct v4l2_subdev *sd, unsigned int gain_val)
{
	struct sensor_info *info = to_state(sd);

	if (gain_val < 1 * 16)
		gain_val = 16;
	if (gain_val > 16 * 16 - 1)
		gain_val = 16 * 16 - 1;

	if (gain_val < 32) {
		sensor_write(sd, 0x3314, 0x1e);
		sensor_write(sd, 0x3317, 0x10);
	} else {
		sensor_write(sd, 0x3314, 0x4f);
		sensor_write(sd, 0x3317, 0x0f);
	}

	if (gain_val < 32) {
		sensor_write(sd, 0x3e08, 0x03);
		sensor_write(sd, 0x3e09, gain_val);
	} else if (gain_val >= 32 && gain_val < 64) {
		sensor_write(sd, 0x3e08, 0x07);
		sensor_write(sd, 0x3e09, gain_val >> 1);
	} else if (gain_val >= 64 && gain_val < 128) {
		sensor_write(sd, 0x3e08, 0x0f);
		sensor_write(sd, 0x3e09, gain_val >> 2);
	} else if (gain_val >= 128) {
		sensor_write(sd, 0x3e08, 0x1f);
		sensor_write(sd, 0x3e09, gain_val >> 3);
	}

	sensor_dbg("sensor_s_gain info->gain %d\n", gain_val);
	info->gain = gain_val;

	return 0;
}

static int sensor_s_exp_gain(struct v4l2_subdev *sd,
			     struct sensor_exp_gain *exp_gain)
{
	sensor_s_exp(sd, exp_gain->exp_val);
	sensor_s_gain(sd, exp_gain->gain_val);

	return 0;
}

static int sensor_get_fmt_mbus_core(struct v4l2_subdev *sd, int *code)
{
	*code = MEDIA_BUS_FMT_SBGGR10_1X10;
	return 0;
}

static int sensor_s_hflip(struct v4l2_subdev *sd, int enable)
{
	data_type get_value;
	data_type set_value;

	sensor_dbg("into set sensor hfilp the value:%d\n", enable);
	if (!(enable == 0 || enable == 1))
		return -1;

	sensor_read(sd, 0x3221, &get_value);
	if (enable)
		set_value = get_value | 0x06;
	else
		set_value = get_value & 0xf9;
	sensor_write(sd, 0x3221, set_value);

	return 0;
}

static int sensor_s_vflip(struct v4l2_subdev *sd, int enable)
{
	data_type get_value;
	data_type set_value;

	sensor_dbg("into set sensor hfilp the value:%d\n", enable);
	if (!(enable == 0 || enable == 1))
		return -1;

	sensor_read(sd, 0x3221, &get_value);
	if (enable)
		set_value = get_value | 0x60;
	else
		set_value = get_value & 0x9f;
	sensor_write(sd, 0x3221, set_value);

	return 0;
}

static int sensor_s_sw_stby(struct v4l2_subdev *sd, int on_off)
{
	int ret;
	data_type rdval;

	ret = sensor_read(sd, 0x0100, &rdval);
	if (ret != 0)
		return ret;

	if (on_off == STBY_ON)
		ret = sensor_write(sd, 0x0100, rdval & 0xfe);
	else
		ret = sensor_write(sd, 0x0100, rdval | 0x01);

	return ret;
}

/*
 * Stuff that knows about the sensor.
 */
static int sensor_power(struct v4l2_subdev *sd, int on)
{
	int ret;

	ret = 0;
	switch (on) {
	case STBY_ON:
		ret = sensor_s_sw_stby(sd, STBY_ON);
		if (ret < 0)
			sensor_err("soft stby falied!\n");
		usleep_range(10000, 12000);
		cci_lock(sd);
		/* inactive mclk after stadby in */
		vin_set_mclk(sd, OFF);
		cci_unlock(sd);
		break;
	case STBY_OFF:
		cci_lock(sd);
		vin_set_mclk_freq(sd, MCLK);
		vin_set_mclk(sd, ON);
		usleep_range(10000, 12000);
		cci_unlock(sd);
		ret = sensor_s_sw_stby(sd, STBY_OFF);
		if (ret < 0)
			sensor_err("soft stby off falied!\n");
		usleep_range(10000, 12000);
		break;
	case PWR_ON:
		sensor_print("PWR_ON!\n");
		cci_lock(sd);
		vin_gpio_set_status(sd, RESET, 1);
		vin_gpio_write(sd, RESET, CSI_GPIO_LOW);
		usleep_range(1000, 1200);
		vin_set_pmu_channel(sd, CAMERAVDD, ON);
		vin_set_pmu_channel(sd, IOVDD, ON);
		usleep_range(1000, 1200);
		vin_set_pmu_channel(sd, DVDD, ON);
		usleep_range(1000, 1200);
		vin_set_pmu_channel(sd, AVDD, ON);
		usleep_range(1000, 1200);
		vin_set_mclk_freq(sd, MCLK);
		vin_set_mclk(sd, ON);
		usleep_range(1000, 1200);
		vin_gpio_write(sd, RESET, CSI_GPIO_HIGH);
		usleep_range(10000, 12000);
		cci_unlock(sd);
		break;
	case PWR_OFF:
		sensor_print("PWR_OFF!\n");
		cci_lock(sd);
		vin_gpio_write(sd, RESET, CSI_GPIO_LOW);
		vin_set_mclk(sd, OFF);
		vin_set_pmu_channel(sd, CAMERAVDD, OFF);
		vin_set_pmu_channel(sd, AVDD, OFF);
		vin_set_pmu_channel(sd, DVDD, OFF);
		vin_set_pmu_channel(sd, IOVDD, OFF);
		vin_gpio_set_status(sd, RESET, 0);
		cci_unlock(sd);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int sensor_reset(struct v4l2_subdev *sd, u32 val)
{
	sensor_print("%s val %d\n", __func__, val);

	switch (val) {
	case 0:
		vin_gpio_write(sd, RESET, CSI_GPIO_HIGH);
		usleep_range(10000, 12000);
		break;
	case 1:
		vin_gpio_write(sd, RESET, CSI_GPIO_LOW);
		usleep_range(10000, 12000);
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static int sensor_detect(struct v4l2_subdev *sd)
{
	unsigned int SENSOR_ID = 0;
	data_type rdval;
	int cnt = 0;

	sensor_read(sd, 0x3107, &rdval);
	SENSOR_ID |= (rdval << 8);
	sensor_read(sd, 0x3108, &rdval);
	SENSOR_ID |= (rdval);
	sensor_print("V4L2_IDENT_SENSOR = 0x%x\n", SENSOR_ID);

	while ((SENSOR_ID != V4L2_IDENT_SENSOR) && (cnt < 5)) {
		sensor_read(sd, 0x3107, &rdval);
		SENSOR_ID |= (rdval << 8);
		sensor_read(sd, 0x3108, &rdval);
		SENSOR_ID |= (rdval);
		sensor_print("retry = %d, V4L2_IDENT_SENSOR = %x\n", cnt, SENSOR_ID);
		cnt++;
		}
	if (SENSOR_ID != V4L2_IDENT_SENSOR)
		return -ENODEV;

	return 0;
}

static int sensor_init(struct v4l2_subdev *sd, u32 val)
{
	int ret;
	struct sensor_info *info = to_state(sd);

	sensor_dbg("sensor_init\n");

	/* Make sure it is a target sensor */
	ret = sensor_detect(sd);
	if (ret) {
		sensor_err("chip found is not an target chip.\n");
		return ret;
	}

	info->focus_status = 0;
	info->low_speed = 0;
	info->width = VGA_WIDTH;
	info->height = VGA_HEIGHT;
	info->hflip = 0;
	info->vflip = 0;
	info->exp = 0;
	info->gain = 0;
	info->tpf.numerator = 1;
	info->tpf.denominator = SENSOR_FRAME_RATE;
	info->preview_first_flag = 1;

	return 0;
}

static long sensor_ioctl(struct v4l2_subdev *sd, unsigned int cmd, void *arg)
{
	int ret = 0;
	struct sensor_info *info = to_state(sd);

	switch (cmd) {
	case GET_CURRENT_WIN_CFG:
		if (info->current_wins != NULL) {
			memcpy(arg, info->current_wins,
			       sizeof(*info->current_wins));
			ret = 0;
		} else {
			sensor_err("empty wins!\n");
			ret = -1;
		}
		break;
	case SET_FPS:
		ret = 0;
		break;
	case VIDIOC_VIN_SENSOR_EXP_GAIN:
		ret = sensor_s_exp_gain(sd, (struct sensor_exp_gain *)arg);
		break;
	case VIDIOC_VIN_SENSOR_CFG_REQ:
		sensor_cfg_req(sd, (struct sensor_config *)arg);
		break;
	case VIDIOC_VIN_GET_SENSOR_CODE:
		sensor_get_fmt_mbus_core(sd, (int *)arg);
		break;
	default:
		return -EINVAL;
	}
	return ret;
}

/*
 * Store information about the video data format.
 */
static struct sensor_format_struct sensor_formats[] = {
	{
		.desc = "Raw RGB Bayer",
		.mbus_code = MEDIA_BUS_FMT_SBGGR10_1X10,
		.regs = sensor_fmt_raw,
		.regs_size = ARRAY_SIZE(sensor_fmt_raw),
		.bpp = 1
	},
};
#define N_FMTS ARRAY_SIZE(sensor_formats)

/*
 * Then there is the issue of window sizes.  Try to capture the info here.
 */
static struct sensor_win_size sensor_win_sizes[] = {
#if (MIPI_LANE_NUM == 1)
/*
	{
		.width      = VGA_WIDTH,
		.height     = VGA_HEIGHT,
		.hoffset    = 0,
		.voffset    = 0,
		.hts        = 878,
		.vts        = 2732,
		.pclk       = 72 * 1000 * 1000,
		.mipi_bps   = 720 * 1000 * 1000,
		.fps_fixed  = 30,
		.bin_factor = 1,
		.intg_min   = 1 << 4,
		.intg_max   = (2732 - 6) << 4,
		.gain_min   = 1 << 4,
		.gain_max   = 1440 << 4,
		.regs       = sensor_VGA_30fps_1lane_regs,
		.regs_size  = ARRAY_SIZE(sensor_VGA_30fps_1lane_regs),
		.set_size   = NULL,
	},

	{
		.width      = VGA_WIDTH,
		.height     = VGA_HEIGHT,
		.hoffset    = 0,
		.voffset    = 0,
		.hts        = 878,
		.vts        = 683,
		.pclk       = 72 * 1000 * 1000,
		.mipi_bps   = 720 * 1000 * 1000,
		.fps_fixed  = 120,
		.bin_factor = 1,
		.intg_min   = 1 << 4,
		.intg_max   = (683 - 6) << 4,
		.gain_min   = 1 << 4,
		.gain_max   = 1440 << 4,
		.regs       = sensor_VGA_120fps_1lane_regs,
		.regs_size  = ARRAY_SIZE(sensor_VGA_120fps_1lane_regs),
		.set_size   = NULL,
	},
*/
#endif

#if (MIPI_LANE_NUM == 2)
	{
		.width      = VGA_WIDTH,
		.height     = VGA_HEIGHT,
		.hoffset    = 0,
		.voffset    = 0,
		.hts	    = 878,
		.vts        = 683,
		.pclk       = 72 * 1000 * 1000,
		.mipi_bps   = 360 * 1000 * 1000,
		.fps_fixed  = 120,
		.bin_factor = 1,
		.intg_min   = 1 << 4,
		.intg_max   = (683 - 6) << 4,
		.gain_min   = 1 << 4,
		.gain_max   = 1440 << 4,
		.regs       = sensor_VGA_120fps_2lane_regs,
		.regs_size  = ARRAY_SIZE(sensor_VGA_120fps_2lane_regs),
		.set_size   = NULL,
	},
#endif
};
#define N_WIN_SIZES (ARRAY_SIZE(sensor_win_sizes))

static int sensor_g_mbus_config(struct v4l2_subdev *sd,
				struct v4l2_mbus_config *cfg)
{
	cfg->type = V4L2_MBUS_CSI2_DPHY;
#if (MIPI_LANE_NUM == 1)
	cfg->flags = 0 | V4L2_MBUS_CSI2_1_LANE | V4L2_MBUS_CSI2_CHANNEL_0;
#endif
#if (MIPI_LANE_NUM == 2)
	cfg->flags = 0 | V4L2_MBUS_CSI2_2_LANE | V4L2_MBUS_CSI2_CHANNEL_0;
#endif

	return 0;
}

static int sensor_g_ctrl(struct v4l2_ctrl *ctrl)
{
	struct sensor_info *info =
		container_of(ctrl->handler, struct sensor_info, handler);
	struct v4l2_subdev *sd = &info->sd;

	switch (ctrl->id) {
	case V4L2_CID_GAIN:
		return sensor_g_gain(sd, &ctrl->val);
	case V4L2_CID_EXPOSURE:
		return sensor_g_exp(sd, &ctrl->val);
	}
	return -EINVAL;
}

static int sensor_s_ctrl(struct v4l2_ctrl *ctrl)
{
	struct sensor_info *info =
		container_of(ctrl->handler, struct sensor_info, handler);
	struct v4l2_subdev *sd = &info->sd;

	switch (ctrl->id) {
	case V4L2_CID_GAIN:
		return sensor_s_gain(sd, ctrl->val);
	case V4L2_CID_EXPOSURE:
		return sensor_s_exp(sd, ctrl->val);
	case V4L2_CID_HFLIP:
		return sensor_s_hflip(sd, ctrl->val);
	case V4L2_CID_VFLIP:
		return sensor_s_vflip(sd, ctrl->val);
	}
	return -EINVAL;
}

static int sensor_reg_init(struct sensor_info *info)
{
	int ret;
	struct v4l2_subdev *sd = &info->sd;
	struct sensor_format_struct *sensor_fmt = info->fmt;
	struct sensor_win_size *wsize = info->current_wins;

	ret = sensor_write_array(sd, sensor_default_regs,
				 ARRAY_SIZE(sensor_default_regs));
	if (ret < 0) {
		sensor_err("write sensor_default_regs error\n");
		return ret;
	}

	sensor_dbg("sensor_reg_init\n");
	sensor_write_array(sd, sensor_fmt->regs, sensor_fmt->regs_size);

	if (wsize->regs)
		sensor_write_array(sd, wsize->regs, wsize->regs_size);

	if (wsize->set_size)
		wsize->set_size(sd);

	info->width = wsize->width;
	info->height = wsize->height;
	sc031gs_sensor_vts = wsize->vts;
	info->exp = 0;
	info->gain = 0;
	sensor_print("s_fmt set width = %d, height = %d\n", wsize->width,
		     wsize->height);

	return 0;
}

static int sensor_s_stream(struct v4l2_subdev *sd, int enable)
{
	struct sensor_info *info = to_state(sd);

	sensor_print("%s on = %d, %d*%d fps: %d code: %x\n", __func__, enable,
		     info->current_wins->width, info->current_wins->height,
		     info->current_wins->fps_fixed, info->fmt->mbus_code);

	if (!enable) {
		sensor_write(sd, 0x0100, 0x00);
		info->current_wins = NULL;
		return 0;
	}

	return sensor_reg_init(info);
}

/* ----------------------------------------------------------------------- */

static const struct v4l2_ctrl_ops sensor_ctrl_ops = {
	.g_volatile_ctrl = sensor_g_ctrl,
	.s_ctrl = sensor_s_ctrl,
	.try_ctrl = sensor_try_ctrl,
};

static const struct v4l2_subdev_core_ops sensor_core_ops = {
	.reset = sensor_reset,
	.init = sensor_init,
	.s_power = sensor_power,
	.ioctl = sensor_ioctl,
#if IS_ENABLED(CONFIG_COMPAT)
	.compat_ioctl32 = sensor_compat_ioctl32,
#endif
};

static const struct v4l2_subdev_video_ops sensor_video_ops = {
	.s_stream = sensor_s_stream,
	.g_mbus_config = sensor_g_mbus_config,
};

static const struct v4l2_subdev_pad_ops sensor_pad_ops = {
	.enum_mbus_code = sensor_enum_mbus_code,
	.enum_frame_size = sensor_enum_frame_size,
	.enum_frame_interval = sensor_enum_frame_interval,
	.get_fmt = sensor_get_fmt,
	.set_fmt = sensor_set_fmt,
};

static const struct v4l2_subdev_ops sensor_ops = {
	.core = &sensor_core_ops,
	.video = &sensor_video_ops,
	.pad = &sensor_pad_ops,
};

/* ----------------------------------------------------------------------- */
static struct cci_driver cci_drv = {
	.name = SENSOR_NAME,
	.addr_width = CCI_BITS_16,
	.data_width = CCI_BITS_8,
};

static const struct v4l2_ctrl_config sensor_custom_ctrls[] = {
	{
		.ops = &sensor_ctrl_ops,
		.id = V4L2_CID_FRAME_RATE,
		.name = "frame rate",
		.type = V4L2_CTRL_TYPE_INTEGER,
		.min = 15,
		.max = 120,
		.step = 1,
		.def = 120,
	},
};

static int sensor_init_controls(struct v4l2_subdev *sd,
				const struct v4l2_ctrl_ops *ops)
{
	struct sensor_info *info = to_state(sd);
	struct v4l2_ctrl_handler *handler = &info->handler;
	struct v4l2_ctrl *ctrl;
	int ret = 0;

	v4l2_ctrl_handler_init(handler, 4);
	ctrl = v4l2_ctrl_new_std(handler, ops, V4L2_CID_GAIN, 1 * 16, 16 * 16, 1, 16);

	if (ctrl != NULL)
		ctrl->flags |= V4L2_CTRL_FLAG_VOLATILE;
	ctrl = v4l2_ctrl_new_std(handler, ops, V4L2_CID_EXPOSURE,
				 3 * 16, 65536 * 16, 1, 3 * 16);
	v4l2_ctrl_new_std(handler, ops, V4L2_CID_HFLIP, 0, 1, 1, 0);
	v4l2_ctrl_new_std(handler, ops, V4L2_CID_VFLIP, 0, 1, 1, 0);
	if (ctrl != NULL)
		ctrl->flags |= V4L2_CTRL_FLAG_VOLATILE;

	if (handler->error) {
		ret = handler->error;
		v4l2_ctrl_handler_free(handler);
	}

	sd->ctrl_handler = handler;

	return ret;
}

static int sensor_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	struct v4l2_subdev *sd = NULL;
	struct sensor_info *info = NULL;

	info = kzalloc(sizeof(*info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;

	sd = &info->sd;

	cci_dev_probe_helper(sd, client, &sensor_ops, &cci_drv);
	sensor_init_controls(sd, &sensor_ctrl_ops);

	mutex_init(&info->lock);

	info->fmt = &sensor_formats[0];
	info->fmt_pt = &sensor_formats[0];
	info->win_pt = &sensor_win_sizes[0];
	info->fmt_num = N_FMTS;
	info->win_size_num = N_WIN_SIZES;
	info->sensor_field = V4L2_FIELD_NONE;
	info->stream_seq = MIPI_BEFORE_SENSOR;
	info->af_first_flag = 1;
	info->exp = 0;
	info->gain = 0;

	return 0;
}

static int sensor_remove(struct i2c_client *client)
{
	struct v4l2_subdev *sd;

	sd = cci_dev_remove_helper(client, &cci_drv);

	kfree(to_state(sd));
	return 0;
}

static const struct i2c_device_id sensor_id[] = {
	{SENSOR_NAME, 0},
	{}
};

MODULE_DEVICE_TABLE(i2c, sensor_id);

static struct i2c_driver sensor_driver = {
	.driver = {
		.owner = THIS_MODULE,
		.name = SENSOR_NAME,
	},
	.probe = sensor_probe,
	.remove = sensor_remove,
	.id_table = sensor_id,
};
static __init int init_sensor(void)
{
	return cci_dev_init_helper(&sensor_driver);
}

static __exit void exit_sensor(void)
{
	cci_dev_exit_helper(&sensor_driver);
}

VIN_INIT_DRIVERS(init_sensor);
module_exit(exit_sensor);
