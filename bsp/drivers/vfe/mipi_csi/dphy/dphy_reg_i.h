/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright(c) 2020 - 2023 Allwinner Technology Co.,Ltd. All rights reserved. */
/*
 * linux-4.9/drivers/media/platform/sunxi-vfe/mipi_csi/dphy/dphy_reg_i.h
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


#ifndef __DPHY_REG_I_H__
#define __DPHY_REG_I_H__

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------------ */
/*  */
/* ------------------------------------------------------------------------------ */


/*  */
/* Register Offset */
/*  */
#define DPHY_CTL_REG_OFF          0x000
#define DPHY_TX_CTL_REG_OFF       0x004
#define DPHY_RX_CTL_REG_OFF       0x008
#define DPHY_TX_TIME0_REG_OFF     0x010
#define DPHY_TX_TIME1_REG_OFF     0x014
#define DPHY_TX_TIME2_REG_OFF     0x018
#define DPHY_TX_TIME3_REG_OFF     0x01C
#define DPHY_TX_TIME4_REG_OFF     0x020
#define DPHY_RX_TIME0_REG_OFF     0x030
#define DPHY_RX_TIME1_REG_OFF     0x034
#define DPHY_RX_TIME2_REG_OFF     0x038
#define DPHY_RX_TIME3_REG_OFF     0x040
#define DPHY_ANA0_REG_OFF         0x04C
#define DPHY_ANA1_REG_OFF         0x050
#define DPHY_ANA2_REG_OFF         0x054
#define DPHY_ANA3_REG_OFF         0x058
#define DPHY_ANA4_REG_OFF         0x05C
#define DPHY_INT_EN0_REG_OFF      0x060
#define DPHY_INT_EN1_REG_OFF      0x064
#define DPHY_INT_EN2_REG_OFF      0x068
#define DPHY_INT_PD0_REG_OFF      0x070
#define DPHY_INT_PD1_REG_OFF      0x074
#define DPHY_INT_PD2_REG_OFF      0x078
#define DPHY_DBG0_REG_OFF         0x0E0
#define DPHY_DBG1_REG_OFF         0x0E4
#define DPHY_DBG2_REG_OFF         0x0E8
#define DPHY_DBG3_REG_OFF         0x0EC
#define DPHY_DBG4_REG_OFF         0x0F0
#define DPHY_DBG5_REG_OFF         0x0F4

