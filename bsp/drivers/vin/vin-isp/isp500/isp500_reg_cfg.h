/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright(c) 2020 - 2023 Allwinner Technology Co.,Ltd. All rights reserved. */

/* SPDX-License-Identifier: GPL-2.0 */
/*
  * isp500_reg_cfg.h
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

#ifndef _ISP500_REG_CFG_H_
#define _ISP500_REG_CFG_H_

#define ISP_ADDR_BIT_R_SHIFT 2

#define ISP_LOAD_DRAM_SIZE			(0x400+0x5a00+0x1f00)
#define ISP_SAVE_DRAM_SIZE			(0x400+0xab00)

/* ISP size configs */
#define ISP_REG_SIZE			0x400
#define ISP_LOAD_REG_SIZE		0x400
#define ISP_SAVED_REG_SIZE		0x400

/* stat size configs */

#define ISP_STAT_TOTAL_SIZE		0xAB00

#define ISP_STAT_HIST_MEM_SIZE      0x0200
#define ISP_STAT_AE_MEM_SIZE        0x4800
#define ISP_STAT_AF_MEM_SIZE        0x0500
#define ISP_STAT_AFS_MEM_SIZE       0x0200
#define ISP_STAT_AWB_RGB_MEM_SIZE   0x4800
#define ISP_STAT_AWB_CNT_MEM_SIZE   0x0C00
#define ISP_STAT_PLTM_LST_MEM_SIZE  0x0600

#define ISP_STAT_HIST_MEM_OFS       0x0000
#define ISP_STAT_AE_MEM_OFS         0x0200
#define ISP_STAT_AF_MEM_OFS         0x4a00
#define ISP_STAT_AFS_MEM_OFS        0x4f00
#define ISP_STAT_AWB_MEM_OFS        0x5100
#define ISP_STAT_PLTM_LST_MEM_OFS   0xa500

/* table size configs */

#define ISP_TABLE_MAPPING1_SIZE		0x5a00
#define ISP_LSC_MEM_SIZE		(256*8)
#define ISP_GAMMA_MEM_SIZE		(256*4)
#define ISP_LINEAR_MEM_SIZE		(256*6)
#define ISP_WDR_GAMMA_FE_MEM_SIZE	(4096*2)
#define ISP_WDR_GAMMA_BE_MEM_SIZE	(4096*2)
#define ISP_TDNF_DIFF_MEM_SIZE		(256*1)
#define ISP_PLTM_H_MEM_SIZE		(256*1)
#define ISP_PLTM_V_MEM_SIZE		(256*1)
#define ISP_PLTM_POW_MEM_SIZE		(256*2)
#define ISP_PLTM_F_MEM_SIZE		(256*2)
#define ISP_CONTRAST_PE_MEM_SIZE	(128*2)

#define ISP_TABLE_MAPPING2_SIZE		0x1f00
#define ISP_DRC_MEM_SIZE		(256*2)
#define ISP_SATURATION_MEM_SIZE		(256*2)
#define ISP_CEM_MEM_SIZE		(736*8)

#define ISP_LSC_MEM_OFS			0x0
#define ISP_GAMMA_MEM_OFS		0x0800
#define ISP_LINEAR_MEM_OFS		0x0c00
#define ISP_WDR_GAMMA_FE_MEM_OFS	0x1200
#define ISP_WDR_GAMMA_BE_MEM_OFS	0x3200
#define ISP_TDNF_DIFF_MEM_OFS		0x5200
#define ISP_PLTM_H_MEM_OFS		0x5300
#define ISP_PLTM_V_MEM_OFS		0x5400
#define ISP_PLTM_POW_MEM_OFS		0x5500
#define ISP_PLTM_F_MEM_OFS		0x5700
#define ISP_CONTRAST_PE_MEM_OFS		0x5900

#define ISP_DRC_MEM_OFS			0x0
#define ISP_SATURATION_MEM_OFS		0x0600
#define ISP_CEM_MEM_OFS			0x0800

/*
 *  update table
 */
#define LUT_UPDATE	(1 << 3)
#define LINEAR_UPDATE	(1 << 3)
#define LENS_UPDATE	(1 << 4)
#define GAMMA_UPDATE	(1 << 5)
#define DRC_UPDATE	(1 << 6)
#define DISC_UPDATE	(1 << 7)
#define D3D_UPDATE	(1 << 7)
#define SATU_UPDATE	(1 << 8)
#define WDR_UPDATE	(1 << 9)
#define TDNF_UPDATE	(1 << 10)
#define PLTM_UPDATE	(1 << 11)
#define CEM_UPDATE	(1 << 12)
#define CONTRAST_UPDATE	(1 << 13)

