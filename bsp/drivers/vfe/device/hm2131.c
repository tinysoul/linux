/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright(c) 2020 - 2023 Allwinner Technology Co.,Ltd. All rights reserved. */
/*
 * A V4L2 driver for HM2131 Raw cameras.
 *
 */
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

MODULE_AUTHOR("xiongbiao");
MODULE_DESCRIPTION("A low-level driver for HM2131 Raw sensors");
MODULE_LICENSE("GPL");

/* for internal driver debug */
#define DEV_DBG_EN      0
#if (DEV_DBG_EN == 1)
#define vfe_dev_dbg(x, arg...) pr_err("[HM2131]"x, ##arg)
#else
#define vfe_dev_dbg(x, arg...)
#endif
#define vfe_dev_err(x, arg...) pr_err("[HM2131]"x, ##arg)
#define vfe_dev_print(x, arg...) pr_info("[HM2131]"x, ##arg)


/* define module timing */
#define MCLK			(24*1000*1000)

#define V4L2_IDENT_SENSOR	0x2131


/*
 * Our nominal (default) frame rate.
 */

#define SENSOR_FRAME_RATE	30


/*
 * The hm2131 i2c address
 */
/* #define I2C_ADDR 0x48 */
#define I2C_ADDR 0x48

/* static struct delayed_work sensor_s_ae_ratio_work; */
static struct v4l2_subdev *glb_sd;

#define SENSOR_NAME "hm2131"

/*
 * Information we maintain about a known sensor.
 */
struct sensor_format_struct;  /* coming later */

struct cfg_array { /* coming later */
	struct regval_list *regs;
	int size;
};

static int LOG_ERR_RET(int x)
{
	int ret;

	ret = x;
	if (ret < 0)
		vfe_dev_err("error at %s\n", __func__);
	return ret;
}

static inline struct sensor_info *to_state(struct v4l2_subdev *sd)
{
	return container_of(sd, struct sensor_info, sd);
}

/*
 * The default register settings
 *
 */