/*  */
/* Register Address */
/*  */
#define DPHY_CTL_REG_ADDR         (DPHY_VBASE + DPHY_CTL_REG_OFF)    /* DPHY Control Register */
#define DPHY_TX_CTL_REG_ADDR      (DPHY_VBASE + DPHY_TX_CTL_REG_OFF)    /* DPHY TX Control Register */
#define DPHY_RX_CTL_REG_ADDR      (DPHY_VBASE + DPHY_RX_CTL_REG_OFF)    /* DPHY RX Control Register */
#define DPHY_TX_TIME0_REG_ADDR    (DPHY_VBASE + DPHY_TX_TIME0_REG_OFF)    /* DPHY TX Timing parameter 0 Register */
#define DPHY_TX_TIME1_REG_ADDR    (DPHY_VBASE + DPHY_TX_TIME1_REG_OFF)    /* DPHY TX Timing parameter 1 Register */
#define DPHY_TX_TIME2_REG_ADDR    (DPHY_VBASE + DPHY_TX_TIME2_REG_OFF)    /* DPHY TX Timing parameter 2 Register */
#define DPHY_TX_TIME3_REG_ADDR    (DPHY_VBASE + DPHY_TX_TIME3_REG_OFF)    /* DPHY TX Timing parameter 3 Register */
#define DPHY_TX_TIME4_REG_ADDR    (DPHY_VBASE + DPHY_TX_TIME4_REG_OFF)    /* DPHY TX Timing parameter 4 Register */
#define DPHY_RX_TIME0_REG_ADDR    (DPHY_VBASE + DPHY_RX_TIME0_REG_OFF)    /* DPHY RX Timing parameter 0 Register */
#define DPHY_RX_TIME1_REG_ADDR    (DPHY_VBASE + DPHY_RX_TIME1_REG_OFF)    /* DPHY RX Timing parameter 1 Register */
#define DPHY_RX_TIME2_REG_ADDR    (DPHY_VBASE + DPHY_RX_TIME2_REG_OFF)    /* DPHY RX Timing parameter 2 Register */
#define DPHY_RX_TIME3_REG_ADDR    (DPHY_VBASE + DPHY_RX_TIME3_REG_OFF)    /* DPHY RX Timing parameter 3 Register */
#define DPHY_ANA0_REG_ADDR        (DPHY_VBASE + DPHY_ANA0_REG_OFF)    /* DPHY Analog 0 Register */
#define DPHY_ANA1_REG_ADDR        (DPHY_VBASE + DPHY_ANA1_REG_OFF)    /* DPHY Analog 1 Register */
#define DPHY_ANA2_REG_ADDR        (DPHY_VBASE + DPHY_ANA2_REG_OFF)    /* DPHY Analog 2 Register */
#define DPHY_ANA3_REG_ADDR        (DPHY_VBASE + DPHY_ANA3_REG_OFF)    /* DPHY Analog 3 Register */
#define DPHY_ANA4_REG_ADDR        (DPHY_VBASE + DPHY_ANA4_REG_OFF)    /* DPHY Analog 4 Register */
#define DPHY_INT_EN0_REG_ADDR     (DPHY_VBASE + DPHY_INT_EN0_REG_OFF)    /* DPHY Interrupt Enable 0 Register */
#define DPHY_INT_EN1_REG_ADDR     (DPHY_VBASE + DPHY_INT_EN1_REG_OFF)    /* DPHY Interrupt Enable 1 Register */
#define DPHY_INT_EN2_REG_ADDR     (DPHY_VBASE + DPHY_INT_EN2_REG_OFF)    /* DPHY Interrupt Enable 2 Register */
#define DPHY_INT_PD0_REG_ADDR     (DPHY_VBASE + DPHY_INT_PD0_REG_OFF)    /* DPHY Interrupt Pending 0 Register */
#define DPHY_INT_PD1_REG_ADDR     (DPHY_VBASE + DPHY_INT_PD1_REG_OFF)    /* DPHY Interrupt Pending 1 Register */
#define DPHY_INT_PD2_REG_ADDR     (DPHY_VBASE + DPHY_INT_PD2_REG_OFF)    /* DPHY Interrupt Pending 2 Register */
#define DPHY_DBG0_REG_ADDR        (DPHY_VBASE + DPHY_DBG0_REG_OFF)    /* DPHY Debug 0 Register */
#define DPHY_DBG1_REG_ADDR        (DPHY_VBASE + DPHY_DBG1_REG_OFF)    /* DPHY Debug 1 Register */
#define DPHY_DBG2_REG_ADDR        (DPHY_VBASE + DPHY_DBG2_REG_OFF)    /* DPHY Debug 2 Register */
#define DPHY_DBG3_REG_ADDR        (DPHY_VBASE + DPHY_DBG3_REG_OFF)    /* DPHY Debug 3 Register */
#define DPHY_DBG4_REG_ADDR        (DPHY_VBASE + DPHY_DBG4_REG_OFF)    /* DPHY Debug 4 Register */
#define DPHY_DBG5_REG_ADDR        (DPHY_VBASE + DPHY_DBG5_REG_OFF)    /* DPHY Debug 5 Register */

/*  */
/* Detail information of registers */
/*  */

typedef union {
	unsigned int dwval;
	struct {
		unsigned int module_en           :  1;    /* Default: 0; */
		unsigned int res0                :  3;    /* Default:; */
		unsigned int lane_num            :  2;    /* Default: 0; */
		unsigned int res1                : 26;    /* Default:; */
	} bits;
} DPHY_CTL_REG_t;

