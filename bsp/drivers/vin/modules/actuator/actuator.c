/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright(c) 2020 - 2023 Allwinner Technology Co.,Ltd. All rights reserved. */
/*
 *
 * Copyright (c) 2014 softwinner.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

/*
 ******************************************************************************
 *
 * actuator.c
 *
 * Hawkview ISP - actuator.c module
 *
 * Copyright (c) 2015 by Allwinnertech Co., Ltd.  http://www.allwinnertech.com
 *
 * Version        Author         Date           Description
 *
 *   3.0        Yang Feng        2015/12/02 ISP Tuning Tools Support
 *
 ******************************************************************************
 */
#include "../../utility/vin_log.h"
#include <linux/module.h>
#include "actuator.h"

#define SUNXI_ACT_NAME "act"

long sunxi_actuator_ioctl(struct v4l2_subdev *sd,
			unsigned int cmd, void *arg)
{
#if IS_ENABLED(CONFIG_ACTUATOR_MODULE)
	long ret = 0;

	struct actuator_ctrl_t *act_ctrl;

	act_ctrl = container_of(sd, struct actuator_ctrl_t, sdev);

	mutex_lock(act_ctrl->actuator_mutex);

	switch (cmd) {
	case ACT_INIT:{
		struct actuator_para_t *a_para = arg;

		act_dbg("ACT_INIT\n");
		ret = act_ctrl->func_tbl.actuator_init(act_ctrl, a_para);
		break;
	}
	case ACT_PWDN: {
		act_dbg("ACT_PWDN\n");
		ret = act_ctrl->func_tbl.actuator_pwdn(act_ctrl, 0);
		break;
	}
	case ACT_SOFT_PWDN: {
		act_dbg("ACT_SOFT_PWDN\n");
		ret = act_ctrl->func_tbl.actuator_pwdn(act_ctrl, 1);
		break;
	}
	case ACT_RELEASE: {
		struct actuator_ctrl_word_t *ctrlwd = arg;

		act_dbg("ACT_RELEASE\n");
		ret = act_ctrl->func_tbl.actuator_release(act_ctrl, ctrlwd);
		break;
	}
	case ACT_SET_CODE_RANGE: {
		unsigned short min, max, steps;
		unsigned short *pt = arg;

		act_dbg("ACT_SET_CODE_RANGE\n");
		min = pt[0];
		max = pt[1];
		steps = pt[2];
		if ((min < max - act_ctrl->total_steps)
		    && max < ACT_DEV_MAX_CODE) {
			act_ctrl->active_min = min;
			act_ctrl->active_max = max;
			act_ctrl->total_steps = steps;
			ret = 0;
		} else {
			ret = -1;
		}
		break;
	}
	case ACT_GET_CODE_RANGE: {
		unsigned short *pt = arg;

		act_dbg("ACT_SET_CODE_RANGE\n");
		pt[0] = act_ctrl->active_min;
		pt[1] = act_ctrl->active_max;
		pt[2] = act_ctrl->total_steps;
		break;
	}
	case ACT_SET_CODE: {
		struct actuator_ctrl_word_t *ctrlwd = arg;

		act_dbg("ACT_SET_CODE[%d][%d]\n", ctrlwd->code, ctrlwd->sr);
		ret =
		    act_ctrl->func_tbl.actuator_set_code(act_ctrl,
							 ctrlwd->code,
							 ctrlwd->sr);
		break;
	}
	case ACT_GET_CODE: {
		act_dbg("ACT_GET_CODE\n");
		ret = 0;
		break;
	}
	case ACT_SET_POS: {
		unsigned short *dest_pos = arg;

		act_dbg("ACT_SET_POS[%d]\n", dest_pos[0]);
		ret =
		    act_ctrl->func_tbl.actuator_set_pos(act_ctrl, dest_pos[0]);
		break;
	}
	case ACT_GET_POS: {
		unsigned short *step_pos = arg;

		act_dbg("ACT_GET_POS\n");
		step_pos[0] = act_ctrl->curr_pos;
		break;
	}
	case ACT_MOV_POS: {
		struct actuator_ctrl_step_t *ctrlstep = arg;

		act_dbg("ACT_MOV_POS[%x][%x]\n", ctrlstep->step,
			ctrlstep->dir);
		ret =
		    act_ctrl->func_tbl.actuator_move_pos(act_ctrl,
							 ctrlstep->step,
							 ctrlstep->dir);
		break;
	}
	default:
		return -EINVAL;
	}

	mutex_unlock(act_ctrl->actuator_mutex);

	return ret;
#else
	return 0;
#endif
}
EXPORT_SYMBOL_GPL(sunxi_actuator_ioctl);

MODULE_AUTHOR("zenglingying");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("actuator driver for sunxi");
