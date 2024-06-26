/*
 * Sbus interfaces for XRadio drivers
 *
 * Copyright (c) 2013
 * Xradio Technology Co., Ltd. <www.xradiotech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */


#ifndef __SBUS_H
#define __SBUS_H

#include <generated/uapi/linux/version.h>
#include <linux/module.h>
/*
 * sbus priv forward definition.
 * Implemented and instantiated in particular modules.
 */

struct xradio_common;
/*sdio bus private struct*/
#define SDIO_UNLOAD   0
#define SDIO_LOAD     1

#ifdef CONFIG_XRADIO_NON_POWER_OF_TWO_BLOCKSIZES

#define SDIO_BLOCK_SIZE (264)  /*264 = 528/2*/
#if (SDIO_BLOCK_SIZE & 0x3)
#error "SDIO_BLOCK_SIZE is not aligned of 4 byte"
#endif

#else

#define SDIO_BLOCK_SIZE (256)
#if ((SDIO_BLOCK_SIZE-1) & SDIO_BLOCK_SIZE)
#error "SDIO_BLOCK_SIZE is not power of 2"
#endif

#endif


typedef void (*sbus_irq_handler)(void *priv);
struct sbus_priv {
	struct sdio_func     *func;
	spinlock_t            lock;
	sbus_irq_handler      irq_handler;
	void                 *irq_priv;
	wait_queue_head_t     init_wq;
	int                   load_state;
	u32                  *val32_r;
	u32                  *val32_w;
};

struct sbus_ops {
	int (*sbus_data_read)(struct sbus_priv *self, unsigned int addr,
					void *dst, int count);
	int (*sbus_data_write)(struct sbus_priv *self, unsigned int addr,
					const void *src, int count);
	void (*lock)(struct sbus_priv *self);
	void (*unlock)(struct sbus_priv *self);
	size_t (*align_size)(struct sbus_priv *self, size_t size);
	int (*set_block_size)(struct sbus_priv *self, size_t size);
	size_t (*get_block_size)(struct sbus_priv *self);
	int (*irq_subscribe)(struct sbus_priv *self,
	      sbus_irq_handler handler, void *priv);
	int (*irq_unsubscribe)(struct sbus_priv *self);
	int (*power_mgmt)(struct sbus_priv *self, bool suspend);
	int (*reset)(struct sbus_priv *self);
};

#ifdef CONFIG_XRADIO_NON_POWER_OF_TWO_BLOCKSIZES
static inline unsigned xr_sdio_blksize_align(unsigned size)
{
	unsigned align_size = size%SDIO_BLOCK_SIZE;
	return align_size ? (size + SDIO_BLOCK_SIZE - align_size) : size;
}
#else
static inline unsigned xr_sdio_blksize_align(unsigned size)
{
	return (size + (SDIO_BLOCK_SIZE - 1)) &
			(~(SDIO_BLOCK_SIZE - 1));
}
#endif

static inline unsigned xr_sdio_align_broken_byte512(unsigned size)
{
	unsigned align_size = (size + 0x3) & 0x3;
	return (align_size < 512 ?
			align_size : xr_sdio_blksize_align(align_size));
}

/*sbus init functions*/
struct device *sbus_sdio_init(struct sbus_ops  **sdio_ops,
			       struct sbus_priv **sdio_priv);
void  sbus_sdio_deinit(void);

#endif /* __SBUS_H */