typedef union {
	unsigned int dwval;
	struct {
		unsigned int tx_d0_force         :  1;    /* Default: 0; */
		unsigned int tx_d1_force         :  1;    /* Default: 0; */
		unsigned int tx_d2_force         :  1;    /* Default: 0; */
		unsigned int tx_d3_force         :  1;    /* Default: 0; */
		unsigned int tx_clk_force        :  1;    /* Default: 0; */
		unsigned int res0                :  3;    /* Default:; */
		unsigned int lptx_endian         :  1;    /* Default: 0; */
		unsigned int hstx_endian         :  1;    /* Default: 0; */
		unsigned int lptx_8b9b_en        :  1;    /* Default: 0; */
		unsigned int hstx_8b9b_en        :  1;    /* Default: 0; */
		unsigned int force_lp11          :  1;    /* Default: 0; */
		unsigned int res1                :  3;    /* Default:; */
		unsigned int ulpstx_data0_exit   :  1;    /* Default: 0; */
		unsigned int ulpstx_data1_exit   :  1;    /* Default: 0; */
		unsigned int ulpstx_data2_exit   :  1;    /* Default: 0; */
		unsigned int ulpstx_data3_exit   :  1;    /* Default: 0; */
		unsigned int ulpstx_clk_exit     :  1;    /* Default: 0; */
		unsigned int res2                :  3;    /* Default:; */
		unsigned int hstx_data_exit      :  1;    /* Default: 0; */
		unsigned int hstx_clk_exit       :  1;    /* Default: 0; */
		unsigned int res3                :  2;    /* Default:; */
		unsigned int hstx_clk_cont       :  1;    /* Default: 0; */
		unsigned int ulpstx_enter        :  1;    /* Default: 0; */
		unsigned int res4                :  2;    /* Default:; */
	} bits;
} DPHY_TX_CTL_REG_t;

typedef union {
	unsigned int dwval;
	struct {
		unsigned int res0                :  8;    /* Default:; */
		unsigned int lprx_endian         :  1;    /* Default: 0; */
		unsigned int hsrx_endian         :  1;    /* Default: 0; */
		unsigned int lprx_8b9b_en        :  1;    /* Default: 0; */
		unsigned int hsrx_8b9b_en        :  1;    /* Default: 0; */
		unsigned int hsrx_sync           :  1;    /* Default: 0; */
		unsigned int res1                :  3;    /* Default:; */
		unsigned int lprx_trnd_mask      :  4;    /* Default: 0; */
		unsigned int rx_d0_force         :  1;    /* Default: 0; */
		unsigned int rx_d1_force         :  1;    /* Default: 0; */
		unsigned int rx_d2_force         :  1;    /* Default: 0; */
		unsigned int rx_d3_force         :  1;    /* Default: 0; */
		unsigned int rx_clk_force        :  1;    /* Default: 0; */
		unsigned int res2                :  6;    /* Default:; */
		unsigned int dbc_en              :  1;    /* Default: 0; */
	} bits;
} DPHY_RX_CTL_REG_t;

typedef union {
	unsigned int dwval;
	struct {
		unsigned int lpx_tm_set          :  8;    /* Default: 0; */
		unsigned int dterm_set           :  8;    /* Default: 0; */
		unsigned int hs_pre_set          :  8;    /* Default: 0; */
		unsigned int hs_trail_set        :  8;    /* Default: 0; */
	} bits;
} DPHY_TX_TIME0_REG_t;

typedef union {
	unsigned int dwval;
	struct {
		unsigned int ck_prep_set         :  8;    /* Default: 0; */
		unsigned int ck_zero_set         :  8;    /* Default: 0; */
		unsigned int ck_pre_set          :  8;    /* Default: 0; */
		unsigned int ck_post_set         :  8;    /* Default: 0; */
	} bits;
} DPHY_TX_TIME1_REG_t;

typedef union {
	unsigned int dwval;
	struct {
		unsigned int ck_trail_set        :  8;    /* Default: 0; */
		unsigned int hs_dly_set          : 16;    /* Default: 0; */
		unsigned int res0                :  4;    /* Default:; */
		unsigned int hs_dly_mode         :  1;    /* Default: 0; */
		unsigned int res1                :  3;    /* Default:; */
	} bits;
} DPHY_TX_TIME2_REG_t;

typedef union {
	unsigned int dwval;
	struct {
		unsigned int lptx_ulps_exit_set  : 20;    /* Default: 0; */
		unsigned int res0                : 12;    /* Default:; */
	} bits;
} DPHY_TX_TIME3_REG_t;

typedef union {
	unsigned int dwval;
	struct {
		unsigned int hstx_ana0_set       :  8;    /* Default: 1; */
		unsigned int hstx_ana1_set       :  8;    /* Default: 1; */
		unsigned int res0                : 16;    /* Default:; */
	} bits;
} DPHY_TX_TIME4_REG_t;

