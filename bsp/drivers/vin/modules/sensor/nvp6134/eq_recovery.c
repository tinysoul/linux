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

#include "../../../utility/vin_log.h"
/* #include "video.h" */
#include "eq_recovery.h"
#include "common.h"
#include "eq_common.h"
/*******************************************************************************
 * extern variable
 *******************************************************************************/
extern unsigned int nvp6134_iic_addr[4];
extern unsigned char ch_mode_status[16];
extern unsigned char ch_vfmt_status[16];
extern unsigned int nvp6134_cnt;

/*******************************************************************************
 * internal variable
 *******************************************************************************/
/* Atype is THD */
int __recovery_Atype_logic1(int ch, int curvidmode, int vfmt,
			    int acc_gain_status, int dc_val, int ac_min_val,
			    int ac_max_val, int dc_value);
void __recovery_Atype_logic2(int ch, int curvidmode, int vfmt,
			     int acc_gain_status, int dc_val, int ac_min_val,
			     int ac_max_val, int dc_value);
void __recovery_Atype_logic3(int ch, int curvidmode, int vfmt,
			     int acc_gain_status, int dc_val, int ac_min_val,
			     int ac_max_val, int dc_value);

/* Btype is AHD, CVI */
void __recovery_Btype_logic1(int ch, int curvidmode, int vfmt,
			     int acc_gain_status, int y_minus_slope,
			     int y_plus_slope);
void __recovery_Btype_logic2(int ch, int curvidmode, int vfmt,
			     int acc_gain_status, int y_minus_slope,
			     int y_plus_slope);
void __recovery_Btype_logic3(int ch, int curvidmode, int vfmt,
			     int acc_gain_status, int y_minus_slope,
			     int y_plus_slope);

/*******************************************************************************
 *
 *
 *
 *  Functions(for Recovery)
 *
 *
 *
 *******************************************************************************/
/*******************************************************************************
*	Description		: recovery type1 EQ - Atype
*	Argurments		: ch(channel information), curvidmode(Current
*Video mode)
*					  acc_gain_status(ACC gain status-Auto Color
*Gain), dc_val(DC value),
*					  ac_min_val(ACC Min value), ac_max_val(ACC
*max value),
*					  dc_value(DC value)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
int __recovery_Atype_logic1(int ch, int curvidmode, int vfmt,
			    int acc_gain_status, int dc_val, int ac_min_val,
			    int ac_max_val, int dc_value)
{
	unsigned int thd_tp_value; /* TEST Pattern Size in THD */
	int ret = 0x7FF;
#if 1
	/* To recovery FSC Locking Miss Situation */
	if (acc_gain_status == 0x7FF) {
		thd_tp_value = (dc_val - ac_min_val) + (ac_max_val - dc_value);
		if (thd_tp_value > 35) /* 500M THD Test Pattern size is 36 under. */ {
			gpio_i2c_write(nvp6134_iic_addr[ch / 4], 0xFF,
				       0x05 + ch % 4); /* FSC_LOCKING Toggle */
			gpio_i2c_write(nvp6134_iic_addr[ch / 4], 0x25,
				       0xDC); /* FSC_LOCKING Toggle */
			msleep(35);
			gpio_i2c_write(nvp6134_iic_addr[ch / 4], 0x25,
				       0xCC); /* FSC_LOCKING Toggle */
			msleep(35);
			ret = GetAccGain(ch);
			printk("EXT FSC Toggling to locking recovery. TP_value "
			       "= %d\n",
			       thd_tp_value);
		} else {
			ret = GetAccGain(ch);
			printk("EXT Cable Distance MAYBE 400M Over. TP_value = "
			       "%d\n",
			       thd_tp_value);
		}
		printk("EXT ACC_GAIN is MAX. ACC_GAIN = %x\n", acc_gain_status);
	} else {
		ret = GetAccGain(ch);
		printk("EXT ACC_GAIN is Not MAX. ACC_GAIN = %x\n",
		       acc_gain_status);
	}
	return ret;
