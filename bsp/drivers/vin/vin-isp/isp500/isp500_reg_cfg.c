/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright(c) 2020 - 2023 Allwinner Technology Co.,Ltd. All rights reserved. */

/* SPDX-License-Identifier: GPL-2.0 */
/*
  * isp500_reg_cfg.c
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

#include <linux/io.h>
#include <linux/kernel.h>
#include "isp500_reg.h"
#include "isp500_reg_cfg.h"

#define ISP500_MAX_NUM 2

/*
 *
 *  Load ISP register variables
 *
 */
struct isp500_reg {
	ISP_FE_CFG_REG_t *isp_fe_cfg;
	ISP_FE_CTRL_REG_t *isp_fe_ctrl;
	ISP_FE_INT_EN_REG_t *isp_fe_int_en;
	ISP_FE_INT_STA_REG_t *isp_fe_int_sta;
	ISP_DBG_OUTPUT_REG_t *isp_dbg_output;
	ISP_LINE_INT_NUM_REG_t *isp_line_int_num;
	ISP_ROT_OF_CFG_REG_t *isp_rot_of_cfg;
	ISP_REG_LOAD_ADDR_REG_t *isp_reg_load_addr;
	ISP_REG_SAVED_ADDR_REG_t *isp_reg_saved_addr;
	ISP_LUT_LENS_GAMMA_ADDR_REG_t *isp_lut_lens_gamma_addr;
	ISP_DRC_ADDR_REG_t *isp_drc_addr;
	ISP_STATISTICS_ADDR_REG_t *isp_statistics_addr;
	ISP_VER_CFG_REG_t *isp_ver_cfg;
	ISP_SRAM_RW_OFFSET_REG_t *isp_sram_rw_offset;
	ISP_SRAM_RW_DATA_REG_t *isp_sram_rw_data;
	unsigned int *isp_s0_fmerr_cnt;
	unsigned int *isp_s0_hb_cnt;

	ISP_FE_CTRL_REG_t *isp_update_flag;
	ISP_EN_REG_t *isp_en;
	ISP_MODE_REG_t *isp_mode;
	ISP_OB_SIZE_REG_t *isp_ob_size;
	ISP_OB_VALID_REG_t *isp_ob_valid;
	ISP_OB_VALID_START_REG_t *isp_ob_valid_start;

	unsigned int *isp_wdr_exp_addr0;
	unsigned int *isp_wdr_exp_addr1;
	unsigned int *isp_d3d_rec_addr0;
	unsigned int *isp_d3d_rec_addr1;
};

struct isp500_reg isp_regs[ISP500_MAX_NUM];

/*
 * Register Address
 */