typedef union {
	unsigned int dwval;
	struct {
		unsigned int lprx_to_en          :  1;    /* Default: 0; */
		unsigned int freq_cnt_en         :  1;    /* Default: 0; */
		unsigned int res0                :  2;    /* Default:; */
		unsigned int hsrx_clk_miss_en    :  1;    /* Default: 0; */
		unsigned int hsrx_sync_err_to_en :  1;    /* Default: 0; */
		unsigned int res1                :  2;    /* Default:; */
		unsigned int lprx_to             :  8;    /* Default: 0; */
		unsigned int hsrx_clk_miss       :  8;    /* Default: 0; */
		unsigned int hsrx_sync_err_to    :  8;    /* Default: 0; */
	} bits;
} DPHY_RX_TIME0_REG_t;

typedef union {
	unsigned int dwval;
	struct {
		unsigned int lprx_ulps_wp        : 20;    /* Default: 0; */
		unsigned int rx_dly              : 12;    /* Default: 0; */
	} bits;
} DPHY_RX_TIME1_REG_t;

typedef union {
	unsigned int dwval;
	struct {
		unsigned int hsrx_ana0_set       :  8;    /* Default: 1; */
		unsigned int hsrx_ana1_set       :  8;    /* Default: 1; */
		unsigned int res0                : 16;    /* Default:; */
	} bits;
} DPHY_RX_TIME2_REG_t;

typedef union {
	unsigned int dwval;
	struct {
		unsigned int freq_cnt            : 16;    /* Default: 0; */
		unsigned int lprst_dly           : 16;    /* Default: 0; */
	} bits;
} DPHY_RX_TIME3_REG_t;

typedef union {
	unsigned int dwval;
	struct {
		unsigned int reg_selsck          :  1;    /* Default: 0; */
		unsigned int reg_rsd             :  1;    /* Default: 0; */
		unsigned int reg_sfb             :  2;    /* Default: 0; */
		unsigned int reg_plr             :  4;    /* Default: 0; */
		unsigned int reg_den             :  4;    /* Default: 0; */
		unsigned int reg_slv             :  3;    /* Default: 0; */
		unsigned int reg_sdiv2           :  1;    /* Default: 0; */
		unsigned int reg_srxck           :  4;    /* Default: 0; */
		unsigned int reg_srxdt           :  4;    /* Default: 0; */
		unsigned int reg_dmp             :  4;    /* Default: 0; */
		unsigned int reg_dmpc            :  1;    /* Default: 0; */
		unsigned int reg_pwenc           :  1;    /* Default: 0; */
		unsigned int reg_pwend           :  1;    /* Default: 0; */
		unsigned int reg_pws             :  1;    /* Default: 0; */
	} bits;
} DPHY_ANA0_REG_t;

typedef union {
	unsigned int dwval;
	struct {
		unsigned int reg_stxck           :  1;    /* Default: 0; */
		unsigned int res0                :  3;    /* Default:; */
		unsigned int reg_svdl0           :  4;    /* Default: 0; */
		unsigned int reg_svdl1           :  4;    /* Default: 0; */
		unsigned int reg_svdl2           :  4;    /* Default: 0; */
		unsigned int reg_svdl3           :  4;    /* Default: 0; */
		unsigned int reg_svdlc           :  4;    /* Default: 0; */
		unsigned int reg_svtt            :  4;    /* Default: 0; */
		unsigned int reg_csmps           :  2;    /* Default: 0; */
		unsigned int res1                :  1;    /* Default:; */
		unsigned int reg_vttmode          :  1;    /* Default: 0; */
	} bits;
} DPHY_ANA1_REG_t;

typedef union {
	unsigned int dwval;
	struct {
		unsigned int ana_cpu_en          :  1;    /* Default: 0; */
		unsigned int enib                :  1;    /* Default: 0; */
		unsigned int enrvs               :  1;    /* Default: 0; */
		unsigned int res0                :  1;    /* Default:; */
		unsigned int enck_cpu            :  1;    /* Default: 0; */
		unsigned int entxc_cpu           :  1;    /* Default: 0; */
		unsigned int enckq_cpu           :  1;    /* Default: 0; */
		unsigned int res1                :  1;    /* Default:; */
		unsigned int entx_cpu            :  4;    /* Default: 0; */
		unsigned int res2                :  1;    /* Default:; */
		unsigned int entermc_cpu         :  1;    /* Default: 0; */
		unsigned int enrxc_cpu           :  1;    /* Default: 0; */
		unsigned int res3                :  1;    /* Default:; */
		unsigned int enterm_cpu          :  4;    /* Default: 0; */
		unsigned int enrx_cpu            :  4;    /* Default: 0; */
		unsigned int enp2s_cpu           :  4;    /* Default: 0; */
		unsigned int res4                :  4;    /* Default:; */
	} bits;
} DPHY_ANA2_REG_t;

