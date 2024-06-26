/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright(c) 2020 - 2023 Allwinner Technology Co.,Ltd. All rights reserved. */
/*
 * linux-4.9/drivers/media/platform/sunxi-vfe/actuator/actuator.h
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
 * sunxi actuator driver
 */

#ifndef __ACTUATOR__H__
#define __ACTUATOR__H__

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <linux/mutex.h>
#include <linux/videodev2.h>
#include <linux/clk.h>
#include <linux/delay.h>

#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>
#include <media/v4l2-common.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <media/v4l2-subdev.h>
/* #include <media/videobuf-dma-contig.h> */
#include <linux/moduleparam.h>
#include "../csi_cci/cci_helper.h"
/* #include <linux/regulator/consumer.h> */

/*
 *POS(STEP)     0----1---2--3...----------------------TOTAL_STEPS_ACTIVE
 *	      |ACTIVE_MIN|                          |ACTIVE_MAX|
 *CODE  0-------|-------------------------------------|--------1023
 */

/* #define ACT_DBG */

/* print when error happens */
#define act_err(x, arg...) pr_err("[ACT_ERR]"x, ##arg)

/* print unconditional, for important info */
#ifdef ACT_DBG
#define act_dbg(x, arg...) pr_debug("[ACT]"x, ##arg)
#else
#define act_dbg(x, arg...)
#endif

struct actuator_ctrl_t;

#define ACT_DEV_MIN_CODE 0   /* normally code range from 0~1023 */
#define ACT_DEV_MAX_CODE 1023/* normally code range from 0~1023 */

#define MOVE_NEAR	0
#define MOVE_FAR	1

typedef enum tag_ACT_SUBDEV_CMD {
	ACT_INIT,
	ACT_PWDN,
	ACT_SOFT_PWDN,

	ACT_RELEASE,

	ACT_SET_CODE_RANGE,
	ACT_GET_CODE_RANGE,
	ACT_SET_CODE,
	ACT_GET_CODE,

	ACT_SET_POS,
	ACT_GET_POS,
	ACT_MOV_POS,

	/* ACT_LD_DOF_TBL      ,  //need para */
	/* ACT_GET_DOF_POS     , */
	/* ACT_SET_DOF_POS     ,  //need para=position in dof table */

	ACT_MOV_NEAR,
	ACT_MOV_FAR,
	ACT_MOV_NEAREST,
	ACT_MOV_FARTHEST,
} __act_subdev_cmd_t;

struct actuator_para_t {
	unsigned short active_min;
	unsigned short active_max;
	unsigned short ext_tbl_en;/* 0-disable 1- en */
	unsigned short ext_tbl_steps;
	unsigned short *ext_tbl;
};

struct actuator_ctrl_word_t {
	unsigned short pwdn;
	unsigned short code;
	unsigned short sr;
};

struct actuator_ctrl_step_t {
	unsigned short step;
	unsigned short dir;
};

struct actuator_set_info_t {
	unsigned int total_steps;
	unsigned short gross_steps;
	unsigned short fine_steps;
};

struct actuator_get_info_t {
	unsigned int focal_length_num;
	unsigned int focal_length_den;
	unsigned int f_number_num;
	unsigned int f_number_den;
	unsigned int f_pix_num;
	unsigned int f_pix_den;
	unsigned int total_f_dist_num;
	unsigned int total_f_dist_den;
	unsigned int hor_view_angle_num;
	unsigned int hor_view_angle_den;
	unsigned int ver_view_angle_num;
	unsigned int ver_view_angle_den;
};

struct actuator_move_params_t {
	char dir;
	int num_steps;
};

struct actuator_cfg_data {
	int cfgtype;
	unsigned char is_af_supported;
	union {
		struct actuator_move_params_t move;
		struct actuator_set_info_t set_info;
		struct actuator_get_info_t get_info;
	} cfg;
};

struct actuator_func_tbl {
	/* long (*actuator_ioctl)(struct v4l2_subdev *, */
	/* unsigned int, */
	/* void *); */
	int (*actuator_init)(struct actuator_ctrl_t *,
			struct actuator_para_t *);
	int (*actuator_pwdn)(struct actuator_ctrl_t *,
			unsigned short);
	int (*actuator_init_table)(struct actuator_ctrl_t *,
			unsigned short,
			unsigned short,
			unsigned short *);
	/* int (*actuator_set_default_focus) (struct actuator_ctrl_t *); */
	int (*actuator_release)(struct actuator_ctrl_t *,
			struct actuator_ctrl_word_t *);
	int (*actuator_move_pos)(struct actuator_ctrl_t *,
			unsigned short,
			unsigned short);
	int (*actuator_set_pos)(struct actuator_ctrl_t *,
			unsigned short);
	int (*actuator_set_code)(struct actuator_ctrl_t *,
			unsigned short,
			unsigned short);
	int (*actuator_i2c_write)(struct actuator_ctrl_t *,
			unsigned short, void *);
};

typedef enum tag_ACT_STA {
	ACT_STA_HW_PWDN,
	ACT_STA_SOFT_PWDN,
	ACT_STA_IDLE,  /* released */
	ACT_STA_BUSY,
	ACT_STA_ERR,
	ACT_STA_SCANNING,
	ACT_STA_HALT,  /* paused to specific pos */
} __act_sta_t;

struct actuator_ctrl_t {
	unsigned int i2c_addr;
	struct i2c_driver *i2c_driver;
	struct i2c_client *i2c_client;
	struct mutex *actuator_mutex;
	/* struct sunxi_actuator_ctrl actuator_ext_ctrl; */
	struct actuator_func_tbl func_tbl;
	struct v4l2_subdev sdev;
	struct v4l2_subdev_ops *sdev_ops;
	struct actuator_set_info_t set_info;
	struct actuator_get_info_t get_info;

	int work_status;

	unsigned short active_min;
	unsigned short active_max;
	unsigned short total_steps;

	/* unsigned short initial_code; */
	unsigned short curr_pos;
	unsigned short curr_code;

	unsigned short *step_position_table;

};

int actuator_move_pos(struct actuator_ctrl_t *a_ctrl,
		int num_steps,
		int direction);
/* int actuator_init_table(struct actuator_ctrl_t *a_ctrl); */
/* int actuator_set_default_focus(struct actuator_ctrl_t *a_ctrl); */
/* int actuator_af_power_down(struct actuator_ctrl_t *a_ctrl); */

int actuator_write_focus(struct actuator_ctrl_t *a_ctrl,
		unsigned short pos);
/* unsigned short curr_lens_pos, */
/* struct damping_params_t *damping_params, */
/* char sign_direction, */
/* short code_boundary); */
long sunxi_actuator_ioctl(struct v4l2_subdev *sd, unsigned int cmd, void *arg);

#endif	/* __ACTUATOR__H__ */