void bsp_isp_map_reg_addr(unsigned long id, unsigned long isp_reg_base)
{
	isp_regs[id].isp_fe_cfg = (ISP_FE_CFG_REG_t *) (isp_reg_base + ISP_FE_CFG_REG_OFF);
	isp_regs[id].isp_fe_ctrl = (ISP_FE_CTRL_REG_t *) (isp_reg_base + ISP_FE_CTRL_REG_OFF);
	isp_regs[id].isp_fe_int_en = (ISP_FE_INT_EN_REG_t *) (isp_reg_base + ISP_FE_INT_EN_REG_OFF);
	isp_regs[id].isp_fe_int_sta = (ISP_FE_INT_STA_REG_t *) (isp_reg_base + ISP_FE_INT_STA_REG_OFF);
	isp_regs[id].isp_dbg_output = (ISP_DBG_OUTPUT_REG_t *) (isp_reg_base + ISP_DBG_OUTPUT_REG_OFF);
	isp_regs[id].isp_line_int_num = (ISP_LINE_INT_NUM_REG_t *) (isp_reg_base + ISP_LINE_INT_NUM_REG_OFF);
	isp_regs[id].isp_rot_of_cfg = (ISP_ROT_OF_CFG_REG_t *) (isp_reg_base + ISP_ROT_OF_CFG_REG_OFF);
	isp_regs[id].isp_reg_load_addr = (ISP_REG_LOAD_ADDR_REG_t *) (isp_reg_base + ISP_REG_LOAD_ADDR_REG_OFF);
	isp_regs[id].isp_reg_saved_addr = (ISP_REG_SAVED_ADDR_REG_t *) (isp_reg_base + ISP_REG_SAVED_ADDR_REG_OFF);

	isp_regs[id].isp_lut_lens_gamma_addr = (ISP_LUT_LENS_GAMMA_ADDR_REG_t *) (isp_reg_base + ISP_LUT_LENS_GAMMA_ADDR_REG_OFF);
	isp_regs[id].isp_drc_addr = (ISP_DRC_ADDR_REG_t *) (isp_reg_base + ISP_DRC_ADDR_REG_OFF);
	isp_regs[id].isp_statistics_addr = (ISP_STATISTICS_ADDR_REG_t *) (isp_reg_base + ISP_STATISTICS_ADDR_REG_OFF);
	isp_regs[id].isp_ver_cfg = (ISP_VER_CFG_REG_t *) (isp_reg_base + ISP_VER_CFG_REG_OFF);

	isp_regs[id].isp_sram_rw_offset = (ISP_SRAM_RW_OFFSET_REG_t *) (isp_reg_base + ISP_SRAM_RW_OFFSET_REG_OFF);
	isp_regs[id].isp_sram_rw_data = (ISP_SRAM_RW_DATA_REG_t *) (isp_reg_base + ISP_SRAM_RW_DATA_REG_OFF);

	isp_regs[id].isp_s0_fmerr_cnt = (unsigned int *) (isp_reg_base + 0x5c);
	isp_regs[id].isp_s0_hb_cnt = (unsigned int *) (isp_reg_base + 0x60);
#ifdef USE_DEF_PARA
	isp_regs[id].isp_en = (ISP_EN_REG_t *) (isp_reg_base + ISP_EN_REG_OFF);
	isp_regs[id].isp_mode = (ISP_MODE_REG_t *) (isp_reg_base + ISP_MODE_REG_OFF);
	isp_regs[id].isp_ob_size = (ISP_OB_SIZE_REG_t *) (isp_reg_base + ISP_OB_SIZE_REG_OFF);
	isp_regs[id].isp_ob_valid = (ISP_OB_VALID_REG_t *) (isp_reg_base + ISP_OB_VALID_REG_OFF);
	isp_regs[id].isp_ob_valid_start = (ISP_OB_VALID_START_REG_t *) (isp_reg_base + ISP_OB_VALID_START_REG_OFF);

	isp_regs[id].isp_wdr_exp_addr0 = (unsigned int *) (isp_reg_base + ISP_WDR_EXP_ADDR0_REG);
	isp_regs[id].isp_wdr_exp_addr1 = (unsigned int *) (isp_reg_base + ISP_WDR_EXP_ADDR1_REG);
	isp_regs[id].isp_d3d_rec_addr0 = (unsigned int *) (isp_reg_base + ISP_D3D_REC_ADDR0_REG);
	isp_regs[id].isp_d3d_rec_addr1 = (unsigned int *) (isp_reg_base + ISP_D3D_REC_ADDR1_REG);
#endif
}

/*
 * Load DRAM Register Address
 */

void bsp_isp_map_load_dram_addr(unsigned long id, unsigned long isp_load_dram_base)
{
#ifndef USE_DEF_PARA
	isp_regs[id].isp_update_flag = (ISP_FE_CTRL_REG_t *) (isp_load_dram_base + ISP_FE_CTRL_REG_OFF);
	isp_regs[id].isp_en = (ISP_EN_REG_t *) (isp_load_dram_base + ISP_EN_REG_OFF);
	isp_regs[id].isp_mode = (ISP_MODE_REG_t *) (isp_load_dram_base + ISP_MODE_REG_OFF);
	isp_regs[id].isp_ob_size = (ISP_OB_SIZE_REG_t *) (isp_load_dram_base + ISP_OB_SIZE_REG_OFF);
	isp_regs[id].isp_ob_valid = (ISP_OB_VALID_REG_t *) (isp_load_dram_base + ISP_OB_VALID_REG_OFF);
	isp_regs[id].isp_ob_valid_start = (ISP_OB_VALID_START_REG_t *) (isp_load_dram_base + ISP_OB_VALID_START_REG_OFF);

	isp_regs[id].isp_wdr_exp_addr0 = (unsigned int *) (isp_load_dram_base + ISP_WDR_EXP_ADDR0_REG);
	isp_regs[id].isp_wdr_exp_addr1 = (unsigned int *) (isp_load_dram_base + ISP_WDR_EXP_ADDR1_REG);
	isp_regs[id].isp_d3d_rec_addr0 = (unsigned int *) (isp_load_dram_base + ISP_D3D_REC_ADDR0_REG);
	isp_regs[id].isp_d3d_rec_addr1 = (unsigned int *) (isp_load_dram_base + ISP_D3D_REC_ADDR1_REG);
#endif
}