typedef union {
	unsigned int dwval;
	struct {
		unsigned int enlptx_cpu          :  4;    /* Default: 0; */
		unsigned int enlprx_cpu          :  4;    /* Default: 0; */
		unsigned int enlpcd_cpu          :  4;    /* Default: 0; */
		unsigned int enlprxc_cpu         :  1;    /* Default: 0; */
		unsigned int enlptxc_cpu         :  1;    /* Default: 0; */
		unsigned int enlpcdc_cpu         :  1;    /* Default: 0; */
		unsigned int res0                :  1;    /* Default:; */
		unsigned int entest              :  1;    /* Default: 0; */
		unsigned int enckdbg             :  1;    /* Default: 0; */
		unsigned int enldor              :  1;    /* Default: 0; */
		unsigned int res1                :  5;    /* Default:; */
		unsigned int enldod              :  1;    /* Default: 0; */
		unsigned int enldoc              :  1;    /* Default: 0; */
		unsigned int endiv               :  1;    /* Default: 0; */
		unsigned int envttc              :  1;    /* Default: 0; */
		unsigned int envtt               :  4;    /* Default: 0; */
	} bits;
} DPHY_ANA3_REG_t;

typedef union {
	unsigned int dwval;
	struct {
		unsigned int reg_txpusd          :  2;    /* Default: 0; */
		unsigned int reg_txpusc          :  2;    /* Default: 0; */
		unsigned int reg_txdnsd          :  2;    /* Default: 0; */
		unsigned int reg_txdnsc          :  2;    /* Default: 0; */
		unsigned int reg_tmsd            :  2;    /* Default: 0; */
		unsigned int reg_tmsc            :  2;    /* Default: 0; */
		unsigned int reg_ckdv            :  5;    /* Default: 0; */
		unsigned int res0                :  3;    /* Default:; */
		unsigned int reg_dmplvd					 :	4;		 /*  */
		unsigned int reg_dmplvc					 :	1;		 /*  */
		unsigned int res1                :  7;    /* Default:; */
	} bits;
} DPHY_ANA4_REG_t;

typedef union {
	unsigned int dwval;
	struct {
		unsigned int sot_d0_int          :  1;    /* Default: 0x0; */
		unsigned int sot_d1_int          :  1;    /* Default: 0x0; */
		unsigned int sot_d2_int          :  1;    /* Default: 0x0; */
		unsigned int sot_d3_int          :  1;    /* Default: 0x0; */
		unsigned int sot_err_d0_int      :  1;    /* Default: 0x0; */
		unsigned int sot_err_d1_int      :  1;    /* Default: 0x0; */
		unsigned int sot_err_d2_int      :  1;    /* Default: 0x0; */
		unsigned int sot_err_d3_int      :  1;    /* Default: 0x0; */
		unsigned int sot_sync_err_d0_int :  1;    /* Default: 0x0; */
		unsigned int sot_sync_err_d1_int :  1;    /* Default: 0x0; */
		unsigned int sot_sync_err_d2_int :  1;    /* Default: 0x0; */
		unsigned int sot_sync_err_d3_int :  1;    /* Default: 0x0; */
		unsigned int rx_alg_err_d0_int   :  1;    /* Default: 0x0; */
		unsigned int rx_alg_err_d1_int   :  1;    /* Default: 0x0; */
		unsigned int rx_alg_err_d2_int   :  1;    /* Default: 0x0; */
		unsigned int rx_alg_err_d3_int   :  1;    /* Default: 0x0; */
		unsigned int res0                :  6;    /* Default:; */
		unsigned int cd_lp0_err_clk_int  :  1;    /* Default: 0x0; */
		unsigned int cd_lp1_err_clk_int  :  1;    /* Default: 0x0; */
		unsigned int cd_lp0_err_d0_int   :  1;    /* Default: 0x0; */
		unsigned int cd_lp1_err_d0_int   :  1;    /* Default: 0x0; */
		unsigned int cd_lp0_err_d1_int   :  1;    /* Default: 0x0; */
		unsigned int cd_lp1_err_d1_int   :  1;    /* Default: 0x0; */
		unsigned int cd_lp0_err_d2_int   :  1;    /* Default: 0x0; */
		unsigned int cd_lp1_err_d2_int   :  1;    /* Default: 0x0; */
		unsigned int cd_lp0_err_d3_int   :  1;    /* Default: 0x0; */
		unsigned int cd_lp1_err_d3_int   :  1;    /* Default: 0x0; */
	} bits;
} DPHY_INT_EN0_REG_t;