#else

	unsigned char get_start_val;
	unsigned char get_end_val;

	acc_gain_status = GetAccGain(ch);
	tbl_num = get_resol_to_eqtable(curvidmode, vfmt);
	stage = eq_get_stage(curvidmode, acc_gain_status, 0 /* Not used */,
			     0 /* Not used */, vfmt);
	eq_adjust_recovery(ch, curvidmode, vfmt, stage, &get_start_val,
			   &get_end_val);
	thd_tp_value = (dc_val - ac_min_val) + (ac_max_val - dc_value);

	for (get_cnt = 0; get_cnt < 10; get_cnt++) {
		if (get_start_val < thd_tp_value &&
		    get_end_val > thd_tp_value) {
			printk(">>>>> DRV : %s eq get color success\n",
			       __func__);
			break;
		} else {
			gpio_i2c_write(nvp6134_iic_addr[ch / 4], 0xFF,
				       0x05 + (ch % 4));
			gpio_i2c_write(nvp6134_iic_addr[ch / 4], 0x23,
				       0x80); /* PN init - unlock */
			msleep(1);
			gpio_i2c_write(nvp6134_iic_addr[ch / 4], 0x23,
				       0x00); /* PN init Toggle */
			msleep(1);
			gpio_i2c_write(nvp6134_iic_addr[ch / 4], 0x23,
				       0x80); /* PN init Toggle */
			msleep(1);
			gpio_i2c_write(nvp6134_iic_addr[ch / 4], 0x23,
				       0x00); /* PN init Toggle */
			msleep(1);
			printk(">>>>> DRV : %s get_cnt[%d]	\n", __func__,
			       get_cnt);
		}
	}
#endif
}

/*******************************************************************************
*	Description		: recovery type2 EQ - Atype
*	Argurments		: ch(channel information), curvidmode(Current
*Video mode)
*					  acc_gain_status(ACC gain status-Auto Color
*Gain), dc_val(DC value),
*					  ac_min_val(ACC Min value), ac_max_val(ACC
*max value),
*					  dc_value(DC value)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
void __recovery_Atype_logic2(int ch, int curvidmode, int vfmt,
			     int acc_gain_status, int dc_val, int ac_min_val,
			     int ac_max_val, int dc_value)
{
}

/*******************************************************************************
*	Description		: recovery type3 EQ - Atype
*	Argurments		: ch(channel information), curvidmode(Current
*Video mode)
*					  acc_gain_status(ACC gain status-Auto Color
*Gain), dc_val(DC value),
*					  ac_min_val(ACC Min value), ac_max_val(ACC
*max value),
*					  dc_value(DC value)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
void __recovery_Atype_logic3(int ch, int curvidmode, int vfmt,
			     int acc_gain_status, int dc_val, int ac_min_val,
			     int ac_max_val, int dc_value)
{
}

/*******************************************************************************
*	Description		: recovery type1 EQ - Btype
*	Argurments		: ch(channel information), curvidmode(Current
*Video mode)
*					  acc_gain_status(ACC gain status-Auto Color
*Gain),
*					  y_minus_slope(Y minus slope),
*y_plus_slope(Y minus slope)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
void __recovery_Btype_logic1(int ch, int curvidmode, int vfmt,
			     int acc_gain_status, int y_minus_slope,
			     int y_plus_slope)
{
/* /* testing...
	return 0;
	unsigned int  thd_tp_value; /* TEST Pattern Size in THD
	int tbl_num = 0;
	unsigned char stage;
	unsigned char get_start_val;
	unsigned char get_end_val;
	int get_cnt = 0;

#if VIN_FALSE
	/* acc_gain_status value start/end
		not->pn init
		true->pass


	/* To recovery FSC Locking Miss Situation
	if (acc_gain_status == 0x7FF) {
		/* thd_tp_value = (dc_val - ac_min_val) + (ac_max_val - dc_value );
		if (thd_tp_value > 35)   /* 500M THD Test Pattern size is 36 under.  {
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+ch%4); /* FSC_LOCKING Toggle
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x25, 0xDC); /* FSC_LOCKING Toggle
			msleep(10);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x25, 0xCC); /* FSC_LOCKING Toggle
			msleep(10);
			acc_gain_status = GetAccGain(ch);
			printk("EXT FSC Toggling to locking recovery. TP_value = %d\n", thd_tp_value);
		} else {
			printk("EXT Cable Distance MAYBE 400M Over. TP_value = %d\n", thd_tp_value);
		}
		printk("EXT ACC_GAIN is MAX. ACC_GAIN = %x\n", acc_gain_status);
	} else {
		printk("EXT ACC_GAIN is Not MAX. ACC_GAIN = %x\n", acc_gain_status);
	}
#else
	acc_gain_status = GetAccGain(ch);
	tbl_num = get_resol_to_eqtable(ch, curvidmode, vfmt);
	stage = eq_get_stage(ch, curvidmode, y_plus_slope, y_minus_slope, 0, vfmt);/* Not used
	eq_adjust_recovery(ch, curvidmode, vfmt, stage, &get_start_val, &get_end_val);

	printk("\r\ntbl_num[%d]	stage[%d]", tbl_num, stage);
	printk("\r\ninside __recovery_Btype_logic1!!! \n start val [0x%x] end val [0x%x]\n", get_start_val, get_end_val);

	for (get_cnt = 0; get_cnt < 10; get_cnt++) {
		acc_gain_status = GetAccGain(ch);
		if (get_start_val < acc_gain_status && get_end_val > acc_gain_status) {
			printk(">>>>> DRV : %s eq get color success\n", __func__);
			break;
		} else {
			/* pn init
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0xFF, 0x05+(ch%4));
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x23, 0x80); /* PN init - unlock
			msleep(1);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x23, 0x00); /* PN init Toggle
			msleep(1);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x23, 0x80); /* PN init Toggle
			msleep(1);
			gpio_i2c_write(nvp6134_iic_addr[ch/4], 0x23, 0x00); /* PN init Toggle
			msleep(1);
			printk(">>>>> DRV : %s get_cnt[%d]	\n", __func__, get_cnt);
		}
	}
#endif
*/
}