#define TABLE_UPDATE_ALL 0xffffffff

/*
 *  ISP Module enable
 */
#define AE_EN		(1 << 0)
#define LC_EN           (1 << 1)
#define WDR_EN          (1 << 2)
#define DPC_EN      	(1 << 3)
#define D2D_EN		(1 << 4)
#define D3D_EN		(1 << 5)
#define AWB_EN		(1 << 6)
#define WB_EN           (1 << 7)
#define LSC_EN          (1 << 8)
#define BGC_EN          (1 << 9)
#define SHARP_EN	(1 << 10)
#define AF_EN           (1 << 11)
#define RGB2RGB_EN      (1 << 12)
#define RGB_DRC_EN      (1 << 13)
#define PLTM_EN         (1 << 14)
#define CEM_EN          (1 << 15)
#define AFS_EN          (1 << 16)
#define HIST_EN         (1 << 17)
#define BLC_EN          (1 << 18)
#define DG_EN           (1 << 19)
#define SO_EN           (1 << 20)
#define CTC_EN          (1 << 21)
#define CONTRAST_EN     (1 << 22)
#define CNR_EN          (1 << 23)
#define SATU_EN		(1 << 24)
#define SRC0_EN         (1 << 31)
#define ISP_MODULE_EN_ALL	(0xffffffff)

/*
 *  ISP interrupt enable
 */
#define FINISH_INT_EN		(1 << 0)
#define START_INT_EN		(1 << 1)
#define PARA_SAVE_INT_EN	(1 << 2)
#define PARA_LOAD_INT_EN	(1 << 3)
#define SRC0_FIFO_INT_EN	(1 << 4)
#define N_LINE_START_INT_EN	(1 << 7)
#define FRAME_ERROR_INT_EN	(1 << 8)
#define FRAME_LOST_INT_EN	(1 << 14)

#define ISP_IRQ_EN_ALL	0xffffffff

/*
 *  ISP interrupt status
 */
#define FINISH_PD	(1 << 0)
#define START_PD	(1 << 1)
#define PARA_SAVE_PD	(1 << 2)
#define PARA_LOAD_PD	(1 << 3)
#define SRC0_FIFO_OF_PD	(1 << 4)
#define N_LINE_START_PD	(1 << 7)
#define FRAME_ERROR_PD	(1 << 8)
#define CIN_FIFO_OF_PD	(1 << 9)
#define DPC_FIFO_OF_PD	(1 << 10)
#define D2D_FIFO_OF_PD	(1 << 11)
#define BIS_FIFO_OF_PD	(1 << 12)
#define CNR_FIFO_OF_PD	(1 << 13)
#define FRAME_LOST_PD	(1 << 14)
#define HB_SHORT_PD	(1 << 16)
#define D3D_HB_PD	(1 << 24)
#define PLTM_FIFO_OF_PD	(1 << 25)
#define D3D_WRITE_FIFO_OF_PD	(1 << 26)
#define D3D_READ_FIFO_OF_PD	(1 << 27)
#define D3D_WT2CMP_FIFO_OF_PD	(1 << 28)
#define WDR_WRITE_FIFO_OF_PD	(1 << 29)
#define WDR_WT2CMP_FIFO_OF_PD	(1 << 30)
#define WDR_READ_FIFO_OF_PD	(1 << 31)

#define ISP_IRQ_STATUS_ALL	0xffffffff

enum isp_channel {
	ISP_CH0 = 0,
	ISP_CH1 = 1,
	ISP_CH2 = 2,
	ISP_CH3 = 3,
	ISP_MAX_CH_NUM,
};

struct isp_size {
	unsigned int width;
	unsigned int height;
};

struct coor {
	unsigned int hor;
	unsigned int ver;
};

struct isp_size_settings {
	struct coor ob_start;
	struct isp_size ob_black;
	struct isp_size ob_valid;
	u32 set_cnt;
};

enum ready_flag {
	PARA_NOT_READY = 0,
	PARA_READY = 1,
};

enum enable_flag {
	DISABLE = 0,
	ENABLE = 1,
};

enum isp_input_tables {
	LENS_GAMMA_TABLE = 0,
	DRC_TABLE = 1,
};

enum isp_input_seq {
	ISP_BGGR = 4,
	ISP_RGGB = 5,
	ISP_GBRG = 6,
	ISP_GRBG = 7,
};

enum isp_output_speed {
	ISP_OUTPUT_SPEED_0 = 0,
	ISP_OUTPUT_SPEED_1 = 1,
	ISP_OUTPUT_SPEED_2 = 2,
	ISP_OUTPUT_SPEED_3 = 3,
};

