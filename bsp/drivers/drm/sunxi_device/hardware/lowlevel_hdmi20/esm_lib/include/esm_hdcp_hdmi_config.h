/* SPDX-License-Identifier: GPL-2.0-or-later */
/*******************************************************************************
 * Allwinner SoCs hdmi2.0 driver.
 *
 * Copyright (C) 2016 Allwinner.
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 ******************************************************************************/
#ifndef _ESM_HDCP_HDMI_CONFIG_H_
#define _ESM_HDCP_HDMI_CONFIG_H_

#include "esm_hdcp_hdmi_common.h"

#define ESM_HDCP_HDMI_GAP_1        128
#define ESM_HDCP_HDMI_GAP_1_TX     ESM_HDCP_HDMI_GAP_1

#define ESM_HDCP_HDMI_GAP_2       64


#if (ESM_HDCP_APP_MODE == ESM_HDCP_HOSTLIB_APP)
#define ESM_HDCP_HDMI_CONFIG_VERSION  0x01
#elif (ESM_HDCP_APP_MODE == ESM_HDCP_RX_APP)
#define ESM_HDCP_HDMI_CONFIG_VERSION  0x01
#elif (ESM_HDCP_APP_MODE == ESM_HDCP_TX_APP)
#define ESM_HDCP_HDMI_CONFIG_VERSION  0x01
#elif (ESM_HDCP_APP_MODE == ESM_HDCP_RP_APP) || \
	(ESM_HDCP_APP_MODE == ESM_HDCP_RP_TX_APP) || \
	(ESM_HDCP_APP_MODE == ESM_HDCP_RP_RX_APP)
#define ESM_HDCP_HDMI_CONFIG_VERSION  0x01
#endif


/* this structure contains elements used internally */
typedef struct _esm_hdmi_hdcp_controls_internal_ {
	/* general */
	uint8_t  ake_timeouts;
	uint8_t  cee_enable;
	uint32_t cee_bsod;
	uint32_t cee_frame_cnt_debug;

#if (ESM_HDCP_APP_MODE == ESM_HDCP_RX_TX_APP) || \
	(ESM_HDCP_APP_MODE == ESM_HDCP_TX_APP)  || \
	(ESM_HDCP_APP_MODE == ESM_HDCP_HOSTLIB_APP)
	/* TX */
	uint8_t pairing_enabled;
	uint8_t max_pairing_devices;
	uint8_t max_attempts_li;
	uint16_t tx_poll_rxstatus_li;          /* in milliseconds */
	uint16_t tx_wait_li_rxstatus_timeout;  /* in milliseconds */
	uint16_t tx_wait_hdcp2capable_timeout; /* in milliseconds */
	uint16_t tx_wait_ake_complete_timeout; /* in milliseconds */
	uint16_t tx_wait_cs_message_timeout;   /* in milliseconds */
	uint16_t tx_srm_version;
	uint16_t tx_lvc_timeout;               /* in milliseconds */
	uint16_t tx_lvc_ignore_timeout;        /* in milliseconds */
	uint16_t tx_capable_bypass_time;       /* in milliseconds */
	/* Empty for future use */
	uint8_t  spare_gap_1_tx[4];
#endif

	/* Empty for future use */
	uint8_t spare_gap_1[ESM_HDCP_HDMI_GAP_1 - 4];

	/* WoO starting position */
	uint16_t cee_woo_start;
	/* WoO ending position */
	uint16_t cee_woo_end;
	/* WoO CTLS */
	uint8_t  cee_enable_ctls;
} esm_hdmi_hdcp_controls_internal;

typedef struct htest {
	int a;
	esm_hdmi_hdcp_controls_internal b;
} htest_t;

/* Structure used ONLY during init. */
typedef struct esm_hdcp_ctrl {
	uint8_t woocompatability;
	esm_hdmi_hdcp_controls_internal c_int;
} esm_hdmi_hdcp_controls;

typedef struct {
	esm_hdmi_hdcp_controls c;
} esm_hdmi_hdcp_config;

#define ESM_APP_CONFIG_MAX_SIZE (sizeof(esm_hdmi_hdcp_controls) + sizeof(esm_controls))

#endif
