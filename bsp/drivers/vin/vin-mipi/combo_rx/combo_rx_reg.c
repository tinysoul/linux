/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright(c) 2020 - 2023 Allwinner Technology Co.,Ltd. All rights reserved. */
/*
 * combo rx module
 *
 * Copyright (c) 2017 by Allwinnertech Co., Ltd.  http://www.allwinnertech.com
 *
 * Authors:  Zhao Wei <zhaowei@allwinnertech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include "combo_rx_reg_i.h"
#include "combo_rx_reg.h"
#include "../../utility/vin_io.h"
#include "../../platform/platform_cfg.h"

volatile void *cmb_rx_base_addr[VIN_MAX_MIPI];

int cmb_rx_set_base_addr(unsigned int sel, unsigned long addr)
{
	if (sel > VIN_MAX_MIPI - 1)
		return -1;
	cmb_rx_base_addr[sel] = (volatile void *)addr;

	return 0;
}

void cmb_rx_enable(unsigned int sel)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_CTR_REG_OFF,
		CMB_PHYA_PWDN_MASK, 1 << CMB_PHYA_PWDN);
		vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_CTR_REG_OFF,
		CMB_PHYA_ENABLE_MASK, 1 << CMB_PHYA_ENABLE);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MODE_SEL_REG_OFF,
		CMB_MODULE_EN_MASK, 1 << CMB_MODULE_EN);
}

void cmb_rx_disable(unsigned int sel)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_CTR_REG_OFF,
		CMB_PHYA_PWDN_MASK, 0 << CMB_PHYA_PWDN);
		vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_CTR_REG_OFF,
		CMB_PHYA_ENABLE_MASK, 0 << CMB_PHYA_ENABLE);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MODE_SEL_REG_OFF,
		CMB_MODULE_EN_MASK, 0 << CMB_MODULE_EN);
}

void cmb_rx_mode_sel(unsigned int sel, enum combo_rx_mode_sel mode)
{
#if IS_ENABLED(CONFIG_ARCH_SUN8IW12P1)
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MODE_SEL_REG_OFF,
		CMB_MODE_SEL_MASK, mode << CMB_MODE_SEL);
#else
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_CFG_REG_OFF,
		CMB_MODE_SEL_MASK, mode << CMB_MODE_SEL);
#endif
}

void cmb_rx_app_pixel_out(unsigned int sel, enum combo_rx_pix_num pix_num)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_APP_PIXEL_OUT_REG_OFF,
		CMB_APP_PIXEL_OUT_SEL_MASK, pix_num << CMB_APP_PIXEL_OUT_SEL);
}

void cmb_rx_phya_a_d0_en(unsigned int sel, unsigned int en)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_LANE_EN_REG_OFF,
		CMB_PHYA_A_D0_EN_MASK, en << CMB_PHYA_A_D0_EN);
}

void cmb_rx_phya_b_d0_en(unsigned int sel, unsigned int en)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_LANE_EN_REG_OFF,
		CMB_PHYA_B_D0_EN_MASK, en << CMB_PHYA_B_D0_EN);
}

void cmb_rx_phya_c_d0_en(unsigned int sel, unsigned int en)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_LANE_EN_REG_OFF,
		CMB_PHYA_C_D0_EN_MASK, en << CMB_PHYA_C_D0_EN);
}

void cmb_rx_phya_a_d1_en(unsigned int sel, unsigned int en)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_LANE_EN_REG_OFF,
		CMB_PHYA_A_D1_EN_MASK, en << CMB_PHYA_A_D1_EN);
}

void cmb_rx_phya_b_d1_en(unsigned int sel, unsigned int en)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_LANE_EN_REG_OFF,
		CMB_PHYA_B_D1_EN_MASK, en << CMB_PHYA_B_D1_EN);
}

void cmb_rx_phya_c_d1_en(unsigned int sel, unsigned int en)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_LANE_EN_REG_OFF,
		CMB_PHYA_C_D1_EN_MASK, en << CMB_PHYA_C_D1_EN);
}

void cmb_rx_phya_a_d2_en(unsigned int sel, unsigned int en)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_LANE_EN_REG_OFF,
		CMB_PHYA_A_D2_EN_MASK, en << CMB_PHYA_A_D2_EN);
}

void cmb_rx_phya_b_d2_en(unsigned int sel, unsigned int en)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_LANE_EN_REG_OFF,
		CMB_PHYA_B_D2_EN_MASK, en << CMB_PHYA_B_D2_EN);
}

void cmb_rx_phya_c_d2_en(unsigned int sel, unsigned int en)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_LANE_EN_REG_OFF,
		CMB_PHYA_C_D2_EN_MASK, en << CMB_PHYA_C_D2_EN);
}

void cmb_rx_phya_a_d3_en(unsigned int sel, unsigned int en)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_LANE_EN_REG_OFF,
		CMB_PHYA_A_D3_EN_MASK, en << CMB_PHYA_A_D3_EN);
}

void cmb_rx_phya_b_d3_en(unsigned int sel, unsigned int en)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_LANE_EN_REG_OFF,
		CMB_PHYA_B_D3_EN_MASK, en << CMB_PHYA_B_D3_EN);
}

void cmb_rx_phya_c_d3_en(unsigned int sel, unsigned int en)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_LANE_EN_REG_OFF,
		CMB_PHYA_C_D3_EN_MASK, en << CMB_PHYA_C_D3_EN);
}

void cmb_rx_phya_a_ck_en(unsigned int sel, unsigned int en)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_LANE_EN_REG_OFF,
		CMB_PHYA_A_CK_EN_MASK, en << CMB_PHYA_A_CK_EN);
}

void cmb_rx_phya_b_ck_en(unsigned int sel, unsigned int en)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_LANE_EN_REG_OFF,
		CMB_PHYA_B_CK_EN_MASK, en << CMB_PHYA_B_CK_EN);
}

void cmb_rx_phya_c_ck_en(unsigned int sel, unsigned int en)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_LANE_EN_REG_OFF,
		CMB_PHYA_C_CK_EN_MASK, en << CMB_PHYA_C_CK_EN);
}

void cmb_rx_te_auto_disable(unsigned int sel, unsigned int en)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_LANE_EN_REG_OFF,
		CMB_TE_AUTO_DISABLE_MASK, en << CMB_TE_AUTO_DISABLE);
}

void cmb_rx_phya_config(unsigned int sel)
{
	/* vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_CTR_REG_OFF,
		CMB_PHYA_PWDN_MASK, 1 << CMB_PHYA_PWDN); */
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_CTR_REG_OFF,
		CMB_PHYA_CKIN_MODE_SEL_MASK, 1 << CMB_PHYA_CKIN_MODE_SEL); /* mipi 0, lvds 1 */
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_CTR_REG_OFF,
		CMB_PHYA_BIT_SEL_MASK, 2 << CMB_PHYA_BIT_SEL);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_CTR_REG_OFF,
		CMB_PHYA_OUT_CLK_POL_MASK, ((sel+1)%2) << CMB_PHYA_OUT_CLK_POL);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_CTR_REG_OFF,
		CMB_PHYA_IN_CLK_POL_MASK, 1 << CMB_PHYA_IN_CLK_POL);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_CTR_REG_OFF,
		CMB_PHYA_IRM_MASK, 0 << CMB_PHYA_IRM);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_CTR_REG_OFF,
		CMB_PHYA_VRM_MASK, 0 << CMB_PHYA_VRM);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_CTR_REG_OFF,
		CMB_PHYA_LP_VOL_HL_MASK, 0 << CMB_PHYA_LP_VOL_HL);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_CTR_REG_OFF,
		CMB_PHYA_LP_VOL_LH_MASK, 0 << CMB_PHYA_LP_VOL_LH);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_CTR_REG_OFF,
		CMB_PHYA_OFFSET_SEL_MASK, 0 << CMB_PHYA_OFFSET_SEL); /* V5:mipi 3, lvds 0; V5200:mipi 1, lvds 0 */
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_CTR_REG_OFF,
		CMB_PHYA_RESET_SEL_MASK, 0 << CMB_PHYA_RESET_SEL);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_CTR_REG_OFF,
		CMB_PHYA_SB_SEL_MASK, 0 << CMB_PHYA_SB_SEL);
	/* vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_CTR_REG_OFF,
		CMB_PHYA_ENABLE_MASK, 1 << CMB_PHYA_ENABLE); */
}

