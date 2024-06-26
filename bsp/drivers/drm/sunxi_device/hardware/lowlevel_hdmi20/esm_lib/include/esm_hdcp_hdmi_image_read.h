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
#ifndef _ESM_HDCP_HDMI_IMAGE_READ_H_
#define _ESM_HDCP_HDMI_IMAGE_READ_H_

#include "esm_errors.h"
#include "esm_system.h"
#include "esm_exceptions.h"
#include "esm_config.h"

uint32_t elliptic_byte_get(uint8_t *image, uint32_t const value_size);

ELP_STATUS esm_image_read(uint8_t *config_table, uint32_t config_table_size,
			uint8_t app_id, uint8_t *ctrl, esm_controls *sys_c,
			void *app_c, uint32_t fw_version, uint32_t *offset);
ELP_STATUS esm_image_read_tx(uint8_t *config_table, void *app_c,
						uint32_t *offset);

#endif /* _ESM_HDCP_HDMI_IMAGE_READ_H_ */
