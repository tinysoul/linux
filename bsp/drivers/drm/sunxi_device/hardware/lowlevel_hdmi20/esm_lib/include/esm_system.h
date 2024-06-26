/* SPDX-License-Identifier: GPL-2.0-or-later
 *
 * Allwinner SoCs hdmi2.0 driver.
 *
 * Copyright (C) 2016 Allwinner.
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */
#ifndef _ESM_SYSTEM_H_
#define _ESM_SYSTEM_H_

/* System Level Macros */
#define ESM_MACRO_ASSERT(__err__)

/* ESM Shared System Definitions */

#define ESM_TEST_BENCH_APP 1
#define ESM_HDCP_RX_APP    2
#define ESM_HDCP_TX_APP    3
#define ESM_HDCP_RX_TX_APP 4
#define ESM_HDCP_RP_APP    5
#define ESM_HDCP_RP_TX_APP 6
#define ESM_HDCP_RP_RX_APP 7
#define ESM_HDCP_HOSTLIB_APP 99


/* enumerated WATCHDOG times */
#define ESM_SYSTEM_WATCHDOG_MODE_OFF    0
#define ESM_SYSTEM_WATCHDOG_MODE_TIME_0 1
#define ESM_SYSTEM_WATCHDOG_MODE_TIME_1 2
#define ESM_SYSTEM_WATCHDOG_MODE_TIME_2 3
#define ESM_SYSTEM_WATCHDOG_MODE_MAX    (ESM_SYSTEM_WATCHDOG_MODE_TIME_2+1)

/* enumerated LOGGING modes */
#define ESM_SYSTEM_LOGGING_MODE_OFF     0
#define ESM_SYSTEM_LOGGING_MODE_ON_MEM  1
#define ESM_SYSTEM_LOGGING_MODE_ON_UART 2
#define ESM_SYSTEM_LOGGING_MODE_MAX     (ESM_SYSTEM_LOGGING_MODE_ON_UART + 1)

#define ESM_SYSTEM_LOGGING_LEVEL_NORMAL 0
#define ESM_SYSTEM_LOGGING_LEVEL_MED    1
#define ESM_SYSTEM_LOGGING_LEVEL_HIGH   2
#define ESM_SYSTEM_LOGGING_LEVEL_MAX   (ESM_SYSTEM_LOGGING_LEVEL_HIGH + 1)

/* external memory */
#define ESM_SYSTEM_MAX_SIZE         0xffffffff
#define ESM_SYSTEM_MB_MAX_SIZE      0xffff
#define ESM_SYSTEM_LOGGING_MAX_SIZE 0xffff
#define ESM_SYSTEM_SRM_MAX_SIZE     0xffff
#define ESM_SYSTEM_PAIRING_MAX_SIZE 0xffff
#define ESM_SYSTEM_EXCEPT_MAX_SIZE  0xffff
#define ESM_SYSTEM_TOPO_MAX_SIZE    0xffff

#endif /* _ESM_SYSTEM_H_ */