void cmb_rx_phya_ck_mode(unsigned int sel, unsigned int mode)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_CTR_REG_OFF,
		CMB_PHYA_CKIN_MODE_SEL_MASK, mode << CMB_PHYA_CKIN_MODE_SEL); /* mipi 0, lvds 1 */
}

void cmb_rx_phya_ck_pol(unsigned int sel, unsigned int pol)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_CTR_REG_OFF,
		CMB_PHYA_OUT_CLK_POL_MASK, pol << CMB_PHYA_OUT_CLK_POL);
}

void cmb_rx_phya_offset(unsigned int sel, unsigned int offset)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_CTR_REG_OFF,
		CMB_PHYA_OFFSET_SEL_MASK, offset << CMB_PHYA_OFFSET_SEL); /* V5:mipi 3, lvds 0; V5V200:mipi 1, lvds 0 */
}

void cmb_rx_phya_signal_dly_en(unsigned int sel, unsigned int en)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_DLY_CTR0_REG_OFF,
		CMB_PHYA_DLY_EN_MASK, en << CMB_PHYA_DLY_EN);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_DLY_CTR0_REG_OFF,
		CMB_PHYA_A_CLK_DLY_SET_MASK, 0x1 << CMB_PHYA_A_CLK_DLY_SET);
}