static struct regval_list sensor_1080p_regs_soft_reset[] = {
	{0x0103, 0x00}
};
static struct regval_list sensor_1080p_regs[] = { /* 1080: 1920*1080@30fps */
	{0x0304, 0x2A}, {0x0305, 0x0C}, {0x0307, 0x55},
	{0x0303, 0x04}, {0x0309, 0x00}, {0x030A, 0x0A}, {0x030D, 0x02},
	{0x030F, 0x14}, {0x5268, 0x01}, {0x5264, 0x24}, {0x5265, 0x92},
	{0x5266, 0x23}, {0x5267, 0x07}, {0x5269, 0x02}, {0x0100, 0x02},
	{0x0100, 0x02}, {0x0111, 0x01}, {0x0112, 0x0A}, {0x0113, 0x0A},
	{0x4B20, 0xCE}, {0x4B18, 0x12}, {0x4B02, 0x05}, {0x4B43, 0x07},
	{0x4B05, 0x1C}, {0x4B0E, 0x00}, {0x4B0F, 0x0D}, {0x4B06, 0x06},
	{0x4B39, 0x0B}, {0x4B42, 0x03}, {0x4B03, 0x0C}, {0x4B04, 0x07},
	{0x4B3A, 0x0B}, {0x4B51, 0x80}, {0x4B52, 0x09}, {0x4B52, 0xC9},
	{0x4B57, 0x07}, {0x4B68, 0x6B}, {0x0350, 0x27}, {0x5030, 0x10},
	{0x5032, 0x02}, {0x5033, 0xD1}, {0x5034, 0x01}, {0x5035, 0x67},
	{0x5229, 0x90}, {0x5061, 0x00}, {0x5062, 0x94}, {0x50F5, 0x06},
	{0x5230, 0x00}, {0x526C, 0x00}, {0x5254, 0x08}, {0x522B, 0x00},
	{0x4144, 0x08}, {0x4148, 0x03}, {0x4024, 0x40}, {0x4B66, 0x00},
	{0x0340, 0x04}, {0x0341, 0xD6}, {0x0342, 0x04}, {0x0343, 0x78},
	{0x034C, 0x07}, {0x034D, 0x88}, {0x034E, 0x04}, {0x034F, 0x40},
	{0x0101, 0x00}, {0x4020, 0x10},
#ifdef HII_GAIN
	{0x50DD, 0x01}, {0x0350, 0x37},
#else
	{0x50DD, 0x00}, {0x0350, 0x27},
#endif
	{0x4131, 0x01}, {0x4132, 0x20}, {0x5011, 0x00}, {0x5015, 0x00},
	{0x501D, 0x1C}, {0x501E, 0x00}, {0x501F, 0x20}, {0x50D5, 0xF0},
	{0x50D7, 0x12}, {0x50BB, 0x14}, {0x5040, 0x07}, {0x50B7, 0x00},
	{0x50B8, 0x35}, {0x50B9, 0xFF}, {0x50BA, 0xFF}, {0x5200, 0x26},
	{0x5201, 0x00}, {0x5202, 0x00}, {0x5203, 0x00}, {0x5217, 0x01},
	{0x5234, 0x01}, {0x526B, 0x03}, {0x4C00, 0x00}, {0x4B31, 0x02},
	{0x4B3B, 0x02}, {0x4B44, 0x0C}, {0x4B45, 0x01}, {0x50A1, 0x00},
	{0x50AA, 0x2D}, {0x50AC, 0x11}, {0x50AB, 0x04}, {0x50A0, 0xB0},
	{0x50A2, 0x12}, {0x50AF, 0x00}, {0x5208, 0x55}, {0x5209, 0x0F},
	{0x520D, 0x40}, {0x5214, 0x18}, {0x5215, 0x0B}, {0x5216, 0x00},
	{0x521A, 0x10}, {0x521B, 0x24}, {0x5232, 0x04}, {0x5233, 0x03},
	{0x5106, 0xF0}, {0x510E, 0xF0}, {0x51C0, 0x80}, {0x51C4, 0x80},
	{0x51C8, 0x80}, {0x51CC, 0x80}, {0x51D0, 0x80}, {0x51D4, 0x8F},
	{0x51D8, 0x8F}, {0x51DC, 0x8F}, {0x51C1, 0x03}, {0x51C5, 0x13},
	{0x51C9, 0x17}, {0x51CD, 0x17}, {0x51D1, 0x17}, {0x51D5, 0x1F},
	{0x51D9, 0x1F}, {0x51DD, 0x1F}, {0x51C2, 0x4B}, {0x51C6, 0x4B},
	{0x51CA, 0x4B}, {0x51CE, 0x4B}, {0x51D2, 0x4B}, {0x51D6, 0x3B},
	{0x51DA, 0x3B}, {0x51DE, 0x39}, {0x51C3, 0x10}, {0x51C7, 0x10},
	{0x51CB, 0x10}, {0x51CF, 0x08}, {0x51D3, 0x08}, {0x51D7, 0x08},
	{0x51DB, 0x08}, {0x51DF, 0x00}, {0x5264, 0x23}, {0x5265, 0x07},
	{0x5266, 0x24}, {0x5267, 0x92}, {0x5268, 0x01}, {0xBAA2, 0xC0},
	{0xBAA2, 0x40}, {0xBA90, 0x01}, {0xBA93, 0x02}, {0x3110, 0x0B},
	{0x373E, 0x8A}, {0x373F, 0x8A}, {0x3713, 0x00}, {0x3717, 0x00},
	{0x5043, 0x01}, {0x0202, 0x03}, {0x0203, 0xff}, {0x0205, 0xe0},
	{0x0000, 0x00}, {0x0104, 0x01}, {0x0104, 0x00}, {0x0100, 0x03},
};

