/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright(c) 2020 - 2023 Allwinner Technology Co.,Ltd. All rights reserved. */
 /*
  * Hawkview ISP - sun8iw12p1_vin_cfg.h module
  *
  * Copyright (c) 2016 by Allwinnertech Co., Ltd.  http://www.allwinnertech.com
  *
  * Authors:  Zhao Wei <zhaowei@allwinnertech.com>
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License version 2 as
  * published by the Free Software Foundation.
  */

#ifndef _SUN8IW12P1_VIN_CFG_H_
#define _SUN8IW12P1_VIN_CFG_H_

#define CSI_TOP_REGS_BASE			0x06600000

#define CSI0_REGS_BASE				0x06601000
#define CSI1_REGS_BASE				0x06602000
#define CSI2_REGS_BASE				0x06603000
#define CSI3_REGS_BASE				0x06604000

#define CSI_CCI0_REG_BASE			0x06614000
#define CSI_CCI1_REG_BASE			0x06614400
#define CSI_CCI2_REG_BASE			0x06614800
#define CSI_CCI3_REG_BASE			0x06614c00

#define VIPP0_REGS_BASE				0x02101000
#define VIPP1_REGS_BASE				0x02101400
#define VIPP2_REGS_BASE				0x02101800
#define VIPP3_REGS_BASE				0x02101c00
#define VIPP4_REGS_BASE				0x02102000
#define VIPP5_REGS_BASE				0x02102400
#define VIPP6_REGS_BASE				0x02102800
#define VIPP7_REGS_BASE				0x02102c00

#define ISP_REGS_BASE				0x02100000
#define ISP0_REGS_BASE				0x02100000
#define ISP1_REGS_BASE				0x02100800

#define GPIO_REGS_VBASE				0x0300b000

#define CPU_DRAM_PADDR_ORG			0x40000000
#define HW_DMA_OFFSET				0x00000000

/* set vin core clk base on sensor size */
#define CORE_CLK_RATE_FOR_2M		(108*1000*1000)
#define CORE_CLK_RATE_FOR_3M		(216*1000*1000)
#define CORE_CLK_RATE_FOR_5M		(216*1000*1000)
#define CORE_CLK_RATE_FOR_8M		(432*1000*1000)
#define CORE_CLK_RATE_FOR_16M		(432*1000*1000)

/* CSI & ISP size configs */

#define CSI_REG_SIZE			0x1000
#define CSI_CCI_REG_SIZE		0x0400

#define MIPI_CSI2_REG_SIZE		0x1000
#define MIPI_DPHY_REG_SIZE		0x1000

#define VIN_MAX_DEV			8
#define VIN_MAX_CSI			4
#define VIN_MAX_CCI			4
#define VIN_MAX_TDM			0
#define VIN_MAX_MIPI			2
#define VIN_MAX_ISP			4
#define VIN_MAX_SCALER			8

#define MAX_CH_NUM      4

#endif /* _SUN8IW12P1_VIN_CFG_H_ */