void cmb_rx_phya_signal_dly_ctr(unsigned int sel, struct phya_signal_dly_ctr *phya_signal_dly)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_DLY_CTR0_REG_OFF,
		CMB_PHYA_A_CLK_DLY_SET_MASK, phya_signal_dly->a_clk_dly << CMB_PHYA_A_CLK_DLY_SET);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_DLY_CTR0_REG_OFF,
		CMB_PHYA_B_CLK_DLY_SET_MASK, phya_signal_dly->b_clk_dly << CMB_PHYA_B_CLK_DLY_SET);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_DLY_CTR0_REG_OFF,
		CMB_PHYA_C_CLK_DLY_SET_MASK, phya_signal_dly->c_clk_dly << CMB_PHYA_C_CLK_DLY_SET);

	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_DLY_CTR0_REG_OFF,
		CMB_PHYA_A_D0_DLY_SET_MASK, phya_signal_dly->a_d0_dly << CMB_PHYA_A_D0_DLY_SET);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_DLY_CTR0_REG_OFF,
		CMB_PHYA_B_D0_DLY_SET_MASK, phya_signal_dly->b_d0_dly << CMB_PHYA_B_D0_DLY_SET);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_DLY_CTR0_REG_OFF,
		CMB_PHYA_C_D0_DLY_SET_MASK, phya_signal_dly->c_d0_dly << CMB_PHYA_C_D0_DLY_SET);

	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_DLY_CTR1_REG_OFF,
		CMB_PHYA_A_D1_DLY_SET_MASK, phya_signal_dly->a_d1_dly << CMB_PHYA_A_D1_DLY_SET);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_DLY_CTR1_REG_OFF,
		CMB_PHYA_B_D1_DLY_SET_MASK, phya_signal_dly->b_d1_dly << CMB_PHYA_B_D1_DLY_SET);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_DLY_CTR1_REG_OFF,
		CMB_PHYA_C_D1_DLY_SET_MASK, phya_signal_dly->c_d1_dly << CMB_PHYA_C_D1_DLY_SET);

	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_DLY_CTR1_REG_OFF,
		CMB_PHYA_A_D2_DLY_SET_MASK, phya_signal_dly->a_d2_dly << CMB_PHYA_A_D2_DLY_SET);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_DLY_CTR1_REG_OFF,
		CMB_PHYA_B_D2_DLY_SET_MASK, phya_signal_dly->b_d2_dly << CMB_PHYA_B_D2_DLY_SET);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_DLY_CTR1_REG_OFF,
		CMB_PHYA_C_D2_DLY_SET_MASK, phya_signal_dly->c_d2_dly << CMB_PHYA_C_D2_DLY_SET);

	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_DLY_CTR2_REG_OFF,
		CMB_PHYA_A_D3_DLY_SET_MASK, phya_signal_dly->a_d3_dly << CMB_PHYA_A_D3_DLY_SET);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_DLY_CTR2_REG_OFF,
		CMB_PHYA_B_D3_DLY_SET_MASK, phya_signal_dly->b_d3_dly << CMB_PHYA_B_D3_DLY_SET);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_PHYA_DLY_CTR2_REG_OFF,
		CMB_PHYA_C_D3_DLY_SET_MASK, phya_signal_dly->c_d3_dly << CMB_PHYA_C_D3_DLY_SET);

}