/*
 * Here we'll try to encapsulate the changes for just the output
 * video format.
 *
 */

static struct regval_list sensor_fmt_raw[] = {
};


static int sensor_g_exp(struct v4l2_subdev *sd, __s32 *value)
{
	struct sensor_info *info = to_state(sd);

	*value = info->exp;
	vfe_dev_dbg("sensor_get_exposure = %d\n", info->exp);
	return 0;
}

static int sensor_s_exp(struct v4l2_subdev *sd, unsigned int exp_val)
{
	unsigned char explow, expmid;
	struct sensor_info *info = to_state(sd);

	vfe_dev_dbg("sensor_set_exposure = %d\n", exp_val >> 4);
	if (exp_val > 0xfffff)
		exp_val = 0xfffff;

	expmid = (unsigned char)((0x0ff000 & exp_val) >> 12);
	explow = (unsigned char)((0x000ff0 & exp_val) >> 4);

	sensor_write(sd, 0x0104, 0x01);
	sensor_write(sd, 0x0202, expmid);
	sensor_write(sd, 0x0203, explow);
	sensor_write(sd, 0x0104, 0x00);

	info->exp = exp_val;
	return 0;
}


static int sensor_g_gain(struct v4l2_subdev *sd, __s32 *value)
{
	struct sensor_info *info = to_state(sd);

	*value = info->gain;
	vfe_dev_dbg("sensor_get_gain = %d\n", info->gain);

	return 0;
}

static int sensor_s_gain(struct v4l2_subdev *sd, int gain_val)
{
	struct sensor_info *info = to_state(sd);
	unsigned char gainlow = 0;

	if (gain_val < 1 * 16)
		gain_val = 16;
	if (gain_val > 64 * 16 - 1)
		gain_val = 64 * 16 - 1;
	vfe_dev_dbg("sensor_set_gain = %d\n", gain_val);

	gainlow = (unsigned char)(gain_val - 16);

	sensor_write(sd, 0x0104, 0x01);
	sensor_write(sd, 0x0205, gainlow);
	sensor_write(sd, 0x0104, 0x00);

	vfe_dev_dbg("2131 sensor_set_gain = %d, 0x%x, Done!\n",
						gain_val, gainlow);
	info->gain = gain_val;

	return 0;
}

static int hm2131_sensor_vts;
static int sensor_s_exp_gain(struct v4l2_subdev *sd,
					struct sensor_exp_gain *exp_gain)
{
	int exp_val, gain_val, frame_length, shutter;
	unsigned char explow = 0, expmid = 0;
	unsigned char gainlow = 0;
	struct sensor_info *info = to_state(sd);

	exp_val = exp_gain->exp_val;
	gain_val = exp_gain->gain_val;

	if ((info->exp == exp_val) && (info->gain == gain_val))
		return 0;

	if (gain_val < 1 * 16)
		gain_val = 16;
	if (gain_val > 64 * 16 - 1)
		gain_val = 64 * 16 - 1;

	if (exp_val > 0xfffff)
		exp_val = 0xfffff;

	shutter = exp_val / 16;
	if (shutter > hm2131_sensor_vts - 4)
		frame_length = shutter + 4;
	else
		frame_length = hm2131_sensor_vts;

	gainlow = (unsigned char)(gain_val - 16);
	expmid = (unsigned char)((0x0ff000 & exp_val) >> 12);
	explow = (unsigned char)((0x000ff0 & exp_val) >> 4);

	sensor_write(sd, 0x0104, 0x01);
	sensor_write(sd, 0x0202, expmid);
	sensor_write(sd, 0x0203, explow);
	sensor_write(sd, 0x0205, gainlow);
	sensor_write(sd, 0x0104, 0x00);

	info->exp = exp_val;
	info->gain = gain_val;
	return 0;
}

