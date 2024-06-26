/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright(c) 2020 - 2023 Allwinner Technology Co.,Ltd. All rights reserved. */
/*
 * linux-4.9/drivers/media/platform/sunxi-vfe/csi/csi_reg.h
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
 * sunxi csi register read/write interface header file
 * Author:raymonxiu
 */
#ifndef __CSI__REG__H__
#define __CSI__REG__H__

#include <linux/types.h>

#define MAX_CSI 2
/*
 * if format
 */
enum csi_if {
	CSI_IF_INTLV                = 0x00,	/* 1SEG DATA in one channel  */
	CSI_IF_SPL                  = 0x01,	/* 2SEG: 1SEG Y in one channel , 1SEG UV in second channel */
	CSI_IF_PL                   = 0x02,	/* 3SEG YUV444 */
	CSI_IF_PL_SPL               = 0x03,	/* 3SEG YUV444 to 2SEG YUV422  */

	CSI_IF_CCIR656_1CH          = 0x04,	/* 1SEG ccir656  1ch   */
	CSI_IF_CCIR656_1CH_SPL      = 0x05,	/* 2SEG  ccir656 1ch   */
	CSI_IF_CCIR656_1CH_PL       = 0x06,	/* 3SEG ccir656  1ch   */
	CSI_IF_CCIR656_1CH_PL_SPL   = 0x07,	/* 3SEG to 2SEG ccir656  1ch   */

	CSI_IF_CCIR656_2CH          = 0x0c,	/* D7~D0:ccir656  2ch   */
	CSI_IF_CCIR656_4CH          = 0x0d,	/* D7~D0:ccir656  4ch   */

	CSI_IF_MIPI                 = 0x80,	/* MIPI CSI */
};

/*
 *  data width
 */
enum csi_data_width {
	CSI_8BIT      = 0,
	CSI_10BIT     = 1,
	CSI_12BIT     = 2,
};

/*
 * input data format
 */
enum csi_input_fmt {
	CSI_RAW = 0,       /* raw stream  */
  /* CSI_BAYER = 1, */
	CSI_YUV422 = 3,    /* yuv422      */
	CSI_YUV420 = 4,    /* yuv420      */
};

/*
 * output data format
 */
enum csi_output_fmt {
	/* only when input is raw */
	CSI_FIELD_RAW_8    = 0,
	CSI_FIELD_RAW_10   = 1,
	CSI_FIELD_RAW_12   = 2,
	CSI_FIELD_RGB565   = 4,
	CSI_FIELD_RGB888   = 5,
	CSI_FIELD_PRGB888  = 6,
	CSI_FRAME_RAW_8    = 8,
	CSI_FRAME_RAW_10   = 9,
	CSI_FRAME_RAW_12   = 10,
	CSI_FRAME_RGB565   = 12,
	CSI_FRAME_RGB888   = 13,
	CSI_FRAME_PRGB888  = 14,

	/* only when input is bayer */
	/* CSI_PLANAR_RGB242 = 0,    */           /* planar rgb242 */

	/* only when input is yuv422/yuv420 */
	CSI_FIELD_PLANAR_YUV422 = 0,         /* parse a field(odd or even) into planar yuv420 */
	CSI_FIELD_PLANAR_YUV420 = 1,         /* parse a field(odd or even) into planar yuv420 */
	CSI_FRAME_PLANAR_YUV420 = 2,         /* parse and reconstruct every 2 fields(odd and even) into a frame, format is planar yuv420 */
	CSI_FRAME_PLANAR_YUV422 = 3,
	CSI_FIELD_UV_CB_YUV422  = 4,
	CSI_FIELD_UV_CB_YUV420  = 5,
	CSI_FRAME_UV_CB_YUV420  = 6,
	CSI_FRAME_UV_CB_YUV422  = 7,
	CSI_FIELD_MB_YUV422     = 8,
	CSI_FIELD_MB_YUV420     = 9,
	CSI_FRAME_MB_YUV420     = 10,
	CSI_FRAME_MB_YUV422     = 11,
	CSI_FIELD_UV_CB_YUV422_10  = 12,
	CSI_FIELD_UV_CB_YUV420_10  = 13,
  /* CSI_INTLC_INTLV_YUV422  = 15, */
};

/*
 * field sequenc or polarity
 */

enum csi_field {
	/* For Embedded Sync timing*/
	CSI_FIELD_TF    = 0,    /* top filed first */
	CSI_FIELD_BF    = 1,    /* bottom field first */

	/* For External Sync timing */
	CSI_FIELD_NEG = 0,      /* field negtive indicates odd field */
	CSI_FIELD_POS = 1,      /* field postive indicates odd field */
};

/*
 * input field selection, only when input is ccir656
 */
enum csi_field_sel {
	CSI_ODD,    /* odd field */
	CSI_EVEN,   /* even field */
	CSI_EITHER, /* either field */
};


/*
 * input source type
 */
enum csi_src_type {
	CSI_PROGRESSIVE = 0,    /* progressive */
	CSI_INTERLACE = 1,      /* interlace */
};

/*
 * input data sequence
 */
enum csi_input_seq {
	/* only when input is yuv422 */
	CSI_YUYV = 0,
	CSI_YVYU,
	CSI_UYVY,
	CSI_VYUY,

