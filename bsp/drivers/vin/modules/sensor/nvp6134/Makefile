# SPDX-License-Identifier: GPL-2.0-or-later
# Makefile for the video capture/playback device drivers.
# Copyright 2010 Steven King <sfking@fdwdc.com>
#Copyright(c) 2022-2027 Allwinnertech Co., Ltd
# Each configuration option enables a list of files.
#ifeq ($(KERNELRELEASE),)
#export MMZ_SRC_BASE=$(PWD)
#endif

obj-$(CONFIG_SENSOR_NVP6134) := nvp6134.o
nvp6134-objs := csi_dev_nvp6134.o nvp6134c.o acp.o acp_firmup.o eq.o eq_common.o eq_recovery.o  video.o
#nvp6134_test_2-y := csi_dev_nvp6134.o nvp6134_2.o acp.o acp_firmup.o eq.o eq_common.o eq_recovery.o  video.o
EXTRA_CFLAGS += -DI2C_INTERNAL
ifeq ($(HIARCH),hi3520D)
EXTRA_CFLAGS += -DHI_GPIO_I2C
EXTRA_CFLAGS += -I$(PWD)/../gpio_i2c
endif
ifeq ($(HIARCH),hi3521a)
EXTRA_CFLAGS += -DHI_CHIP_HI3521A
#EXTRA_CFLAGS += -DI2C_INTERNAL
EXTRA_CFLAGS += -DHI_I2C
EXTRA_CFLAGS += -I$(REL_INC)
EXTRA_CFLAGS += -I$(PWD)/../hi_i2c
EXTRA_CFLAGS += $(DRV_CFLAGS)
endif
ifeq ($(HIARCH),hi3531)
EXTRA_CFLAGS += -DHI_GPIO_I2C
EXTRA_CFLAGS += -I$(PWD)/../gpio_i2c_8b
endif
ifeq ($(HIARCH),hi3531a)
EXTRA_CFLAGS += -DHI_CHIP_HI3531A
EXTRA_CFLAGS += -DI2C_INTERNAL
EXTRA_CFLAGS += -I$(REL_INC)
EXTRA_CFLAGS += $(DRV_CFLAGS)
endif
#EXTRA_CFLAGS += -DSUPPORT_HI3520D   #if defined HI3520D, if not defined Hi3521a
default:
	@make -C $(LINUX_ROOT) M=$(PWD) modules
ifeq ($(HIARCH),hi3520D)
	cp *.ko /nfs/hi3520d/app/ko/extdrv/
endif
ifeq ($(HIARCH),hi3521a)
	cp *.ko /nfs/hi3521a/app/ko/extdrv/
endif
ifeq ($(HIARCH),hi3531)
	cp *.ko /nfs/hi3531/ko/extdrv/
endif
ifeq ($(HIARCH),hi3531a)
	cp *.ko /nfs/hi3531a/app/ko/extdrv/
endif
clean:
	@make -C $(LINUX_ROOT) M=$(PWD) clean


