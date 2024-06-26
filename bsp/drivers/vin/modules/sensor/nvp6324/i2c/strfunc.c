/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright(c) 2020 - 2023 Allwinner Technology Co.,Ltd. All rights reserved. */
/*
 * A V4L2 driver for nvp6324 cameras and AHD Coax protocol.
 *
 * Copyright (c) 2017 by Allwinnertech Co., Ltd.  http://www.allwinnertech.com
 *
 * Authors:  Li Huiyu <lihuiyu@allwinnertech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <stdio.h>
#include <ctype.h>
#include "strfunc.h"

#define ASC2NUM(ch) (ch - '0')
#define HEXASC2NUM(ch) (ch - 'A' + 10)

static int atoul(char *str, unsigned int *pulValue);
static int atoulx(char *str, unsigned int  pulValue);

int StrToNumber(char *str, unsigned int *pulValue)
{
	if (*str == '0' && (*(str+1) == 'x' || *(str+1) == 'X')) {
		if (*(str+2) == '\0') {
			return -1;
		} else {
			return atoulx(str+2, pulValue);
		}
	} else {
		return atoulx(str, pulValue);
	}
}

static int atoul(char *str, unsigned int *pulValue)
{
	unsigned int ulResult = 0;

	while (*str) {
		if (isdigit((int)*str)) {
			if ((ulResult < 429496729) || ((ulResult == 429496729) && (*str < '6'))) {
				ulResult = ulResult*10 + (*str)-48;
			} else {
				*pulValue = ulResult;
				return -1;
			}
		} else {
			*pulValue = ulResult;
			return -1;
		}
		str++;
	}
	*pulValue = ulResult;
	return 0;
}

int  atoulx(char *str, unsigned int *pulValue)
{
	unsigned int   ulResult = 0;
	unsigned char ch;

	while (*str) {
		ch = toupper(*str);
		if (isdigit(ch) || ((ch >= 'A') && (ch <= 'F'))) {
			if (ulResult < 0x10000000) {
				ulResult = (ulResult << 4) + ((ch <= '9')?(ASC2NUM(ch)):(HEXASC2NUM(ch)));
			} else {
				*pulValue = ulResult;
				return -1;
			}
		} else {
			*pulValue = ulResult;
			return -1;
		}
		str++;
	}

	*pulValue = ulResult;
	return 0;
}