/*
 * Saved DRAM Register Address
 */
void bsp_isp_map_saved_dram_addr(unsigned long id, unsigned long base)
{

}

void bsp_isp_enable(unsigned long id, int enable)
{
	isp_regs[id].isp_fe_cfg->bits.isp_enable = enable;
}

void bsp_isp_ch_enable(unsigned long id, int ch, int enable)
{
	switch (ch) {
	case 0:
		isp_regs[id].isp_fe_cfg->bits.isp_ch0_en = enable;
		break;
	case 1:
		isp_regs[id].isp_fe_cfg->bits.isp_ch1_en = enable;
		break;
	case 2:
		isp_regs[id].isp_fe_cfg->bits.isp_ch2_en = enable;
		break;
	case 3:
		isp_regs[id].isp_fe_cfg->bits.isp_ch3_en = enable;
		break;
	default:
		isp_regs[id].isp_fe_cfg->bits.isp_ch0_en = enable;
		break;
	}
}

void bsp_isp_wdr_ch_seq(unsigned long id, int seq)
{
	switch (seq) {
	case 0:
		isp_regs[id].isp_fe_cfg->bits.wdr_ch_seq = 0;
		break;
	case 1:
		isp_regs[id].isp_fe_cfg->bits.wdr_ch_seq = 1;
		break;
	default:
		isp_regs[id].isp_fe_cfg->bits.wdr_ch_seq = 0;
		break;
	}
}
void bsp_isp_set_para_ready(unsigned long id, enum ready_flag ready)
{
#ifndef USE_DEF_PARA
	if (ready == PARA_READY)
		isp_regs[id].isp_fe_ctrl->bits.para_ready = 1;
	else
		isp_regs[id].isp_fe_ctrl->bits.para_ready = 0;
#endif
}

unsigned int bsp_isp_get_para_ready(unsigned long id)
{
	return isp_regs[id].isp_fe_ctrl->bits.para_ready;
}

void bsp_isp_update_table(unsigned long id, unsigned short table_update)
{
	if (table_update & LINEAR_UPDATE)
		isp_regs[id].isp_fe_ctrl->bits.linear_update = 1;
	else
		isp_regs[id].isp_fe_ctrl->bits.linear_update = 0;

	if (table_update & LENS_UPDATE)
		isp_regs[id].isp_fe_ctrl->bits.lens_update = 1;
	else
		isp_regs[id].isp_fe_ctrl->bits.lens_update = 0;

	if (table_update & GAMMA_UPDATE)
		isp_regs[id].isp_fe_ctrl->bits.gamma_update = 1;
	else
		isp_regs[id].isp_fe_ctrl->bits.gamma_update = 0;

	if (table_update & DRC_UPDATE)
		isp_regs[id].isp_fe_ctrl->bits.drc_update = 1;
	else
		isp_regs[id].isp_fe_ctrl->bits.drc_update = 0;

	if (table_update & DISC_UPDATE)
		isp_regs[id].isp_fe_ctrl->bits.disc_update = 1;
	else
		isp_regs[id].isp_fe_ctrl->bits.disc_update = 0;

	if (table_update & SATU_UPDATE)
		isp_regs[id].isp_fe_ctrl->bits.satu_update = 1;
	else
		isp_regs[id].isp_fe_ctrl->bits.satu_update = 0;

	if (table_update & WDR_UPDATE)
		isp_regs[id].isp_fe_ctrl->bits.wdr_update = 1;
	else
		isp_regs[id].isp_fe_ctrl->bits.wdr_update = 0;

	if (table_update & TDNF_UPDATE)
		isp_regs[id].isp_fe_ctrl->bits.tdnf_update = 1;
	else
		isp_regs[id].isp_fe_ctrl->bits.tdnf_update = 0;

	if (table_update & PLTM_UPDATE)
		isp_regs[id].isp_fe_ctrl->bits.pltm_update = 1;
	else
		isp_regs[id].isp_fe_ctrl->bits.pltm_update = 0;

	if (table_update & CEM_UPDATE)
		isp_regs[id].isp_fe_ctrl->bits.cem_update = 1;
	else
		isp_regs[id].isp_fe_ctrl->bits.cem_update = 0;

	if (table_update & CONTRAST_UPDATE)
		isp_regs[id].isp_fe_ctrl->bits.contrast_update = 1;
	else
		isp_regs[id].isp_fe_ctrl->bits.contrast_update = 0;
}
void bsp_isp_capture_start(unsigned long id)
{
	isp_regs[id].isp_fe_ctrl->bits.cap_en = 1;
}

