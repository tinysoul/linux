# SPDX-License-Identifier: GPL-2.0-or-later
# Makefile for the video capture/playback device drivers.
# Copyright 2010 Steven King <sfking@fdwdc.com>
#Copyright(c) 2022-2027 Allwinnertech Co., Ltd
# Each configuration option enables a list of files.
#########################################################################

SUB_DIR = `find ../ -type d`

#########################################################################
#	Toolchain.
#########################################################################

CROSS_COMPILE := /opt/freescale/usr/local/gcc-4.6.2-glibc-2.13-linaro-multilib-2011.12/fsl-linaro-toolchain/bin/arm-fsl-linux-gnueabi-

CC:= $(CROSS_COMPILE)gcc
CPP:= $(CROSS_COMPILE)g++
AR:= $(CROSS_COMPILE)ar
AS:= $(CROSS_COMPILE)as
LD:= $(CROSS_COMPILE)ld
NM:= $(CROSS_COMPILE)nm
RANLIB:= $(CROSS_COMPILE)ranlib
OBJCOPY:= $(CROSS_COMPILE)objcopy
STRIP:= $(CROSS_COMPILE)strip

######################################################################
# NFS path
NFS_PATH = /nfs/$(shell whoami)/imx6

# Build options
######################################################################
# Target
DDOBJS += i2c_dd.c \
	strfunc.c \

DECOBJS += i2c_dec.c \
	strfunc.c \

DD := dd
DEC := dec

######################################################################
# Build

all:
	$(Q)$(CC) -g -Wall -o $(DD) $(DDOBJS)
	$(Q)$(CC) -g -Wall -o $(DEC) $(DECOBJS)

clean:
	@for SDIR in $(SUB_DIR) ; do	\
		rm -f $${SDIR}/*.o;			\
		rm -f $${SDIR}/*.bak;		\
		rm -f *.o;		\
	done;

	@rm -f $(DD)
	@rm -f $(DEC)

	@echo "Cleaning......   [OK]"

install: all
	cp -a $(DD) $(NFS_PATH)
	cp -a $(DEC) $(NFS_PATH)

#########################################################################
# Dependency
ifeq (.depend,$(wildcard .depend))
include .depend
endif

