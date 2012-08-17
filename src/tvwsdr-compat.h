/*
 * tvw-sdr, userspace driver for TVW750 devices
 * Copyright (C) 2012  Joshua Roys <roysjosh@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef TVWSDR_COMPAT_H
#define TVWSDR_COMPAT_H

#include <errno.h>
#include <linux/dvb/frontend.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "libtvwsdr.h"

#define msleep tvwsdr_msleep
#define udelay usleep

#define pr_info printf
#define printk printf
#define KERN_EMERG	"<0>"
#define KERN_ALERT	"<1>"
#define KERN_CRIT	"<2>"
#define KERN_ERR	"<3>"
#define KERN_WARNING	"<4>"
#define KERN_NOTICE	"<5>"
#define KERN_INFO	"<6>"
#define KERN_DEBUG	"<7>"

typedef int8_t   s8;
typedef int16_t  s18;
typedef int32_t  s32;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;

#define false 0
#define true (!false)

struct dvb_frontend {
	void *tuner_priv;
};

struct i2c_adapter {
};

#endif
