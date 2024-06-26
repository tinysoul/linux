/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright(c) 2020 - 2023 Allwinner Technology Co.,Ltd. All rights reserved. */
/*
 * linux-4.9/drivers/media/platform/sunxi-vfe/lib/bsp_isp_null.c
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

#include "bsp_isp.h"
#include "bsp_isp_comm.h"
#include "isp_module_cfg.h"
#include "bsp_isp_algo.h"

void bsp_isp_s_brightness(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_contrast(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_saturation(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_hue(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_auto_white_balance(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_exposure(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_auto_gain(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_gain(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_power_line_frequency(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_hue_auto(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_white_balance_temperature(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_sharpness(struct isp_gen_settings *isp_gen, int value)
{
}
void bsp_isp_s_chroma_agc(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_colorfx(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_auto_brightness(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_band_stop_filter(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_illuminators_1(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_illuminators_2(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_auto_focus_win_num(struct isp_gen_settings *isp_gen, int value, struct isp_h3a_coor_win *coor)
{
}

void bsp_isp_s_flash_mode(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_auto_exposure_win_num(struct isp_gen_settings *isp_gen, int value, struct isp_h3a_coor_win *coor)
{
}

void bsp_isp_s_gsensor_rotation(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_exposure_auto(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_exposure_absolute(struct isp_gen_settings *isp_gen, int value)
{
}
void bsp_isp_s_exposure_auto_priority(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_focus_absolute(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_focus_relative(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_focus_auto(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_auto_exposure_bias(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_auto_n_preset_white_balance(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_wide_dynamic_rage(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_image_stabilization(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_iso_sensitivity(struct isp_gen_settings *isp_gen, int value)
{
}
void bsp_isp_s_iso_sensitivity_auto(struct isp_gen_settings *isp_gen, int value)
{
}
void bsp_isp_s_scene_mode(struct isp_gen_settings *isp_gen, int value)
{
}
void bsp_isp_s_auto_focus_start(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_auto_focus_stop(struct isp_gen_settings *isp_gen, int value)
{
}

void sensor_get_auto_focus_status(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_auto_focus_range(struct isp_gen_settings *isp_gen, int value)
{
}


void bsp_isp_s_r_gain(struct isp_gen_settings *isp_gen, int value)
{
}
void bsp_isp_s_g_gain(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_s_b_gain(struct isp_gen_settings *isp_gen, int value)
{
}
void bsp_isp_s_hdr(struct isp_gen_settings *isp_gen, struct hdr_setting_t *hdr)
{
}
void bsp_isp_s_take_pic(struct isp_gen_settings *isp_gen, int value)
{
}

void bsp_isp_enable(void)
{
}

void bsp_isp_disable(void)
{
}

void bsp_isp_rot_enable(void)
{
}

void bsp_isp_rot_disable(void)
{
}

void bsp_isp_set_rot(enum isp_channel ch, enum isp_rot_angle angle)
{
}

int min_scale_w_shift(int x_ratio, int y_ratio)
{
	return 0;
}

void bsp_isp_channel_enable(enum isp_channel ch)
{
}

void bsp_isp_channel_disable(enum isp_channel ch)
{
}
void bsp_isp_video_capture_start(void)
{
}

void bsp_isp_video_capture_stop(void)
{
}

void bsp_isp_image_capture_start(void)
{
}

void bsp_isp_image_capture_stop(void)
{
}

unsigned int bsp_isp_get_para_ready(void)
{
	return 0;
}

void bsp_isp_set_para_ready(void)
{
}
void bsp_isp_clr_para_ready(void)
{
}

void bsp_isp_irq_enable(unsigned int irq_flag)
{
}
void bsp_isp_irq_disable(unsigned int irq_flag)
{
}

unsigned int bsp_isp_get_irq_status(unsigned int irq)
{
	return 0;
}

void bsp_isp_clr_irq_status(unsigned int irq)
{
}

int bsp_isp_int_get_enable(void)
{
	return 0;
}

void bsp_isp_set_statistics_addr(unsigned int addr)
{
}
void bsp_isp_set_flip(enum isp_channel ch, enum enable_flag on_off)
{
}

void bsp_isp_set_mirror(enum isp_channel ch, enum enable_flag on_off)
{
}

void bsp_isp_set_base_addr(unsigned long vaddr)
{
}

void bsp_isp_set_dma_load_addr(unsigned long dma_addr)
{
}

void bsp_isp_set_dma_saved_addr(unsigned long dma_addr)
{
}

void bsp_isp_set_map_load_addr(unsigned long vaddr)
{
}

void bsp_isp_set_map_saved_addr(unsigned long vaddr)
{
}

void bsp_isp_update_lut_lens_gamma_table(struct isp_table_addr *tbl_addr)
{
}

void bsp_isp_update_drc_table(struct isp_table_addr *tbl_addr)
{
}
void bsp_isp_set_input_fmt(enum isp_input_fmt fmt, enum isp_input_seq seq_t)
{
}
void bsp_isp_set_output_fmt(enum isp_output_fmt isp_fmt, enum isp_output_seq seq_t, enum isp_channel ch)
{
}

void bsp_isp_set_ob_zone(struct isp_size *black, struct isp_size *valid, struct coor *xy, enum isp_src obc_valid_src)
{
}

void bsp_isp_set_output_size(enum isp_channel ch, struct isp_size *size)
{
}
void bsp_isp_scale_cfg(enum isp_channel ch, int x_ratio, int y_ratio, int weight_shift)
{
}
void bsp_isp_set_stride_y(unsigned int stride_val, enum isp_channel ch)
{
}
void bsp_isp_set_stride_uv(unsigned int stride_val, enum isp_channel ch)
{
}
void bsp_isp_set_yuv_addr(struct isp_yuv_channel_addr *addr, enum isp_channel ch, enum isp_src channel_src)
{
}

void bsp_isp_scale_enable(enum isp_channel ch)
{
}
void bsp_isp_module_enable(unsigned int module)
{
}

void bsp_isp_module_disable(unsigned int module)
{
}

void bsp_isp_init(struct isp_init_para *para)
{
}

void bsp_isp_exit(void)
{
}

unsigned int bsp_isp_get_saved_cfa_min_rgb(void)
{
	return 0;
}
unsigned int bsp_isp_get_saved_cfa_pic_tex(void)
{
	return 0;
}
void bsp_isp_get_saved_wb_gain(struct isp_white_balance_gain *wb_gain_saved)
{
}

void bsp_isp_get_saved_awb_avp(struct isp_awb_avp_stat *awb_avp_saved)
{
}
void bsp_isp_get_saved_awb_diff_thresh(struct isp_wb_diff_threshold *diff_th_saved)
{
}
unsigned short bsp_isp_get_saved_awb_sum_thresh(void)
{
	return 0;
}
void bsp_isp_get_saved_ae_win_reg(struct isp_h3a_reg_win *ae_reg_win_saved)
{
}
int bsp_isp_get_saved_cnr_noise(void)
{
	return 0;
}
void bsp_isp_print_reg_saved(void)
{
}

void bsp_isp_init_platform(unsigned int platform_id)
{
}

void isp_isr(struct isp_gen_settings *isp_gen, struct isp_3a_result *isp_result)
{
}

void isp_param_init(struct isp_gen_settings *isp_gen)
{
}

void isp_config_init(struct isp_gen_settings *isp_gen)
{
}

int isp_module_init(struct isp_gen_settings *isp_gen, struct isp_3a_result *isp_result)/* after csi read ini */
{
	return 0;
}

int get_pre_ev_cumul(struct isp_gen_settings *isp_gen, struct isp_3a_result *isp_result)
{
	return 0;
}
void config_sensor_next_exposure(struct isp_gen_settings *isp_gen, struct isp_3a_result *isp_result)
{
}