void bsp_isp_capture_stop(unsigned long id)
{
	isp_regs[id].isp_fe_ctrl->bits.cap_en = 0;
}

void bsp_isp_irq_enable(unsigned long id, unsigned int irq_flag)
{
	isp_regs[id].isp_fe_int_en->dwval |= irq_flag;
}

void bsp_isp_irq_disable(unsigned long id, unsigned int irq_flag)
{
	isp_regs[id].isp_fe_int_en->dwval &= ~irq_flag;
}

int bsp_isp_int_get_enable(unsigned long id)
{
	return isp_regs[id].isp_fe_int_en->dwval;
}

unsigned int bsp_isp_get_irq_status(unsigned long id, unsigned int irq_flag)
{
	return isp_regs[id].isp_fe_int_sta->dwval & irq_flag;
}

void bsp_isp_clr_irq_status(unsigned long id, unsigned int irq_flag)
{
	isp_regs[id].isp_fe_int_sta->dwval = irq_flag;
}

void bsp_isp_debug_output_cfg(unsigned long id, int enable, int output_sel)
{
	isp_regs[id].isp_dbg_output->bits.debug_en = enable;
	isp_regs[id].isp_dbg_output->bits.debug_sel = output_sel;
}

void bsp_isp_set_para_ready_mode(unsigned long id, int enable)
{
}

void bsp_isp_set_line_int_num(unsigned long id, unsigned int line_num)
{
	isp_regs[id].isp_line_int_num->bits.line_int_num = line_num;
}
void bsp_isp_set_last_blank_cycle(unsigned long id, unsigned int last_blank_cycle)
{
	isp_regs[id].isp_line_int_num->bits.last_blank_cycle = last_blank_cycle;
}

void bsp_isp_set_speed_mode(unsigned long id, unsigned int speed_mode)
{
	isp_regs[id].isp_rot_of_cfg->bits.speed_mode = speed_mode;
}

void bsp_isp_set_load_addr(unsigned long id, unsigned long addr)
{
	isp_regs[id].isp_reg_load_addr->dwval = addr >> ISP_ADDR_BIT_R_SHIFT;
}

void bsp_isp_set_saved_addr(unsigned long id, unsigned long addr)
{
	isp_regs[id].isp_reg_saved_addr->dwval = addr >> ISP_ADDR_BIT_R_SHIFT;
}

void bsp_isp_set_table_addr(unsigned long id, enum isp_input_tables table,
				      unsigned long addr)
{
	switch (table) {
	case LENS_GAMMA_TABLE:
		isp_regs[id].isp_lut_lens_gamma_addr->dwval = addr >> ISP_ADDR_BIT_R_SHIFT;
		break;
	case DRC_TABLE:
		isp_regs[id].isp_drc_addr->dwval = addr >> ISP_ADDR_BIT_R_SHIFT;
		break;
	default:
		break;
	}
}

void bsp_isp_set_statistics_addr(unsigned long id, unsigned long addr)
{
	isp_regs[id].isp_statistics_addr->dwval = addr >> ISP_ADDR_BIT_R_SHIFT;
}

unsigned int bsp_isp_get_isp_ver(unsigned long id, unsigned int *major, unsigned int *minor)
{
	*major = isp_regs[id].isp_ver_cfg->bits.major_ver;
	*minor = isp_regs[id].isp_ver_cfg->bits.minor_ver;
	return isp_regs[id].isp_ver_cfg->dwval;
}