static int sensor_s_sw_stby(struct v4l2_subdev *sd, int on_off)
{
	int ret;
	unsigned short rdval;

	ret = sensor_read(sd, 0x0100, &rdval);
	if (ret != 0)
		return ret;

	if (on_off == CSI_GPIO_LOW)
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
	case CSI_SUBDEV_STBY_ON:
		vfe_dev_dbg("CSI_SUBDEV_STBY_ON!\n");
		ret = sensor_s_sw_stby(sd, CSI_GPIO_LOW);
		if (ret < 0)
			vfe_dev_err("soft stby falied!\n");
		usleep_range(10000, 12000);
		cci_lock(sd);
		vfe_gpio_write(sd, RESET, CSI_GPIO_LOW);
		cci_unlock(sd);
		vfe_set_mclk(sd, OFF);
		break;
	case CSI_SUBDEV_STBY_OFF:
		vfe_dev_dbg("CSI_SUBDEV_STBY_OFF!\n");
		cci_lock(sd);
		vfe_set_mclk_freq(sd, MCLK);
		vfe_set_mclk(sd, ON);
		usleep_range(10000, 12000);
		vfe_gpio_write(sd, RESET, CSI_GPIO_HIGH);
		usleep_range(10000, 12000);
		cci_unlock(sd);
		ret = sensor_s_sw_stby(sd, CSI_GPIO_HIGH);
		if (ret < 0)
			vfe_dev_err("soft stby off falied!\n");
		usleep_range(10000, 12000);
		break;
	case CSI_SUBDEV_PWR_ON:
		vfe_dev_dbg("CSI_SUBDEV_PWR_ON!\n");
		cci_lock(sd);
		vfe_gpio_set_status(sd, PWDN, 1);
		vfe_gpio_set_status(sd, RESET, 1);
		vfe_gpio_write(sd, PWDN, CSI_GPIO_LOW);
		vfe_gpio_write(sd, RESET, CSI_GPIO_LOW);
		usleep_range(1000, 1200);

		vfe_set_mclk_freq(sd, MCLK);
		vfe_set_mclk(sd, ON);

		vfe_gpio_write(sd, POWER_EN, CSI_GPIO_HIGH);
		vfe_set_pmu_channel(sd, IOVDD, ON);
		vfe_set_pmu_channel(sd, AVDD, ON);
		vfe_set_pmu_channel(sd, DVDD, ON);
		vfe_set_pmu_channel(sd, AFVDD, ON);

		vfe_gpio_write(sd, PWDN, CSI_GPIO_HIGH);
		usleep_range(10000, 12000);
		vfe_gpio_write(sd, RESET, CSI_GPIO_HIGH);
		usleep_range(30000, 31000);

		cci_unlock(sd);

		break;
	case CSI_SUBDEV_PWR_OFF:

		vfe_dev_dbg("CSI_SUBDEV_PWR_OFF!\n");
		cci_lock(sd);

		vfe_set_mclk(sd, OFF);

		vfe_gpio_write(sd, POWER_EN, CSI_GPIO_LOW);
		vfe_set_pmu_channel(sd, AFVDD, OFF);
		vfe_set_pmu_channel(sd, DVDD, OFF);
		vfe_set_pmu_channel(sd, AVDD, OFF);
		vfe_set_pmu_channel(sd, IOVDD, OFF);

		usleep_range(10000, 12000);
		vfe_gpio_write(sd, POWER_EN, CSI_GPIO_LOW);
		vfe_gpio_write(sd, RESET, CSI_GPIO_LOW);
		vfe_gpio_write(sd, PWDN, CSI_GPIO_LOW);

		vfe_gpio_set_status(sd, RESET, 0);
		vfe_gpio_set_status(sd, PWDN, 0);

		cci_unlock(sd);

		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int sensor_reset(struct v4l2_subdev *sd, u32 val)
{
	switch (val) {
	case 0:
		vfe_dev_dbg("CSI_GPIO_HIGH!\n");
		vfe_gpio_write(sd, RESET, CSI_GPIO_HIGH);
		usleep_range(10000, 12000);
		break;
	case 1:
		vfe_dev_dbg("CSI_GPIO_LOW!\n");
		vfe_gpio_write(sd, RESET, CSI_GPIO_LOW);
		usleep_range(10000, 12000);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int sensor_detect(struct v4l2_subdev *sd)
{
	unsigned short rdval;

	LOG_ERR_RET(sensor_read(sd, 0x0000, &rdval));
	if (rdval != 0x21)
		return -ENODEV;

	LOG_ERR_RET(sensor_read(sd, 0x0001, &rdval));
	if (rdval != 0x31)
		return -ENODEV;

	return 0;
}

static int sensor_init(struct v4l2_subdev *sd, u32 val)
{
	int ret;
	struct sensor_info *info = to_state(sd);

	vfe_dev_dbg("sensor_init\n");

	/*Make sure it is a target sensor*/
	ret = sensor_detect(sd);
	if (ret) {
		vfe_dev_err("chip found is not an target chip.\n");
		return ret;
	}

	vfe_get_standby_mode(sd, &info->stby_mode);

	if ((info->stby_mode == HW_STBY || info->stby_mode == SW_STBY)
			&& info->init_first_flag == 0) {
		vfe_dev_print("stby_mode and init_first_flag = 0\n");
		return 0;
	}

	info->focus_status = 0;
	info->low_speed = 0;
	info->width = HD1080_WIDTH;
	info->height = HD1080_HEIGHT;
	info->hflip = 0;
	info->vflip = 0;
	info->gain = 0;

	info->tpf.numerator = 1;
	info->tpf.denominator = 30;    /* 30fps */

	sensor_write_array(sd, sensor_1080p_regs_soft_reset,
				 ARRAY_SIZE(sensor_1080p_regs_soft_reset));
	msleep(100);
	ret =	sensor_write_array(sd, sensor_1080p_regs,
					 ARRAY_SIZE(sensor_1080p_regs));
	if (ret < 0) {
		vfe_dev_err("write sensor_default_regs error\n");
		return ret;
	}

	if (info->stby_mode == 0)
		info->init_first_flag = 0;

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
					sizeof(struct sensor_win_size));
			ret = 0;
		} else {
			vfe_dev_err("empty wins!\n");
			ret =  -1;
		}
		break;
	case SET_FPS:
		break;
	case ISP_SET_EXP_GAIN:
		ret = sensor_s_exp_gain(sd, (struct sensor_exp_gain *)arg);
		break;
	default:
		return -EINVAL;
	}

	return ret;
}


