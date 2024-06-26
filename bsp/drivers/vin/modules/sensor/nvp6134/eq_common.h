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

#ifndef __EQUALIZER_COMMON_H__
#define __EQUALIZER_COMMON_H__

/********************************************************************
 *  define and enum
 ********************************************************************/
/* stage( distance )*/
#define EQ_STAGE_0 0x00 /* No EQ(NO INPUT) */
#define EQ_STAGE_1 0x01 /* 0~50M(short) */
#define EQ_STAGE_2 0x02 /* 50~150M */
#define EQ_STAGE_3 0x03 /* 150~250M */
#define EQ_STAGE_4 0x04 /* 250~350M */
#define EQ_STAGE_5 0x05 /* 350~450M */
#define EQ_STAGE_6 0x06 /* 450~550M */

/* EQ video mode */
#define EQ_VIDEO_MODE_AFHD_2550 0x00
#define EQ_VIDEO_MODE_AFHD_3060 0x01
#define EQ_VIDEO_MODE_AHD_25 0x02
#define EQ_VIDEO_MODE_AHD_30 0x03
#define EQ_VIDEO_MODE_CFHD_30 0x04
#define EQ_VIDEO_MODE_CFHD_25 0x05
#define EQ_VIDEO_MODE_CHD_30 0x06
#define EQ_VIDEO_MODE_CHD_25 0x07
#define EQ_VIDEO_MODE_TFHD_25 0x08
#define EQ_VIDEO_MODE_TFHD_30 0x09
#define EQ_VIDEO_MODE_THD_A25 0x0A
#define EQ_VIDEO_MODE_THD_A30 0x0B
#define EQ_VIDEO_MODE_THD_B25 0x0C
#define EQ_VIDEO_MODE_THD_B30 0x0D
#define EQ_VIDEO_MODE_AHD_3M_18 0x0E
#define EQ_VIDEO_MODE_AHD_3M_25 0x0F
#define EQ_VIDEO_MODE_AHD_3M_30 0x10
#define EQ_VIDEO_MODE_CHD_60 0x11
#define EQ_VIDEO_MODE_CHD_50 0x12
#define EQ_VIDEO_MODE_AHD_GAIN_25 0x13
#define EQ_VIDEO_MODE_AHD_GAIN_30 0x14
#define EQ_VIDEO_MODE_THD_3M_18 0x15
#define EQ_VIDEO_MODE_THD_5M_18 0x16
#define EQ_VIDEO_MODE_AHD_4M_NRT 0x20
#define EQ_VIDEO_MODE_AHD_4M_25P 0x21
#define EQ_VIDEO_MODE_AHD_4M_30P 0x22
#define EQ_VIDEO_MODE_AHD_5M_12_5P 0x23
#define EQ_VIDEO_MODE_AHD_5M_20P 0x24

#define SUPPORT_RECOVERY_TYPE1_EQ /* enable */

/********************************************************************
 *  structure
 ********************************************************************/
/* EQ Set value - baseon */
typedef struct __equalizer_baseon__ {
	unsigned char eq_analog_eq;
	unsigned char eq_digital_eq1;
	unsigned char eq_digital_eq2;
	unsigned char eq_g_sel;
	unsigned char eq_b_lpf;
	unsigned char eq_lpf_bypass;
	unsigned char eq_ref_volt;
} equalizer_baseon;

/* EQ Set value - coeff A */
typedef struct __equalizer_coeff_a__ {
	unsigned char coeff_A_a0_8;
	unsigned char coeff_A_a0;
	unsigned char coeff_A_a1;
	unsigned char coeff_A_a2;
	unsigned char coeff_A_B0;
	unsigned char coeff_A_B1;
	unsigned char coeff_A_B2;
} equalizer_coeff_a;

/* EQ Set value - coeff B */
typedef struct __equalizer_coeff_b__ {
	unsigned char coeff_B_a0_8;
	unsigned char coeff_B_a0;
	unsigned char coeff_B_a1;
	unsigned char coeff_B_a2;
	unsigned char coeff_B_B0;
	unsigned char coeff_B_B1;
	unsigned char coeff_B_B2;
} equalizer_coeff_b;

/* EQ Set value - color  */
typedef struct __equalizer_color__ {
	unsigned char color_brightness;
	unsigned char color_contrast;
	unsigned char color_h_peaking;
	unsigned char color_saturation;
	unsigned char color_hue;
	unsigned char color_u_gain;
	unsigned char color_v_gain;
	unsigned char color_u_offset;
	unsigned char color_v_offset;
	unsigned char color_black_level;
	unsigned char color_fsc_mode;
	unsigned char color_acc_ref;
	unsigned char color_sat_sub;
	unsigned char eq_h_dly_offset;
	unsigned char color_yc_delay;
	unsigned char color_acc_min_val;
	unsigned char color_acc_max_val;
} equalizer_color;

enum __CABLE_TYPE_INFORMATION__ {
	CABLE_TYPE_COAX = 0,
	CABLE_TYPE_UTP,

	CABLE_TYPE_MAX
};

/********************************************************************
 *  external api
 ********************************************************************/
unsigned int GetAccGain(unsigned char ch);
unsigned int GetYPlusSlope(unsigned char ch);
unsigned int GetYMinusSlope(unsigned char ch);
unsigned char GetACMinValue(unsigned char ch);
unsigned char GetACMaxValue(unsigned char ch);
unsigned char GetDCValue(unsigned char ch);
unsigned char SetAnalogIP(unsigned char ch);

unsigned char eq_get_stage(unsigned char ch, unsigned char resol,
			   unsigned int value1, unsigned int value2,
			   unsigned int value3, int vfmt);
int get_resol_to_eqtable(unsigned char ch, int resol, int vfmt);
void eq_analog_ip_set_value(unsigned char ch, unsigned char g_sel_val,
			    unsigned char b_lpf_val,
			    unsigned char lpf_bypass_val,
			    unsigned char ref_volt_val,
			    unsigned char clk_dly_val);
void eq_adjust_eqvalue(unsigned char ch, unsigned char resol, int vfmt,
		       int stage);
void eq_adjust_recovery(unsigned char ch, unsigned char resol, int vfmt,
			int stage, char *start_val, char *end_val);
unsigned char eq_get_thd_stage(unsigned char ch, unsigned char resol, int vfmt);
unsigned char eq_get_thd_stage_720P_A(unsigned char ch, unsigned char resol,
				      int vfmt);
unsigned char eq_get_cvi_stage_720P50(unsigned char ch, unsigned char resol,
				      int vfmt);
unsigned char eq_get_cvi_stage_720P60(unsigned char ch, unsigned char resol,
				      int vfmt);

unsigned char eq_set_distinguish_type(unsigned char ch,
				      unsigned char curvidmode,
				      unsigned char vfmt);

void eq_set_value(unsigned char ch, void *p_param);
void eq_coeff_A_setting_val(unsigned char ch, void *p_param);
void eq_coeff_B_setting_val(unsigned char ch, void *p_param);
void eq_color_setting_val(unsigned char ch, void *p_param);
#endif /* End of __EQUALIZER_COMMON_H__ */

/********************************************************************
 *  End of file
 ********************************************************************/