typedef union {
	unsigned int dwval;
	struct {
		unsigned int ulps_d0_int         :  1;    /* Default: 0x0; */
		unsigned int ulps_d1_int         :  1;    /* Default: 0x0; */
		unsigned int ulps_d2_int         :  1;    /* Default: 0x0; */
		unsigned int ulps_d3_int         :  1;    /* Default: 0x0; */
		unsigned int ulps_wp_d0_int      :  1;    /* Default: 0x0; */
		unsigned int ulps_wp_d1_int      :  1;    /* Default: 0x0; */
		unsigned int ulps_wp_d2_int      :  1;    /* Default: 0x0; */
		unsigned int ulps_wp_d3_int      :  1;    /* Default: 0x0; */
		unsigned int ulps_clk_int        :  1;    /* Default: 0x0; */
		unsigned int ulps_wp_clk_int     :  1;    /* Default: 0x0; */
		unsigned int res0                :  2;    /* Default:; */
		unsigned int lpdt_d0_int         :  1;    /* Default: 0x0; */
		unsigned int rx_trnd_d0_int      :  1;    /* Default: 0x0; */
		unsigned int tx_trnd_err_d0_int  :  1;    /* Default: 0x0; */
		unsigned int undef1_d0_int       :  1;    /* Default: 0x0; */
		unsigned int undef2_d0_int       :  1;    /* Default: 0x0; */
		unsigned int undef3_d0_int       :  1;    /* Default: 0x0; */
		unsigned int undef4_d0_int       :  1;    /* Default: 0x0; */
		unsigned int undef5_d0_int       :  1;    /* Default: 0x0; */
		unsigned int rst_d0_int          :  1;    /* Default: 0x0; */
		unsigned int rst_d1_int          :  1;    /* Default: 0x0; */
		unsigned int rst_d2_int          :  1;    /* Default: 0x0; */
		unsigned int rst_d3_int          :  1;    /* Default: 0x0; */
		unsigned int esc_cmd_err_d0_int  :  1;    /* Default: 0x0; */
		unsigned int esc_cmd_err_d1_int  :  1;    /* Default: 0x0; */
		unsigned int esc_cmd_err_d2_int  :  1;    /* Default: 0x0; */
		unsigned int esc_cmd_err_d3_int  :  1;    /* Default: 0x0; */
		unsigned int false_ctl_d0_int    :  1;    /* Default: 0x0; */
		unsigned int false_ctl_d1_int    :  1;    /* Default: 0x0; */
		unsigned int false_ctl_d2_int    :  1;    /* Default: 0x0; */
		unsigned int false_ctl_d3_int    :  1;    /* Default: 0x0; */
	} bits;
} DPHY_INT_EN1_REG_t;

typedef union {
	unsigned int dwval;
	struct {
		unsigned int res0;    /* Default:; */
	} bits;
} DPHY_INT_EN2_REG_t;