/*
 * Store information about the video data format.
 */
static struct sensor_format_struct {
	__u8 *desc;
	/* __u32 pixelformat; */
	u32 mbus_code;
	struct regval_list *regs;
	int regs_size;
	int bpp;   /* Bytes per pixel */
} sensor_formats[] = {
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
	/* 1080P */
	{
	 .width = HD1080_WIDTH,
	 .height = HD1080_HEIGHT,
	 .hoffset = 0,
	 .voffset = 0,
	 .hts = 1144,
	 .vts = 1238,
	 .pclk = 42500000,
	 .mipi_bps = 720 * 1000 * 1000,
	 .fps_fixed = 30,
	 .bin_factor = 1,
	 .intg_min = 1 << 4,
	 .intg_max = (1238 - 4) << 4,
	 .gain_min = 1 << 4,
	 .gain_max = 15 << 4,
	 .regs = sensor_1080p_regs,
	 .regs_size = ARRAY_SIZE(sensor_1080p_regs),
	 .set_size = NULL,
	 },
};

#define N_WIN_SIZES (ARRAY_SIZE(sensor_win_sizes))

static int sensor_enum_code(struct v4l2_subdev *sd,
				struct v4l2_subdev_pad_config *cfg,
				struct v4l2_subdev_mbus_code_enum *code)
{
	if (code->index >= N_FMTS)
		return -EINVAL;

