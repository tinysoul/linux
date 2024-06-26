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
#ifndef _ESM_HDCP_HDMI_COMMON_H_
#define _ESM_HDCP_HDMI_COMMON_H_

#include "esm_system.h"
#include "ESMHost.h"

#define ESM_HDCP_APP_MODE ESM_HDCP_HOSTLIB_APP

/* TODO split these HDCP definitions in a separate public header */
#ifndef HDCP_DEVICE_ID_SIZE
#define HDCP_DEVICE_ID_SIZE 5
#endif
#ifndef HDCP_SEQ_NUM_V_SIZE
#define HDCP_SEQ_NUM_V_SIZE 3
#endif
#ifndef HDCP_MAX_DEVICE_COUNT
#define HDCP_MAX_DEVICE_COUNT 31
#endif

#ifndef HDCP_RXINFO_SIZE
#define HDCP_RXINFO_SIZE 2
#endif


#define ESM_HDCP_HDMI_TOPO_INTEGRITY_KEY_SIZE     16
#define ESM_HDCP_HDMI_TOPO_INTEGRITY_SIZE         16
#define ESM_HDCP_HDMI_TOPO_HEADER_SIZE            2
#define ESM_HDCP_HDMI_TOPO_ROW_EMPTY_IND          0
#define ESM_HDCP_HDMI_TOPO_ROW_SET_IND            1
#define ESM_HDCP_HDMI_TOPO_ID_ROW_SIZE            (1 + HDCP_DEVICE_ID_SIZE)
#define ESM_HDCP_HDMI_TOPO_INTEGRITY_COUNTER_SIZE 4
#define ESM_HDCP_HDMI_TOPO_TOTAL_SIZE             (ESM_HDCP_HDMI_TOPO_HEADER_SIZE + \
	HDCP_RXINFO_SIZE + HDCP_SEQ_NUM_V_SIZE + \
	(ESM_HDCP_HDMI_TOPO_ID_ROW_SIZE * HDCP_MAX_DEVICE_COUNT) + \
	ESM_HDCP_HDMI_TOPO_INTEGRITY_COUNTER_SIZE + \
	ESM_HDCP_HDMI_TOPO_INTEGRITY_SIZE)

#endif