void bsp_isp_ver_read_en(unsigned long id, unsigned int en)
{
	isp_regs[id].isp_fe_cfg->bits.isp_ver_read_en = en;
}

void bsp_isp_get_s0_ch_fmerr_cnt(unsigned long id, struct isp_size *size)
{
	int input = readl(isp_regs[id].isp_s0_fmerr_cnt);
	size->width = input & 0xffff;
	size->height = (input >> 16) & 0xffff;
}

void bsp_isp_get_s0_ch_hb_cnt(unsigned long id, unsigned int *hb_max, unsigned int *hb_min)
{
	int blank = readl(isp_regs[id].isp_s0_hb_cnt);
	*hb_max = (blank >> 16) & 0xffff;
	*hb_min = blank & 0xffff;
}

void bsp_isp_module_enable(unsigned long id, unsigned int modules)
{
	isp_regs[id].isp_en->dwval |= modules;
}

void bsp_isp_module_disable(unsigned long id, unsigned int modules)
{
	isp_regs[id].isp_en->dwval &= ~modules;
}

void bsp_isp_set_wdr_mode(unsigned long id, unsigned int wdr_mode)
{
	if (wdr_mode == 1) {/* dol */
		isp_regs[id].isp_mode->bits.wdr_mode = 0;
		isp_regs[id].isp_mode->bits.wdr_dol_mode = 1;
	} else if (wdr_mode == 2) {/* comanding */
		isp_regs[id].isp_mode->bits.wdr_mode = 1;
		isp_regs[id].isp_mode->bits.wdr_dol_mode = 1;
	} else {
		isp_regs[id].isp_mode->bits.wdr_mode = 0;
		isp_regs[id].isp_mode->bits.wdr_dol_mode = 0;
	}
}

void bsp_isp_set_input_fmt(unsigned long id, unsigned int fmt)
{
	isp_regs[id].isp_mode->bits.input_fmt = fmt;
}

void bsp_isp_set_ob(unsigned long id, struct isp_size *black,
		struct isp_size *valid, struct coor *xy)
{
	isp_regs[id].isp_ob_size->bits.ob_width = black->width;
	isp_regs[id].isp_ob_size->bits.ob_height = black->height;
	isp_regs[id].isp_ob_valid->bits.ob_valid_width = valid->width;
	isp_regs[id].isp_ob_valid->bits.ob_valid_height = valid->height;
	isp_regs[id].isp_ob_valid_start->bits.ob_hor_start = xy->hor;
	isp_regs[id].isp_ob_valid_start->bits.ob_ver_start = xy->ver;
}

void bsp_isp_set_size(unsigned long id, struct isp_size_settings *size)
{
	bsp_isp_set_ob(id, &size->ob_black, &size->ob_valid, &size->ob_start);
}

unsigned int bsp_isp_load_update_flag(unsigned long id)
{
	return isp_regs[id].isp_update_flag->dwval;
}

void bsp_isp_set_wdr_addr0(unsigned long id, dma_addr_t addr)
{
	writel(addr >> ISP_ADDR_BIT_R_SHIFT, isp_regs[id].isp_wdr_exp_addr0);
}

void bsp_isp_set_wdr_addr1(unsigned long id, dma_addr_t addr)
{
	writel(addr >> ISP_ADDR_BIT_R_SHIFT, isp_regs[id].isp_wdr_exp_addr1);
}

void bsp_isp_set_d3d_addr0(unsigned long id, dma_addr_t addr)
{
	writel(addr >> ISP_ADDR_BIT_R_SHIFT, isp_regs[id].isp_d3d_rec_addr0);
}

void bsp_isp_set_d3d_addr1(unsigned long id, dma_addr_t addr)
{
	writel(addr >> ISP_ADDR_BIT_R_SHIFT, isp_regs[id].isp_d3d_rec_addr1);
}

void bsp_isp_set_fifo_mode(unsigned long id, unsigned int mode)
{
}

void bsp_isp_min_ddr_size(unsigned long id, unsigned int size)
{
}

void bsp_isp_fifo_raw_write(unsigned long id, unsigned int depth)
{
}

void bsp_isp_k_min_ddr_size(unsigned long id, unsigned int size)
{
}