	code->code = sensor_formats[code->index].mbus_code;

	return 0;
}

static int sensor_enum_frame_size(struct v4l2_subdev *sd,
				struct v4l2_subdev_pad_config *cfg,
				struct v4l2_subdev_frame_size_enum *fse)
{
	if (fse->index > N_WIN_SIZES-1)
		return -EINVAL;

	fse->min_width = sensor_win_sizes[fse->index].width;
	fse->max_width = fse->min_width;
	fse->max_height = sensor_win_sizes[fse->index].height;
	fse->max_height = fse->min_height;

	return 0;
}

static int sensor_try_fmt_internal(struct v4l2_subdev *sd,
					struct v4l2_mbus_framefmt *fmt,
					struct sensor_format_struct **ret_fmt,
					struct sensor_win_size **ret_wsize)
{
	int index;
	struct sensor_win_size *wsize;
	struct sensor_info *info = to_state(sd);

	for (index = 0; index < N_FMTS; index++)
		if (sensor_formats[index].mbus_code == fmt->code)
			break;

	if (index >= N_FMTS)
		return -EINVAL;

	if (ret_fmt != NULL)
		*ret_fmt = sensor_formats + index;
	/*
	 * Fields: the sensor devices claim to be progressive.
	 */
	fmt->field = V4L2_FIELD_NONE;

	/*
	 * Round requested image size down to the nearest
	 * we support, but not below the smallest.
	 */
	for (wsize = sensor_win_sizes; wsize < sensor_win_sizes + N_WIN_SIZES;
									wsize++)
		if (fmt->width >= wsize->width && fmt->height >= wsize->height)
			break;

	if (wsize >= sensor_win_sizes + N_WIN_SIZES)
		wsize--;   /* Take the smallest one */
	if (ret_wsize != NULL)
		*ret_wsize = wsize;
	/*
	 * Note the size we'll actually handle.
	 */
	fmt->width = wsize->width;
	fmt->height = wsize->height;
	info->current_wins = wsize;

	return 0;
}
static int sensor_get_fmt(struct v4l2_subdev *sd,
			struct v4l2_subdev_pad_config *cfg,
			struct v4l2_subdev_format *fmat)
{
	struct v4l2_mbus_framefmt *fmt = &fmat->format;

	return sensor_try_fmt_internal(sd, fmt, NULL, NULL);
}

static int sensor_g_mbus_config(struct v4l2_subdev *sd,
	   struct v4l2_mbus_config *cfg)
{
	cfg->type = V4L2_MBUS_CSI2;
	cfg->flags = 0 | V4L2_MBUS_CSI2_2_LANE | V4L2_MBUS_CSI2_CHANNEL_0;

	return 0;
}


/*
 * Set a format.
 */
static int sensor_set_fmt(struct v4l2_subdev *sd,
		struct v4l2_subdev_pad_config *cfg,
		struct v4l2_subdev_format *fmat)
{
	int ret;
	struct v4l2_mbus_framefmt *fmt = &fmat->format;
	struct sensor_format_struct *sensor_fmt;
	struct sensor_win_size *wsize;
	struct sensor_info *info = to_state(sd);

	vfe_dev_dbg("sensor_set_fmt\n");

	ret = sensor_try_fmt_internal(sd, fmt, &sensor_fmt, &wsize);
	if (ret)
		return ret;

	LOG_ERR_RET(sensor_write_array(sd,
				sensor_fmt->regs, sensor_fmt->regs_size));

	if (wsize->regs)
		LOG_ERR_RET(sensor_write_array(sd,
				wsize->regs, wsize->regs_size));

	if (wsize->set_size)
		LOG_ERR_RET(wsize->set_size(sd));

	info->fmt = sensor_fmt;
	info->width = wsize->width;
	info->height = wsize->height;
	info->exp = 0;
	info->gain = 0;
	hm2131_sensor_vts = wsize->vts;

