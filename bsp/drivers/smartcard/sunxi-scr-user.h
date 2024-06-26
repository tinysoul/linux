/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright(c) 2020 - 2023 Allwinner Technology Co.,Ltd. All rights reserved. */
/*
 * Copyright (C) 2016 Allwinner.
 * fuzhaoke <fuzhaoke@allwinnertech.com>
 *
 * common data structure or macro used both kernel & application
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */

#ifndef __SUNXI_SCR_USER_H__
#define __SUNXI_SCR_USER_H__

#define MAX_ATR_LEN			33

/* ioctrol command */
#define SCR_IOC_MAGIC		'c'
#define SCR_IOCGSTATUS		_IOR(SCR_IOC_MAGIC, 0, uint32_t)
#define SCR_IOCRESET		_IO(SCR_IOC_MAGIC, 1)
#define SCR_IOCGATR		_IOR(SCR_IOC_MAGIC, 2, struct scr_atr)
#define SCR_IOCGPARA		_IOR(SCR_IOC_MAGIC, 3, struct scr_card_para)
#define SCR_IOCSPARA		_IOW(SCR_IOC_MAGIC, 4, struct scr_card_para)
#define SCR_IOCGATRPARA		_IOW(SCR_IOC_MAGIC, 5, struct smc_atr_para)
#define SCR_IOCGPPSPARA		_IOW(SCR_IOC_MAGIC, 6, struct smc_pps_para)
#define SCR_IOCWRDATA		_IOWR(SCR_IOC_MAGIC, 7, struct scr_wr_data)

enum scr_card_sta {
	SCR_CARD_OUT = 0,
	SCR_CARD_IN,
};

/* ATR data */
struct scr_atr {
	unsigned char atr_len;
	unsigned char atr_data[MAX_ATR_LEN];
};

/* current card parameters & status */
struct scr_card_para {
	uint16_t f;
	uint16_t d;
	uint16_t freq;
	uint8_t recv_no_parity;
	uint8_t protocol_type;
};

/* parse ATR data */
struct smc_atr_para {
	uint8_t TS;

	uint8_t TK[15];
	uint8_t TK_NUM;

	uint8_t T;    /* Protocol */
	uint8_t FMAX; /* unit: MHz */
	uint32_t F;
	uint32_t D;
	uint32_t I;   /* Max Cunrrent for Program, in mA */
	uint32_t P;   /* Pogram Voltage */
	uint32_t N;   /* Extra Guard Time, in ETUs */
};

/* parse ATR data for PPS */
struct smc_pps_para {
	uint8_t ppss;
	uint8_t pps0;
	uint8_t pps1;
	uint8_t pps2;
	uint8_t pps3;
	uint8_t pck;
};

/* write cmd and read data immediately */
struct scr_wr_data {
	uint8_t *cmd_buf;
	uint32_t cmd_len;
	uint8_t *rtn_data;
	uint32_t rtn_len;
	uint8_t psw1;
	uint8_t psw2;
};

#endif