void cmb_rx_mipi_ctr(unsigned int sel, struct mipi_ctr *mipi_ctr)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MIPI_DPHY_CTR_REG_OFF,
		CMB_DPHY_CLK_SEL_MASK, 0 << CMB_DPHY_CLK_SEL);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MIPI_DPHY_CTR_REG_OFF,
		CMB_DPHY_LANE_NUM_MASK, mipi_ctr->mipi_lane_num << CMB_DPHY_LANE_NUM);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MIPI_DPHY_CTR_REG_OFF,
		CMB_DPHY_MSB_FIRST_SEL_MASK, 1 << CMB_DPHY_MSB_FIRST_SEL);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MIPI_DPHY_CTR_REG_OFF,
		CMB_DPHY_LP_MODE_CTR_MASK, 0 << CMB_DPHY_LP_MODE_CTR);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MIPI_VMODE_REG_OFF,
		CMB_MIPI_VMODE_MASK, mipi_ctr->mipi_open_multi_ch << CMB_MIPI_VMODE);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MODE_SEL_REG_OFF,
		CMB_WDR_MODE_SEL_MASK, mipi_ctr->mipi_wdr_mode_sel << CMB_WDR_MODE_SEL);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MIPI_VC0_HEIGH_REG_OFF,
		CMB_MIPI_VC0_HIGHT_MASK, mipi_ctr->mipi_ch0_height << CMB_MIPI_VC0_HIGHT);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MIPI_VC1_HEIGH_REG_OFF,
		CMB_MIPI_VC1_HIGHT_MASK, mipi_ctr->mipi_ch1_height << CMB_MIPI_VC1_HIGHT);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MIPI_VC2_HEIGH_REG_OFF,
		CMB_MIPI_VC2_HIGHT_MASK, mipi_ctr->mipi_ch2_height << CMB_MIPI_VC2_HIGHT);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MIPI_VC3_HEIGH_REG_OFF,
		CMB_MIPI_VC3_HIGHT_MASK, mipi_ctr->mipi_ch3_height << CMB_MIPI_VC3_HIGHT);
}

void cmb_rx_mipi_stl_time(unsigned int sel, unsigned char time_hs)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MIPI_DPHY_CTR_REG_OFF,
		CMB_DPHY_HS_DLY_OFF_MASK, 0 << CMB_DPHY_HS_DLY_OFF);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MIPI_DPHY_CTR_REG_OFF,
		CMB_DPHY_P0_TIME_HS_ON_MASK, time_hs << CMB_DPHY_P0_TIME_HS_ON);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MIPI_DPHY_CTR_REG_OFF,
		CMB_DPHY_P0_TIME_DT_ON_MASK, 1 << CMB_DPHY_P0_TIME_DT_ON);
}

void cmb_rx_mipi_dphy_mapping(unsigned int sel, struct mipi_lane_map *mipi_map)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MIPI_DPHY_LANE_MAPPING_REG_OFF,
		CMB_DPHY_INTERNAL_LANE0_MASK, mipi_map->mipi_lane0 << CMB_DPHY_INTERNAL_LANE0);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MIPI_DPHY_LANE_MAPPING_REG_OFF,
		CMB_DPHY_INTERNAL_LANE1_MASK, mipi_map->mipi_lane1 << CMB_DPHY_INTERNAL_LANE1);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MIPI_DPHY_LANE_MAPPING_REG_OFF,
		CMB_DPHY_INTERNAL_LANE2_MASK, mipi_map->mipi_lane2 << CMB_DPHY_INTERNAL_LANE2);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MIPI_DPHY_LANE_MAPPING_REG_OFF,
		CMB_DPHY_INTERNAL_LANE3_MASK, mipi_map->mipi_lane3 << CMB_DPHY_INTERNAL_LANE3);
}

void cmb_rx_mipi_csi2_status(unsigned int sel, unsigned int *mipi_csi2_status)
{
	*mipi_csi2_status = vin_reg_readl(cmb_rx_base_addr[sel] + CMB_MIP_CSI2_STATUS_REG_OFF);
}

void cmb_rx_mipi_csi2_data_id(unsigned int sel, unsigned int *mipi_csi2_data_id)
{
	*mipi_csi2_data_id = vin_reg_readl(cmb_rx_base_addr[sel] + CMB_MIP_CSI2_DATA_ID_REG_OFF);
}

