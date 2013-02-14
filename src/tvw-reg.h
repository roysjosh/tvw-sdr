/*
 * tvw-sdr, userspace driver for TVW750 devices
 * Copyright (C) 2013  Joshua Roys <roysjosh@gmail.com>
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

#ifndef TVWREG_H
#define TVWREG_H

/* register definitions more or less from linux/drivers/gpu/drm/radeon/radeon_reg.h */
#define TVW_I2C_CNTL_0				0x03e0
#define   TVW_I2C_DONE				(1 << 0)
#define   TVW_I2C_NACK				(1 << 1)
#define   TVW_I2C_HALT				(1 << 2)
#define   TVW_I2C_SOFT_RST			(1 << 5)
#define   TVW_I2C_DRIVE_EN			(1 << 6)
#define   TVW_I2C_DRIVE_SEL			(1 << 7)
#define   TVW_I2C_START				(1 << 8)
#define   TVW_I2C_STOP				(1 << 9)
#define   TVW_I2C_RECEIVE			(1 << 10)
#define   TVW_I2C_ABORT				(1 << 11)
#define   TVW_I2C_GO				(1 << 12)
#define   TVW_I2C_PRESCALE_SHIFT		16
#define TVW_I2C_CNTL_1				0x03e4
#define   TVW_I2C_DATA_COUNT_SHIFT		0
/* the definition below is likely not correct;
 * we can have data lengths larger than 4 bits can hold
 */
#define   TVW_I2C_ADDR_COUNT_SHIFT		4
#define   TVW_I2C_INTRA_BYTE_DELAY_SHIFT	8
#define   TVW_I2C_SEL				(1 << 16)
#define   TVW_I2C_EN				(1 << 17)
#define   TVW_I2C_TIME_LIMIT_SHIFT		24
#define TVW_I2C_DATA				0x03e8

#endif