	/* only when input is byer */
	CSI_RGRG = 0,               /* first line sequence is RGRG... */
	CSI_GRGR,                 /* first line sequence is GRGR... */
	CSI_BGBG,                 /* first line sequence is BGBG... */
	CSI_GBGB,                 /* first line sequence is GBGB... */
};

/*
 * input reference signal polarity
 */
enum csi_ref_pol {
	CSI_LOW,    /* active low */
	CSI_HIGH,   /* active high */
};

/*
 * input data valid of the input clock edge type
 */
enum csi_edge_pol {
	CSI_FALLING,    /* active falling */
	CSI_RISING,     /* active rising */
};

/*
 * csi interface configuration
 */
struct csi_if_cfg {
	enum csi_src_type   src_type;     /* interlaced or progressive */
	enum csi_data_width data_width;   /* csi data width */
	enum csi_if         interface;    /* csi interface */
};

/*
 * csi timing configuration
 */
struct csi_timing_cfg {
	enum csi_field      field;    /* top or bottom field first / field polarity */
	enum csi_ref_pol    vref;     /* input vref signal polarity */
	enum csi_ref_pol    href;     /* input href signal polarity */
	enum csi_edge_pol   sample;   /* input data valid of the input clock edge type */
};

/*
 * csi mode configuration
 */
struct csi_fmt_cfg {
	enum csi_input_fmt      input_fmt;   /* input data format */
	enum csi_output_fmt     output_fmt;  /* output data format */
	enum csi_field_sel      field_sel;   /* input field selection */
	enum csi_input_seq      input_seq;   /* input data sequence */
	enum csi_data_width     data_width;   /* csi data width */
};

/*
 * csi buffer
 */

enum csi_buf_sel {
	CSI_BUF_0_A = 0,    /* FIFO for Y address A */
	CSI_BUF_0_B,        /* FIFO for Y address B */
	CSI_BUF_1_A,        /* FIFO for Cb address A */
	CSI_BUF_1_B,        /* FIFO for Cb address B */
	CSI_BUF_2_A,        /* FIFO for Cr address A */
	CSI_BUF_2_B,        /* FIFO for Cr address B */
};


/*
 * csi capture status
 */
struct csi_capture_status {
	_Bool picture_in_progress;
	_Bool video_in_progress;
};

enum csi_cap_mode {
	CSI_SCAP = 1,
	CSI_VCAP,
};

/*
 * csi interrupt
 */
enum csi_int_sel {
	CSI_INT_CAPTURE_DONE     = 0X1,
	CSI_INT_FRAME_DONE       = 0X2,
	CSI_INT_BUF_0_OVERFLOW   = 0X4,
	CSI_INT_BUF_1_OVERFLOW   = 0X8,
	CSI_INT_BUF_2_OVERFLOW   = 0X10,
	CSI_INT_PROTECTION_ERROR = 0X20,
	CSI_INT_HBLANK_OVERFLOW  = 0X40,
	CSI_INT_VSYNC_TRIG       = 0X80,
	CSI_INT_ALL              = 0XFF,
};

/*
 * csi interrupt status
 */
struct csi_int_status {
	_Bool capture_done;
	_Bool frame_done;
	_Bool buf_0_overflow;
	_Bool buf_1_overflow;
	_Bool buf_2_overflow;
	_Bool protection_error;
	_Bool hblank_overflow;
	_Bool vsync_trig;
};

int csi_set_base_addr(unsigned int sel, unsigned long addr);
void csi_enable(unsigned int sel);
void csi_disable(unsigned int sel);
void csi_if_cfg(unsigned int sel, struct csi_if_cfg *csi_if_cfg);
void csi_timing_cfg(unsigned int sel, struct csi_timing_cfg *csi_tmg_cfg);
void csi_fmt_cfg(unsigned int sel, unsigned int ch, struct csi_fmt_cfg *csi_fmt_cfg);
void csi_set_buffer_address(unsigned int sel, unsigned int ch, enum csi_buf_sel buf, u64 addr);
u64 csi_get_buffer_address(unsigned int sel, unsigned int ch, enum csi_buf_sel buf);
void csi_capture_start(unsigned int sel, unsigned int ch_total_num, enum csi_cap_mode csi_cap_mode);
void csi_capture_stop(unsigned int sel, unsigned int ch_total_num, enum csi_cap_mode csi_cap_mode);
void csi_capture_get_status(unsigned int sel, unsigned int ch, struct csi_capture_status *status);
void csi_set_size(unsigned int sel, unsigned int ch, unsigned int length_h, unsigned int length_v, unsigned int buf_length_y, unsigned int buf_length_c);
void csi_set_offset(unsigned int sel, unsigned int ch, unsigned int start_h, unsigned int start_v);
void csi_int_enable(unsigned int sel, unsigned int ch, enum csi_int_sel interrupt);
void csi_int_disable(unsigned int sel, unsigned int ch, enum csi_int_sel interrupt);
void csi_int_get_status(unsigned int sel, unsigned int ch, struct csi_int_status *status);
void csi_int_clear_status(unsigned int sel, unsigned int ch, enum csi_int_sel interrupt);

#endif
