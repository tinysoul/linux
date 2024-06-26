/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright(c) 2020 - 2023 Allwinner Technology Co.,Ltd. All rights reserved. */
/*
 * linux-4.9/drivers/media/platform/sunxi-vfe/lib/bsp_isp.h
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
 ***************************************************************************************
 *
 * bsp_isp.h
 *
 * Hawkview ISP - bsp_isp.h module
 *
 * Copyright (c) 2013 by Allwinnertech Co., Ltd.  http://www.allwinnertech.com
 *
 * Version		  Author         Date		    Description
 *
 *   1.0		Yang Feng	2013/12/25	    First Version
 *
 ****************************************************************************************
 */
#ifndef __BSP__ISP__H
#define __BSP__ISP__H

#include "../bsp_common.h"
#include "bsp_isp_comm.h"

#define MAX_ISP_SRC_CH_NUM 3


enum isp_src_ch_mode {
	ISP_SINGLE_CH,
	ISP_DUAL_CH,
};

struct isp_frame_info {
	unsigned int            byte_size;
	struct isp_size         pixel_size[ISP_MAX_CH_NUM];
	unsigned int            scale_ratio;
	enum bus_pixeltype      bus_pixel_type;
	enum pixel_fmt          pixel_fmt[ISP_MAX_CH_NUM];
};

struct isp_init_para {
	enum isp_src_ch_mode    isp_src_ch_mode;
	unsigned int            isp_src_ch_en[MAX_ISP_SRC_CH_NUM];
	/*
	*void                    *isp_reg_vaddr;
	*void                    *isp_load_vaddr;
	*void                    *isp_load_paddr;
	*void                    *isp_load_dma_addr;
	*void                    *isp_save_vaddr;
	*void                    *isp_save_paddr;
	*void                    *isp_save_dma_addr;
	*/
};

struct isp_table_addr {
	void            *isp_def_lut_tbl_vaddr;
	void            *isp_def_lut_tbl_paddr;
	void            *isp_def_lut_tbl_dma_addr;
	void            *isp_lsc_tbl_vaddr;
	void            *isp_lsc_tbl_paddr;
	void            *isp_lsc_tbl_dma_addr;
	void            *isp_gamma_tbl_vaddr;
	void            *isp_gamma_tbl_paddr;
	void            *isp_gamma_tbl_dma_addr;
	void            *isp_linear_tbl_vaddr;
	void            *isp_linear_tbl_paddr;
	void            *isp_linear_tbl_dma_addr;

	void            *isp_drc_tbl_vaddr;
	void            *isp_drc_tbl_paddr;
	void            *isp_drc_tbl_dma_addr;
	void            *isp_disc_tbl_vaddr;
	void            *isp_disc_tbl_paddr;
	void            *isp_disc_tbl_dma_addr;
};

void bsp_isp_enable(void);
void bsp_isp_disable(void);

void bsp_isp_rot_enable(void);
void bsp_isp_rot_disable(void);
void bsp_isp_channel_enable(enum isp_channel ch);
void bsp_isp_channel_disable(enum isp_channel ch);

void bsp_isp_video_capture_start(void);
void bsp_isp_video_capture_stop(void);
void bsp_isp_image_capture_start(void);
void bsp_isp_image_capture_stop(void);
unsigned int bsp_isp_get_para_ready(void);
void bsp_isp_set_para_ready(void);
void bsp_isp_clr_para_ready(void);

/*
 * irq_flag:
 *
 * FINISH_INT_EN
 * START_INT_EN
 * PARA_SAVE_INT_EN
 * PARA_LOAD_INT_EN
 * SRC0_FIFO_INT_EN
 * SRC1_FIFO_INT_EN
 * ROT_FINISH_INT_EN
 * ISP_IRQ_EN_ALL
 */
void bsp_isp_irq_enable(unsigned int irq_flag);
void bsp_isp_irq_disable(unsigned int irq_flag);
unsigned int bsp_isp_get_irq_status(unsigned int irq);
int bsp_isp_int_get_enable(void);

void bsp_isp_clr_irq_status(unsigned int irq);
void bsp_isp_set_statistics_addr(unsigned int addr);

void bsp_isp_set_flip(enum isp_channel ch, enum enable_flag on_off);
void bsp_isp_set_mirror(enum isp_channel ch, enum enable_flag on_off);

void bsp_isp_set_input_fmt(enum isp_input_fmt fmt, enum isp_input_seq seq_t);

void bsp_isp_set_output_fmt(enum isp_output_fmt isp_fmt, enum isp_output_seq seq_t, enum isp_channel ch);
void bsp_isp_set_rot(enum isp_channel ch, enum isp_rot_angle angle);
int min_scale_w_shift(int x_ratio, int y_ratio);
void bsp_isp_set_ob_zone(struct isp_size *black, struct isp_size *valid, struct coor *xy, enum isp_src obc_valid_src);

void bsp_isp_set_output_size(enum isp_channel ch, struct isp_size *size);
void bsp_isp_scale_cfg(enum isp_channel ch, int x_ratio, int y_ratio, int weight_shift);
void bsp_isp_set_stride_y(unsigned int stride_val, enum isp_channel ch);
void bsp_isp_set_stride_uv(unsigned int stride_val, enum isp_channel ch);
void bsp_isp_set_yuv_addr(struct isp_yuv_channel_addr *addr, enum isp_channel ch, enum isp_src channel_src);


void bsp_isp_set_base_addr(unsigned long vaddr);
void bsp_isp_set_dma_load_addr(unsigned long dma_addr);
void bsp_isp_set_dma_saved_addr(unsigned long dma_addr);
void bsp_isp_set_map_load_addr(unsigned long vaddr);
void bsp_isp_set_map_saved_addr(unsigned long vaddr);
void bsp_isp_update_lut_lens_gamma_table(struct isp_table_addr *tbl_addr);
void bsp_isp_update_drc_table(struct isp_table_addr *tbl_addr);
void bsp_isp_init_platform(unsigned int platform_id);
void bsp_isp_init(struct isp_init_para *para);
void bsp_isp_exit(void);

void bsp_isp_scale_enable(enum isp_channel ch);
void bsp_isp_module_enable(unsigned int module);
void bsp_isp_module_disable(unsigned int module);


unsigned int   bsp_isp_get_saved_cfa_min_rgb(void);
unsigned int   bsp_isp_get_saved_cfa_pic_tex(void);
void           bsp_isp_get_saved_wb_gain(struct isp_white_balance_gain *wb_gain_saved);
void           bsp_isp_get_saved_awb_avp(struct isp_awb_avp_stat *awb_avp_saved);
void           bsp_isp_get_saved_awb_diff_thresh(struct isp_wb_diff_threshold *awb_avp_saved);
unsigned short bsp_isp_get_saved_awb_sum_thresh(void);
void           bsp_isp_get_saved_ae_win_reg(struct isp_h3a_reg_win *ae_reg_win_saved);
int bsp_isp_get_saved_cnr_noise(void);
void bsp_isp_print_reg_saved(void);


#endif /* __BSP__ISP__H */