	vfe_dev_print("s_fmt set width = %d, height = %d\n",
						wsize->width, wsize->height);

	return 0;
}

/*
 * Implement G/S_PARM.  There is a "high quality" mode we could try
 * to do someday; for now, we just do the frame rate tweak.
 */
static int sensor_g_parm(struct v4l2_subdev *sd, struct v4l2_streamparm *parms)
{
	struct v4l2_captureparm *cp = &parms->parm.capture;
	struct sensor_info *info = to_state(sd);

	if (parms->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;

	memset(cp, 0, sizeof(struct v4l2_captureparm));
	cp->capability = V4L2_CAP_TIMEPERFRAME;
	cp->capturemode = info->capture_mode;

	return 0;
}

static int sensor_s_parm(struct v4l2_subdev *sd, struct v4l2_streamparm *parms)
{
	struct v4l2_captureparm *cp = &parms->parm.capture;
	struct sensor_info *info = to_state(sd);

	vfe_dev_dbg("sensor_s_parm\n");

	if (parms->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
		return -EINVAL;

	if (info->tpf.numerator == 0)
		return -EINVAL;

	info->capture_mode = cp->capturemode;

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
	}

	return -EINVAL;
}

/* ---------------------------------------------------------------- */

static const struct v4l2_ctrl_ops sensor_ctrl_ops = {
	.g_volatile_ctrl = sensor_g_ctrl,
	.s_ctrl = sensor_s_ctrl,
};

static const struct v4l2_subdev_core_ops sensor_core_ops = {
	.reset = sensor_reset,
	.init = sensor_init,
	.s_power = sensor_power,
	.ioctl = sensor_ioctl,
};

static const struct v4l2_subdev_video_ops sensor_video_ops = {
	.s_parm = sensor_s_parm,
	.g_parm = sensor_g_parm,
	.g_mbus_config = sensor_g_mbus_config,
};

static const struct v4l2_subdev_pad_ops sensor_pad_ops = {
	.enum_mbus_code = sensor_enum_code,
	.enum_frame_size = sensor_enum_frame_size,
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
	int i;

	v4l2_ctrl_handler_init(handler, 2 +  ARRAY_SIZE(sensor_custom_ctrls));

	ctrl = v4l2_ctrl_new_std(handler, ops,
					V4L2_CID_GAIN, 1*16, 32*16, 1, 1*16);
	if (ctrl != NULL)
		ctrl->flags |= V4L2_CTRL_FLAG_VOLATILE;

	ctrl = v4l2_ctrl_new_std(handler, ops,
					V4L2_CID_EXPOSURE, 0, 65536*16, 1, 0);
	if (ctrl != NULL)
		ctrl->flags |= V4L2_CTRL_FLAG_VOLATILE;

	for (i = 0; i < ARRAY_SIZE(sensor_custom_ctrls); i++)
		v4l2_ctrl_new_custom(handler, &sensor_custom_ctrls[i], NULL);

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
	struct v4l2_subdev *sd;
	struct sensor_info *info;

	vfe_dev_print("hm2131 probe\n");

	info = kzalloc(sizeof(struct sensor_info), GFP_KERNEL);
	if (info == NULL)
		return -ENOMEM;
	sd = &info->sd;
	glb_sd = sd;
	sensor_init_controls(sd, &sensor_ctrl_ops);
	cci_dev_probe_helper(sd, client, &sensor_ops, &cci_drv);

	info->fmt = &sensor_formats[0];
	info->af_first_flag = 1;
	info->init_first_flag = 1;

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
	{SENSOR_NAME, 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, sensor_id);

static const struct of_device_id sernsor_match[] = {
		{ .compatible = "allwinner,sensor_hm2131", },
		{},
};

static struct i2c_driver sensor_driver = {
	.driver = {
		.owner = THIS_MODULE,
		.name = SENSOR_NAME,
		.of_match_table = sernsor_match,
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

module_init(init_sensor);
module_exit(exit_sensor);