void cmb_rx_mipi_csi2_word_cnt(unsigned int sel, unsigned int *mipi_csi2_word_cnt)
{
	*mipi_csi2_word_cnt = vin_reg_readl(cmb_rx_base_addr[sel] + CMB_MIP_CSI2_WORD_CNT_REG_OFF);
}

void cmb_rx_mipi_csi2_ecc_val(unsigned int sel, unsigned int *mipi_csi2_ecc_val)
{
	*mipi_csi2_ecc_val = vin_reg_readl(cmb_rx_base_addr[sel] + CMB_MIP_CSI2_ECC_VAL_REG_OFF);
}

void cmb_rx_mipi_csi2_line_lentgh(unsigned int sel, unsigned int *mipi_csi2_line_lentgh)
{
	*mipi_csi2_line_lentgh = vin_reg_readl(cmb_rx_base_addr[sel] + CMB_MIP_CSI2_LINE_LENTGH_REG_OFF);
}

void cmb_rx_mipi_csi2_rcv_cnt(unsigned int sel, unsigned int *mipi_csi2_rcv_cnt)
{
	*mipi_csi2_rcv_cnt = vin_reg_readl(cmb_rx_base_addr[sel] + CMB_MIP_CSI2_RCV_CNT_REG_OFF);
}

void cmb_rx_mipi_csi2_ecc_err_cnt(unsigned int sel, unsigned int *mipi_csi2_ecc_err_cnt)
{
	*mipi_csi2_ecc_err_cnt = vin_reg_readl(cmb_rx_base_addr[sel] + CMB_MIP_CSI2_ECC_ERR_CNT_REG_OFF);
}

void cmb_rx_mipi_csi2_check_sum_err_cnt(unsigned int sel, unsigned int *mipi_csi2_check_sum_err_cnt)
{
	*mipi_csi2_check_sum_err_cnt = vin_reg_readl(cmb_rx_base_addr[sel] + CMB_MIP_CSI2_CHECK_SUM_ERR_CNT_REG_OFF);
}