/*******************************************************************************
*	Description		: recovery type2 EQ - Btype
*	Argurments		: ch(channel information), curvidmode(Current
*Video mode)
*					  acc_gain_status(ACC gain status-Auto Color
*Gain),
*					  y_minus_slope(Y minus slope),
*y_plus_slope(Y minus slope)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
void __recovery_Btype_logic2(int ch, int curvidmode, int vfmt,
			     int acc_gain_status, int y_minus_slope,
			     int y_plus_slope)
{
}

/*******************************************************************************
*	Description		: recovery type3 EQ - Btype
*	Argurments		: ch(channel information), curvidmode(Current
*Video mode)
*					  acc_gain_status(ACC gain status-Auto Color
*Gain),
*					  y_minus_slope(Y minus slope),
*y_plus_slope(Y minus slope)
*	Return value	: void
*	Modify			:
*	warning			:
*******************************************************************************/
void __recovery_Btype_logic3(int ch, int curvidmode, int vfmt,
			     int acc_gain_status, int y_minus_slope,
			     int y_plus_slope)
{
}

/*******************************************************************************
 *
 *
 *
 *  Functions(for Recovery)
 *
 *
 *
 *******************************************************************************/

/*******************************************************************************
*	Description		: recovery EQ - Atype
*	Argurments		: ch(channel information), curvidmode(Current
*Video mode)
*					  acc_gain_status(ACC gain status-Auto Color
*Gain), dc_val(DC value),
*					  ac_min_val(ACC Min value), ac_max_val(ACC
*max value),
*					  dc_value(DC value)
*	Return value	: void
*	Modify			:
*	warning			:For using THD
*******************************************************************************/
int __eq_recovery_Atype(int ch, int curvidmode, int vfmt, int acc_gain_status,
			int dc_val, int ac_min_val, int ac_max_val,
			int dc_value)
{
	int ret = 0x7FF;
/* recovery logic of EQ */
#if defined(SUPPORT_RECOVERY_TYPE1_EQ)
	ret = __recovery_Atype_logic1(ch, curvidmode, vfmt, acc_gain_status,
				      dc_val, ac_min_val, ac_max_val, dc_value);
#elif defined(SUPPORT_RECOVERY_TYPE2_EQ)
	__recovery_Atype_logic2(ch, curvidmode, vfmt, acc_gain_status, dc_val,
				ac_min_val, ac_max_val, dc_value);
#elif defined(SUPPORT_RECOVERY_TYPE3_EQ)
	__recovery_Atype_logic3(ch, curvidmode, vfmt, acc_gain_status, dc_val,
				ac_min_val, ac_max_val, dc_value);
#endif
	return ret;
}

/*******************************************************************************
*	Description		: recovery EQ - Btype
*	Argurments		: ch(channel information), curvidmode(Current
*Video mode)
*					  acc_gain_status(ACC gain status-Auto Color
*Gain), dc_val(DC value),
*					  ac_min_val(ACC Min value), ac_max_val(ACC
*max value),
*					  dc_value(DC value)
*	Return value	: void
*	Modify			:
*	warning			:For using AHD/CHD
*******************************************************************************/
void __eq_recovery_Btype(int ch, int curvidmode, int vfmt, int acc_gain_status,
			 int y_minus_slope, int y_plus_slope)
{
/* recovery logic of EQ */
#if defined(SUPPORT_RECOVERY_TYPE1_EQ)
	__recovery_Btype_logic1(ch, curvidmode, vfmt, acc_gain_status,
				y_minus_slope, y_plus_slope);
#elif defined(SUPPORT_RECOVERY_TYPE2_EQ)
	__recovery_Btype_logic2(ch, curvidmode, vfmt, acc_gain_status,
				y_minus_slope, y_plus_slope);
#elif defined(SUPPORT_RECOVERY_TYPE3_EQ)
	__recovery_Btype_logic3(ch, curvidmode, vfmt, acc_gain_status,
				y_minus_slope, y_plus_slope);
#endif
}
/*******************************************************************************
*	End of file
*******************************************************************************/
