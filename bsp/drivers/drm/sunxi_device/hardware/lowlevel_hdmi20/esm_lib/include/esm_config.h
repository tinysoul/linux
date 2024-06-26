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
#ifndef _ESM_CONFIG_H_
#define _ESM_CONFIG_H_

#ifdef ESM_BUILD_SC
#include "esm_sc_config.h"
#endif
#ifdef ESM_BUILD_SAUTH
#include "esm_sa_config.h"
#endif

#define ESM_CONFIG_HASH_ID_SIZE 8
#define ESM_CONFIG_MAX_SIZE     (512 + ESM_CONFIG_HASH_ID_SIZE)
#define ESM_CONFIG_COOKIE       0xAEC95743
#define ESM_CONFIG_VERSION      0x02

#define ESM_CONFIG_CEE_PROG_WOO_ENABLE 0x80

typedef struct {
	/* Exception handling. */
	uint8_t except_mode;
	/* Memory partition. */
	uint32_t ext_mem_total_size;
	/* flag to copy config data to ext mem
	 * 0 - no copy
	 * ESM_EXT_MEM_SIZE (512 + 8) is the minimum value */
	uint16_t ext_mem_cfgdata_size;
	/* mirror reg status in ext mem location
	 * 0 - disabled
	 * 4 - enabled (4 bytes) */
	uint16_t ext_mem_cpireg_size;

	uint16_t ext_mem_mb_size;
	uint16_t ext_mem_except_size;
	uint16_t ext_mem_logging_size;
	uint16_t ext_mem_srm_size;
	uint16_t ext_mem_topo_size;
	uint8_t  topo_slots;
	uint8_t  ext_mem_topo_seed_size;
	uint16_t ext_mem_pairing_size;
} esm_controls_internal;

typedef struct {
	/* Cookie. */
	uint32_t cookie;
	/* Firmware version. */
	uint32_t fw_version;
	/* Configuration version. */
	uint32_t config_version;
	/* Timer for watchdog in ms. */
	uint8_t watchdog_mode;
	/* Logging parameters */
	uint8_t logging_mode;
	uint8_t logging_level;
	/* Shared timer in ms. */
	uint16_t shared_timer_int_interval;
	/* Reseed timer in ms. */
	uint16_t reseed_timeout;
	/* Communication timeout in ms. */
	uint16_t comm_timeout;
	/* I2C frequency in kHz. */
	uint16_t i2c_freq;
	/* CEE wait time in us. */
	uint16_t cee_state_wait_time;
	/* CPU frequency. */
	uint32_t cpu_freq;

	/* Exception handling. */
	esm_controls_internal c_int;
} esm_controls;

#endif