void cmb_rx_lvds_ctr(unsigned int sel, struct lvds_ctr *lvds_ctr)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_LVDS_CTR_REG_OFF,
		CMB_LVDS_CLK_SEL_MASK, 0 << CMB_LVDS_CLK_SEL);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_LVDS_CTR_REG_OFF,
		CMB_LVDS_REC_DIRECT_MASK, 0 << CMB_LVDS_REC_DIRECT);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_LVDS_CTR_REG_OFF,
		CMB_LVDS_MSB_FIRST_SEL_MASK, 0 << CMB_LVDS_MSB_FIRST_SEL);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_LVDS_CTR_REG_OFF,
		CMB_LVDS_LANE_NUM_MASK, lvds_ctr->lvds_lane_num << CMB_LVDS_LANE_NUM);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_LVDS_CTR_REG_OFF,
		CMB_LVDS_MODE_SEL_MASK, 0 << CMB_LVDS_MODE_SEL);

	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MODE_SEL_REG_OFF,
		CMB_WDR_MODE_SEL_MASK, lvds_ctr->lvds_wdr_lbl_sel << CMB_WDR_MODE_SEL);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MODE_SEL_REG_OFF,
		CMB_FID_MODE_SEL_MASK, lvds_ctr->lvds_wdr_fid_mode_sel << CMB_FID_MODE_SEL);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MODE_SEL_REG_OFF,
		CMB_WDR_FIDMAP_SEL_EN_MASK, lvds_ctr->lvds_wdr_fid_map_en << CMB_WDR_FIDMAP_SEL_EN);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MODE_SEL_REG_OFF,
		CMB_WDR_FIDMAP_SEL_0_MASK, lvds_ctr->lvds_wdr_fid0_map_sel << CMB_WDR_FIDMAP_SEL_0);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MODE_SEL_REG_OFF,
		CMB_WDR_FIDMAP_SEL_1_MASK, lvds_ctr->lvds_wdr_fid1_map_sel << CMB_WDR_FIDMAP_SEL_1);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MODE_SEL_REG_OFF,
		CMB_WDR_FIDMAP_SEL_2_MASK, lvds_ctr->lvds_wdr_fid2_map_sel << CMB_WDR_FIDMAP_SEL_2);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MODE_SEL_REG_OFF,
		CMB_WDR_FIDMAP_SEL_3_MASK, lvds_ctr->lvds_wdr_fid3_map_sel << CMB_WDR_FIDMAP_SEL_3);

	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_LVDS_CTR_REG_OFF,
		CMB_LVDS_BIT_WIDTH_SEL_MASK, lvds_ctr->lvds_bit_width << CMB_LVDS_BIT_WIDTH_SEL);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_LVDS_CTR_REG_OFF,
		CMB_LVDS_SYNC_LINE_CODE_MODE_SEL_MASK, lvds_ctr->lvds_line_code_mode << CMB_LVDS_SYNC_LINE_CODE_MODE_SEL);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_LVDS_CTR_REG_OFF,
		CMB_LVDS_PIX_LSB_MASK, lvds_ctr->lvds_pix_lsb << CMB_LVDS_PIX_LSB);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_LVDS_CTR_REG_OFF,
		CMB_LVDS_SYNC_CODE_LINE_CNT_MASK, lvds_ctr->lvds_sync_code_line_cnt << CMB_LVDS_SYNC_CODE_LINE_CNT);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_LVDS_CTR_REG_OFF,
		CMB_LVDS_SYNC_CODE_WD4_SEL_MASK, 0 << CMB_LVDS_SYNC_CODE_WD4_SEL);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_HISPI_CODE_MASK_REG_OFF,
		CMB_HISPI_CODE_MASK_MASK, lvds_ctr->lvds_code_mask << CMB_HISPI_CODE_MASK);

	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MIPI_VMODE_REG_OFF,
		CMB_MIPI_VMODE_MASK, lvds_ctr->lvds_wdr_en_multi_ch << CMB_MIPI_VMODE);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MIPI_VC0_HEIGH_REG_OFF,
		CMB_MIPI_VC0_HIGHT_MASK, lvds_ctr->lvds_wdr_ch0_height << CMB_MIPI_VC0_HIGHT);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MIPI_VC1_HEIGH_REG_OFF,
		CMB_MIPI_VC1_HIGHT_MASK, lvds_ctr->lvds_wdr_ch1_height << CMB_MIPI_VC1_HIGHT);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MIPI_VC2_HEIGH_REG_OFF,
		CMB_MIPI_VC2_HIGHT_MASK, lvds_ctr->lvds_wdr_ch2_height << CMB_MIPI_VC2_HIGHT);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_MIPI_VC3_HEIGH_REG_OFF,
		CMB_MIPI_VC3_HIGHT_MASK, lvds_ctr->lvds_wdr_ch3_height << CMB_MIPI_VC3_HIGHT);
}

void cmb_rx_hispi_ctr(unsigned int sel, struct hispi_ctr *hispi_ctr)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_HISPI_CTR_REG_OFF,
		CMB_HISPI_HDR_EN_MASK, hispi_ctr->hispi_wdr_en << CMB_HISPI_HDR_EN);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_HISPI_CTR_REG_OFF,
		CMB_HISPI_NORMAL_EN_MASK, hispi_ctr->hispi_normal << CMB_HISPI_NORMAL_EN);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_HISPI_CTR_REG_OFF,
		CMB_HISPI_MODE_SEL_MASK, hispi_ctr->hispi_trans_mode << CMB_HISPI_MODE_SEL);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_HISPI_SYNC_CODE_REG_OFF,
		CMB_HISPI_HDR_SOF_FILD_MASK, hispi_ctr->hispi_wdr_sof_fild << CMB_HISPI_HDR_SOF_FILD);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_HISPI_SYNC_CODE_REG_OFF,
		CMB_HISPI_HDR_EOF_FILD_MASK, hispi_ctr->hispi_wdr_eof_fild << CMB_HISPI_HDR_EOF_FILD);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_HISPI_CODE_MASK_REG_OFF,
		CMB_HISPI_CODE_MASK_MASK, hispi_ctr->hispi_code_mask << CMB_HISPI_CODE_MASK);
}

