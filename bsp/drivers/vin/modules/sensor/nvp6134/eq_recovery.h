/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * A V4L2 driver for nvp6134 cameras and AHD Coax protocol.
 *
 * Copyright (C) 2016 	NEXTCHIP Inc. All rights reserved.
 * Description	: communicate between Decoder and ISP
 * 				  get information(AGC, motion, FPS) of ISP
 * 				  set NRT/RT
 * 				  upgrade Firmware of ISP
 * Authors:
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __EQUALIZER_RECOVERY_H__
#define __EQUALIZER_RECOVERY_H__

#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/videodev2.h>
#include <media/v4l2-device.h>
#include <media/v4l2-mediabus.h>

#include "../camera.h"
#include "../sensor_helper.h"
#include "csi_dev_nvp6134.h"

/********************************************************************
 *  define and enum
 ********************************************************************/

/********************************************************************
 *  structure
 ********************************************************************/
/* EQ structure for Recovery */
typedef struct _nvp6134_eq_recovery_ {
	unsigned char ch_recovery[4]; /* reserved */

} nvp6134_eq_recovery_;

/********************************************************************
 *  external api
 ********************************************************************/
extern int __eq_recovery_Atype(int ch, int curvidmode, int vfmt,
			       int acc_gain_status, int dc_val, int ac_min_val,
			       int ac_max_val, int dc_value);
extern void __eq_recovery_Btype(int ch, int curvidmode, int vfmt,
				int acc_gain_status, int y_minus_slope,
				int y_plus_slope);

#endif /* End of __EQUALIZER_RECOVERY_H__ */

/********************************************************************
 *  End of file
 ********************************************************************/
