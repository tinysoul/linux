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
#ifndef _ESM_ERRORS_H_
#define _ESM_ERRORS_H_

#define ESM_SYSTEM_ERRORS          -128
#define ESM_BSP_PLATFORM_ERRORS    -256
#define ESM_ERR_APP_START          -512
#define ESM_ERR_SC_START           -1024
#define ESM_ERR_SA_START           -1280

/* Success. */
#define ESM_SUCCESS                0

/* ESM generic errors. */
#define ESM_FAILED                -1
#define ESM_ERR_BAD_PARAMETER     -2
#define ESM_ERR_NO_MEMORY         -3
#define ESM_ERR_INVALID_FSM_STATE -4
#define ESM_ERR_INVALID_SIZE      -5
#define ESM_ERR_INVALID_CMD       -6
#define ESM_ERR_NOT_INIT          -7

/* ESM command errors. */
#define ESM_ERR_CMD_OVERFLOW      -10
#define ESM_ERR_CMD_UNDERFLOW     -11
#define ESM_ERR_CMD_EMPTY         -12
#define ESM_ERR_CMD_READ_FAILED   -13
#define ESM_ERR_CMD_WRITE_FAILED  -14

/* ESM init errors. */
#define ESM_ERR_MAIN_INIT_FAILED       -20
#define ESM_ERR_CRYPTO_INIT_FAILED     -21
#define ESM_ERR_CRYPTO_END_FAILED      -22
#define ESM_ERR_CRYPTO_KGK_INIT_FAILED -23
#define ESM_ERR_CRYPTO_KGK_PROC_FAILED -24
#define ESM_ERR_CRYPTO_KGK_END_FAILED  -25
#define ESM_ERR_SYSTEM_TIMER_FAILED    -26

/* ESM processor errors */
#define ESM_ERR_WRITE_MESSAGE_FAILED  -30
#define ESM_ERR_WRITE_MESSAGE2_FAILED -31
#define ESM_ERR_SYSTEM_HEALTH_FAILED  -32

/* ESM end errors. */
#define ESM_ERR_MAIN_END_FAILED  -40

/* ESM FSM errors. */
#define ESM_ERR_MAIN_FSM_FAILED -50

/* ESM app errors. */
#define ESM_ERR_APP_INIT_FAILED -60
#define ESM_ERR_APP_END_FAILED  -61

/* Communication errors. */
#define ESM_ERR_COMM_ERROR      -70

/* Configuration image errors. */
#define ESM_CONFIG_BAD_COOKIE                   -80
#define ESM_CONFIG_VERSION_NOT_SUPPORTED        -81
#define ESM_SYSTEM_VERSION_NOT_SUPPORTED        -82
#define ESM_APP_ID_NOT_SUPPORTED                -83
#define ESM_CONFIG_WD_ENUM_NOT_SUPPORTED        -84
#define ESM_CONFIG_LOGGING_MODE_NOT_SUPPORTED   -85
#define ESM_CONFIG_LOGGING_LEVEL_NOT_SUPPORTED  -86
#define ESM_CONFIG_MB_SIZE_NOT_SUPPORTED        -87
#define ESM_CONFIG_LOGGING_SIZE_NOT_SUPPORTED   -88
#define ESM_CONFIG_SRM_SIZE_NOT_SUPPORTED       -89
#define ESM_CONFIG_PAIRING_SIZE_NOT_SUPPORTED   -90
#define ESM_CONFIG_EXCEPT_SIZE_NOT_SUPPORTED    -91
#define ESM_CONFIG_TOPO_SIZE_NOT_SUPPORTED      -92
#define ESM_CONFIG_TOPO_SEED_SIZE_NOT_SUPPORTED -93
#define ESM_CONFIG_CRYPTO_INIT_ERROR            -94
#define ESM_CONFIG_CRYPTO_FAILED                -95
#define ESM_CONFIG_VERIFY_FAILED                -96
#define ESM_CONFIG_ABORT_FAILED                 -97
#define ESM_CONFIG_MEM_SIZE_NOT_SUPPORTED       -99
#define ESM_CONFIG_FW_SYS_VERSION_NOT_SUPPORTED -99
#define ESM_CONFIG_FW_APP_VERSION_NOT_SUPPORTED -100

/* System errors. */
#define ESM_ERR_SYSTEM_HP2AE_IRQ_SETUP_FAILED      (ESM_SYSTEM_ERRORS - 0)
#define ESM_ERR_SYSTEM_RESET_FAILED                (ESM_SYSTEM_ERRORS - 1)
#define ESM_ERR_SYSTEM_RESEED_FAILED               (ESM_SYSTEM_ERRORS - 2)
#define ESM_ERR_SYSTEM_CPI_SKP_IRQ_SETUP_FAILED    (ESM_SYSTEM_ERRORS - 3)
#define ESM_ERR_SYSTEM_REQ_TIMER_IRQ_SETUP_FAILED  (ESM_SYSTEM_ERRORS - 4)
#define ESM_ERR_SYSTEM_REQ_WD_IRQ_SETUP_FAILED     (ESM_SYSTEM_ERRORS - 5)
#define ESM_ERR_SYSTEM_REQ_HEALTH_IRQ_SETUP_FAILED (ESM_SYSTEM_ERRORS - 6)

/* BSP platform errors. */
#define ESM_ERR_BSP_SYSTEM_INIT_FAILED (ESM_BSP_PLATFORM_ERRORS - 0)

#endif

