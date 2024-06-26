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

#ifndef __EQUALIZER_NVP6134_H__
#define __EQUALIZER_NVP6134_H__


/********************************************************************
 *  define and enum
 ********************************************************************/
/* init equalizer buffer flag */
#define EQ_INIT_ON		0x00
#define EQ_INIT_OFF		0x01

/* common EQ define */
#define EQ_COMMON_ON	0x00
#define EQ_COMMON_OFF	0x01

/* equalizer - video on/off */
#define EQ_VIDEO_OFF	0x00
#define EQ_VIDEO_ON		0x01

/* max channel number */
#define MAX_CHANNEL_NUM		16

/********************************************************************
 *  structure
 ********************************************************************/
/* EQ structure, this structure shared with application */
typedef struct _nvp6134_equalizer_ {

	unsigned char ch_man_eq_flag[MAX_CHANNEL_NUM];	/* manual opration eq stage flag        16.7.28 -rudy */
	unsigned char ch_previdmode[MAX_CHANNEL_NUM];	/* pre video mode */
	unsigned char ch_curvidmode[MAX_CHANNEL_NUM];	/* current video mode */
	unsigned char ch_previdon[MAX_CHANNEL_NUM];		/* pre video on/off status */
	unsigned char ch_curvidon[MAX_CHANNEL_NUM];		/* current video on/off status */
	unsigned char ch_previdformat[MAX_CHANNEL_NUM];	/* pre video format for Auto detection value */
	unsigned char ch_curvidformat[MAX_CHANNEL_NUM];	/* current video format for Auto detection value */


	int acc_gain[MAX_CHANNEL_NUM];					/* first, get value: acc gain(value when video is ON status) */
	int y_plus_slope[MAX_CHANNEL_NUM];				/* first, get value: y plus slope(value when video is ON status) */
	int y_minus_slope[MAX_CHANNEL_NUM];				/* first, get value: y minus slope(value when video is ON status) */

	int cur_y_plus_slope[MAX_CHANNEL_NUM];			/* current y plus slope */
	int cur_acc_gain[MAX_CHANNEL_NUM];				/* current acc gain */
	int cur_y_minus_slope[MAX_CHANNEL_NUM];			/* current y minus slope */

	int fr_ac_min_value[MAX_CHANNEL_NUM];			/* first AC Min Value */
	int fr_ac_max_value[MAX_CHANNEL_NUM];			/* first AC Max Value */
	int fr_dc_value[MAX_CHANNEL_NUM];				/* first DC value */

	int cur_fr_ac_min[MAX_CHANNEL_NUM];				/* current y plus slope */
	int cur_fr_ac_max[MAX_CHANNEL_NUM];				/* current acc gain */
	int cur_fr_dc[MAX_CHANNEL_NUM];					/* current y minus slope */

	unsigned char ch_stage[MAX_CHANNEL_NUM];			/* stage of channel */
	unsigned char ch_vfmt_status[MAX_CHANNEL_NUM];	/* NTSC/PAL */

} nvp6134_equalizer;

/********************************************************************
 *  external api
 ********************************************************************/
extern void eq_init_each_format(unsigned char ch, int mode, const unsigned char vfmt);
extern int	nvp6134_set_equalizer(unsigned char ch);
unsigned int distinguish_GetYPlusSlope(unsigned char ch);
unsigned int distinguish_GetYMinusSlope(unsigned char ch);
unsigned int distinguish_GetAccGain(unsigned char ch);

#endif	/* End of __EQUALIZER_NVP6134_H__ */

/********************************************************************
 *  End of file
 ********************************************************************/