void cmb_rx_lvds_mapping(unsigned int sel, struct combo_lane_map *lvds_map)
{
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_LVDS_LANE_MAPPING0_REG_OFF,
		CMB_LVDS_LANE0_MAPPING_MASK, lvds_map->lvds_lane0 << CMB_LVDS_LANE0_MAPPING);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_LVDS_LANE_MAPPING0_REG_OFF,
		CMB_LVDS_LANE1_MAPPING_MASK, lvds_map->lvds_lane1 << CMB_LVDS_LANE1_MAPPING);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_LVDS_LANE_MAPPING0_REG_OFF,
		CMB_LVDS_LANE2_MAPPING_MASK, lvds_map->lvds_lane2 << CMB_LVDS_LANE2_MAPPING);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_LVDS_LANE_MAPPING0_REG_OFF,
		CMB_LVDS_LANE3_MAPPING_MASK, lvds_map->lvds_lane3 << CMB_LVDS_LANE3_MAPPING);

	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_LVDS_LANE_MAPPING1_REG_OFF,
		CMB_LVDS_LANE4_MAPPING_MASK, lvds_map->lvds_lane4 << CMB_LVDS_LANE4_MAPPING);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_LVDS_LANE_MAPPING1_REG_OFF,
		CMB_LVDS_LANE5_MAPPING_MASK, lvds_map->lvds_lane5 << CMB_LVDS_LANE5_MAPPING);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_LVDS_LANE_MAPPING1_REG_OFF,
		CMB_LVDS_LANE6_MAPPING_MASK, lvds_map->lvds_lane6 << CMB_LVDS_LANE6_MAPPING);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_LVDS_LANE_MAPPING1_REG_OFF,
		CMB_LVDS_LANE7_MAPPING_MASK, lvds_map->lvds_lane7 << CMB_LVDS_LANE7_MAPPING);

	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_LVDS_LANE_MAPPING2_REG_OFF,
		CMB_LVDS_LANE8_MAPPING_MASK, lvds_map->lvds_lane8 << CMB_LVDS_LANE8_MAPPING);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_LVDS_LANE_MAPPING2_REG_OFF,
		CMB_LVDS_LANE9_MAPPING_MASK, lvds_map->lvds_lane9 << CMB_LVDS_LANE9_MAPPING);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_LVDS_LANE_MAPPING2_REG_OFF,
		CMB_LVDS_LANE10_MAPPING_MASK, lvds_map->lvds_lane10 << CMB_LVDS_LANE10_MAPPING);
	vin_reg_clr_set(cmb_rx_base_addr[sel] + CMB_LVDS_LANE_MAPPING2_REG_OFF,
		CMB_LVDS_LANE11_MAPPING_MASK, lvds_map->lvds_lane11 << CMB_LVDS_LANE11_MAPPING);
}


void cmb_rx_lvds_sync_code(unsigned int sel, struct combo_sync_code *lvds_sync_code)
{
	int lane;

	for (lane = 0; lane < 12; lane++) {
		vin_reg_writel(cmb_rx_base_addr[sel] + CMB_LVDS_LANE0_SOF_SET0_REG_OFF + 0x20 * lane,
			lvds_sync_code->lane_sof[lane].low_bit);
		vin_reg_writel(cmb_rx_base_addr[sel] + CMB_LVDS_LANE0_SOF_SET1_REG_OFF + 0x20 * lane,
			lvds_sync_code->lane_sof[lane].high_bit);
		vin_reg_writel(cmb_rx_base_addr[sel] + CMB_LVDS_LANE0_SOL_SET0_REG_OFF + 0x20 * lane,
			lvds_sync_code->lane_sol[lane].low_bit);
		vin_reg_writel(cmb_rx_base_addr[sel] + CMB_LVDS_LANE0_SOL_SET1_REG_OFF + 0x20 * lane,
			lvds_sync_code->lane_sol[lane].high_bit);
		vin_reg_writel(cmb_rx_base_addr[sel] + CMB_LVDS_LANE0_EOL_SET0_REG_OFF + 0x20 * lane,
			lvds_sync_code->lane_eol[lane].low_bit);
		vin_reg_writel(cmb_rx_base_addr[sel] + CMB_LVDS_LANE0_EOL_SET1_REG_OFF + 0x20 * lane,
			lvds_sync_code->lane_eol[lane].high_bit);
		vin_reg_writel(cmb_rx_base_addr[sel] + CMB_LVDS_LANE0_EOF_SET0_REG_OFF + 0x20 * lane,
			lvds_sync_code->lane_eof[lane].low_bit);
		vin_reg_writel(cmb_rx_base_addr[sel] + CMB_LVDS_LANE0_EOF_SET1_REG_OFF + 0x20 * lane,
			lvds_sync_code->lane_eof[lane].high_bit);
	}
}