typedef union {
	unsigned int dwval;
	struct {
		unsigned int sot_d0_pd           :  1;    /* Default: 0x0; */
		unsigned int sot_d1_pd           :  1;    /* Default: 0x0; */
		unsigned int sot_d2_pd           :  1;    /* Default: 0x0; */
		unsigned int sot_d3_pd           :  1;    /* Default: 0x0; */
		unsigned int sot_err_d0_pd       :  1;    /* Default: 0x0; */
		unsigned int sot_err_d1_pd       :  1;    /* Default: 0x0; */
		unsigned int sot_err_d2_pd       :  1;    /* Default: 0x0; */
		unsigned int sot_err_d3_pd       :  1;    /* Default: 0x0; */
		unsigned int sot_sync_err_d0_pd  :  1;    /* Default: 0x0; */
		unsigned int sot_sync_err_d1_pd  :  1;    /* Default: 0x0; */
		unsigned int sot_sync_err_d2_pd  :  1;    /* Default: 0x0; */
		unsigned int sot_sync_err_d3_pd  :  1;    /* Default: 0x0; */
		unsigned int rx_alg_err_d0_pd    :  1;    /* Default: 0x0; */
		unsigned int rx_alg_err_d1_pd    :  1;    /* Default: 0x0; */
		unsigned int rx_alg_err_d2_pd    :  1;    /* Default: 0x0; */
		unsigned int rx_alg_err_d3_pd    :  1;    /* Default: 0x0; */
		unsigned int res0                :  6;    /* Default:; */
		unsigned int cd_lp0_err_clk_pd   :  1;    /* Default: 0x0; */
		unsigned int cd_lp1_err_clk_pd   :  1;    /* Default: 0x0; */
		unsigned int cd_lp0_err_d1_pd    :  1;    /* Default: 0x0; */
		unsigned int cd_lp1_err_d1_pd    :  1;    /* Default: 0x0; */
		unsigned int cd_lp0_err_d0_pd    :  1;    /* Default: 0x0; */
		unsigned int cd_lp1_err_d0_pd    :  1;    /* Default: 0x0; */
		unsigned int cd_lp0_err_d2_pd    :  1;    /* Default: 0x0; */
		unsigned int cd_lp1_err_d2_pd    :  1;    /* Default: 0x0; */
		unsigned int cd_lp0_err_d3_pd    :  1;    /* Default: 0x0; */
		unsigned int cd_lp1_err_d3_pd    :  1;    /* Default: 0x0; */
	} bits;
} DPHY_INT_PD0_REG_t;

typedef union {
	unsigned int dwval;
	struct {
		unsigned int ulps_d0_pd          :  1;    /* Default: 0x0; */
		unsigned int ulps_d1_pd          :  1;    /* Default: 0x0; */
		unsigned int ulps_d2_pd          :  1;    /* Default: 0x0; */
		unsigned int ulps_d3_pd          :  1;    /* Default: 0x0; */
		unsigned int ulps_wp_d0_pd       :  1;    /* Default: 0x0; */
		unsigned int ulps_wp_d1_pd       :  1;    /* Default: 0x0; */
		unsigned int ulps_wp_d2_pd       :  1;    /* Default: 0x0; */
		unsigned int ulps_wp_d3_pd       :  1;    /* Default: 0x0; */
		unsigned int ulps_clk_pd         :  1;    /* Default: 0x0; */
		unsigned int ulps_wp_clk_pd      :  1;    /* Default: 0x0; */
		unsigned int res0                :  2;    /* Default:; */
		unsigned int lpdt_d0_pd          :  1;    /* Default: 0x0; */
		unsigned int rx_trnd_d0_pd       :  1;    /* Default: 0x0; */
		unsigned int tx_trnd_err_d0_pd   :  1;    /* Default: 0x0; */
		unsigned int undef1_d0_pd        :  1;    /* Default: 0x0; */
		unsigned int undef2_d0_pd        :  1;    /* Default: 0x0; */
		unsigned int undef3_d0_pd        :  1;    /* Default: 0x0; */
		unsigned int undef4_d0_pd        :  1;    /* Default: 0x0; */
		unsigned int undef5_d0_pd        :  1;    /* Default: 0x0; */
		unsigned int rst_d0_pd           :  1;    /* Default: 0x0; */
		unsigned int rst_d1_pd           :  1;    /* Default: 0x0; */
		unsigned int rst_d2_pd           :  1;    /* Default: 0x0; */
		unsigned int rst_d3_pd           :  1;    /* Default: 0x0; */
		unsigned int esc_cmd_err_d0_pd   :  1;    /* Default: 0x0; */
		unsigned int esc_cmd_err_d1_pd   :  1;    /* Default: 0x0; */
		unsigned int esc_cmd_err_d2_pd   :  1;    /* Default: 0x0; */
		unsigned int esc_cmd_err_d3_pd   :  1;    /* Default: 0x0; */
		unsigned int false_ctl_d0_pd     :  1;    /* Default: 0x0; */
		unsigned int false_ctl_d1_pd     :  1;    /* Default: 0x0; */
		unsigned int false_ctl_d2_pd     :  1;    /* Default: 0x0; */
		unsigned int false_ctl_d3_pd     :  1;    /* Default: 0x0; */
	} bits;
} DPHY_INT_PD1_REG_t;