enum isp_debug_sel {
	ISP_DBG_OBC0_INPUT = 0,
	ISP_DBG_OBC0_OUTPUT,
	ISP_DBG_OBC1_OUTPUT,
	ISP_DBG_WDR_OUTPUT,
	ISP_DBG_DPC_OUTPUT,
	ISP_DBG_D2D_OUTPUT,
	ISP_DBG_D3D_OUTPUT,
	ISP_DBG_D3D_Y_OUTPUT,
	ISP_DBG_D3D_B_OUTPUT,
	ISP_DBG_LSC_OUTPUT,
	ISP_DBG_PLTM_OUTPUT,
	ISP_DBG_CNR_R_OUTPUT,
	ISP_DBG_CNR_G_OUTPUT,
	ISP_DBG_CNR_B_OUTPUT,
	ISP_DBG_BGC_R_OUTPUT,
	ISP_DBG_BGC_G_OUTPUT,
	ISP_DBG_BGC_B_OUTPUT,
};

void bsp_isp_map_reg_addr(unsigned long id, unsigned long base);
void bsp_isp_map_load_dram_addr(unsigned long id, unsigned long base);
void bsp_isp_map_saved_dram_addr(unsigned long id, unsigned long base);
void bsp_isp_enable(unsigned long id, int enable);
void bsp_isp_ch_enable(unsigned long id, int ch, int enable);
void bsp_isp_wdr_ch_seq(unsigned long id, int seq);
void bsp_isp_set_para_ready(unsigned long id, enum ready_flag ready);
unsigned int bsp_isp_get_para_ready(unsigned long id);
void bsp_isp_update_table(unsigned long id, unsigned short table_update);
void bsp_isp_capture_start(unsigned long id);
void bsp_isp_capture_stop(unsigned long id);
void bsp_isp_irq_enable(unsigned long id, unsigned int irq_flag);
void bsp_isp_irq_disable(unsigned long id, unsigned int irq_flag);
int bsp_isp_int_get_enable(unsigned long id);
unsigned int bsp_isp_get_irq_status(unsigned long id, unsigned int irq_flag);
void bsp_isp_clr_irq_status(unsigned long id, unsigned int irq_flag);
void bsp_isp_debug_output_cfg(unsigned long id, int enable, int output_sel);
void bsp_isp_set_para_ready_mode(unsigned long id, int enable);
void bsp_isp_set_line_int_num(unsigned long id, unsigned int line_num);
void bsp_isp_set_last_blank_cycle(unsigned long id, unsigned int last_blank_cycle);
void bsp_isp_set_speed_mode(unsigned long id, unsigned int speed_mode);
void bsp_isp_set_load_addr(unsigned long id, unsigned long addr);
void bsp_isp_set_saved_addr(unsigned long id, unsigned long addr);
void bsp_isp_set_table_addr(unsigned long id, enum isp_input_tables table, unsigned long addr);
void bsp_isp_set_statistics_addr(unsigned long id, unsigned long addr);
unsigned int bsp_isp_get_isp_ver(unsigned long id, unsigned int *major, unsigned int *minor);
void bsp_isp_ver_read_en(unsigned long id, unsigned int en);
void bsp_isp_get_s0_ch_fmerr_cnt(unsigned long id, struct isp_size *size);
void bsp_isp_get_s0_ch_hb_cnt(unsigned long id, unsigned int *hb_max, unsigned int *hb_min);
void bsp_isp_module_enable(unsigned long id, unsigned int modules);
void bsp_isp_module_disable(unsigned long id, unsigned int modules);
void bsp_isp_set_wdr_mode(unsigned long id, unsigned int wdr_mode);
void bsp_isp_set_input_fmt(unsigned long id, unsigned int fmt);
void bsp_isp_set_size(unsigned long id, struct isp_size_settings *size);
unsigned int bsp_isp_load_update_flag(unsigned long id);

void bsp_isp_set_wdr_addr0(unsigned long id, dma_addr_t addr);
void bsp_isp_set_wdr_addr1(unsigned long id, dma_addr_t addr);
void bsp_isp_set_d3d_addr0(unsigned long id, dma_addr_t addr);
void bsp_isp_set_d3d_addr1(unsigned long id, dma_addr_t addr);
void bsp_isp_set_fifo_mode(unsigned long id, unsigned int mode);
void bsp_isp_min_ddr_size(unsigned long id, unsigned int size);
void bsp_isp_fifo_raw_write(unsigned long id, unsigned int depth);
void bsp_isp_k_min_ddr_size(unsigned long id, unsigned int size);

#endif /* _ISP500_REG_CFG_H_ */