typedef union {
	unsigned int dwval;
	struct {
		unsigned int res0;    /* Default:; */
	} bits;
} DPHY_INT_PD2_REG_t;

typedef union {
	unsigned int dwval;
	struct {
		unsigned int lptx_sta_d0         :  3;    /* Default: 0; */
		unsigned int res0                :  1;    /* Default:; */
		unsigned int lptx_sta_d1         :  3;    /* Default: 0; */
		unsigned int res1                :  1;    /* Default:; */
		unsigned int lptx_sta_d2         :  3;    /* Default: 0; */
		unsigned int res2                :  1;    /* Default:; */
		unsigned int lptx_sta_d3         :  3;    /* Default: 0; */
		unsigned int res3                :  1;    /* Default:; */
		unsigned int lptx_sta_clk        :  3;    /* Default: 0; */
		unsigned int res4                :  9;    /* Default:; */
		unsigned int direction           :  1;    /* Default: 0; */
		unsigned int res5                :  3;    /* Default:; */
	} bits;
} DPHY_DBG0_REG_t;

typedef union {
	unsigned int dwval;
	struct {
		unsigned int lptx_dbg_en         :  1;    /* Default: 0; */
		unsigned int hstx_dbg_en         :  1;    /* Default: 0; */
		unsigned int res0                :  2;    /* Default:; */
		unsigned int lptx_set_d0         :  2;    /* Default: 0; */
		unsigned int lptx_set_d1         :  2;    /* Default: 0; */
		unsigned int lptx_set_d2         :  2;    /* Default: 0; */
		unsigned int lptx_set_d3         :  2;    /* Default: 0; */
		unsigned int lptx_set_ck         :  2;    /* Default: 0; */
		unsigned int res1                : 18;    /* Default:; */
	} bits;
} DPHY_DBG1_REG_t;

typedef union {
	unsigned int dwval;
	struct {
		unsigned int hstx_data;    /* Default: 0; */
	} bits;
} DPHY_DBG2_REG_t;

typedef union {
	unsigned int dwval;
	struct {
		unsigned int lprx_sta_d0         :  4;    /* Default: 0; */
		unsigned int lprx_sta_d1         :  4;    /* Default: 0; */
		unsigned int lprx_sta_d2         :  4;    /* Default: 0; */
		unsigned int lprx_sta_d3         :  4;    /* Default: 0; */
		unsigned int lprx_sta_clk        :  4;    /* Default: 0; */
		unsigned int res0                : 12;    /* Default:; */
	} bits;
} DPHY_DBG3_REG_t;

typedef union {
	unsigned int dwval;
	struct {
		unsigned int lprx_phy_d0         :  2;    /* Default: 0; */
		unsigned int lprx_phy_d1         :  2;    /* Default: 0; */
		unsigned int lprx_phy_d2         :  2;    /* Default: 0; */
		unsigned int lprx_phy_d3         :  2;    /* Default: 0; */
		unsigned int lprx_phy_clk        :  2;    /* Default: 0; */
		unsigned int res0                :  6;    /* Default:; */
		unsigned int lpcd_phy_d0         :  2;    /* Default: 0; */
		unsigned int lpcd_phy_d1         :  2;    /* Default: 0; */
		unsigned int lpcd_phy_d2         :  2;    /* Default: 0; */
		unsigned int lpcd_phy_d3         :  2;    /* Default: 0; */
		unsigned int lpcd_phy_clk        :  2;    /* Default: 0; */
		unsigned int res1                :  6;    /* Default:; */
	} bits;
} DPHY_DBG4_REG_t;

typedef union {
	unsigned int dwval;
	struct {
		unsigned int hsrx_data0          :  8;    /* Default: 0; */
		unsigned int hsrx_data1          :  8;    /* Default: 0; */
		unsigned int hsrx_data2          :  8;    /* Default: 0; */
		unsigned int hsrx_data3          :  8;    /* Default: 0; */
	} bits;
} DPHY_DBG5_REG_t;


/* ------------------------------------------------------------------------------ */
/*  */
/* ------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

/* ------------------------------------------------------------------------------------------------------------ */
/*  */
/* ------------------------------------------------------------------------------------------------------------ */

#endif
