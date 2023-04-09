/*
 * tvw-sdr, userspace driver for TVW750 devices
 * Copyright (C) 2012-2013  Joshua Roys <roysjosh@gmail.com>
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

#ifndef TVW_SDR_H
#define TVW_SDR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include <libusb.h>

#include "compat.h"
#include "tda18271.h"
#include "tda18271-priv.h"

/*
 * Callback function type for USB ISOCH data handling.
 */
typedef void(*tvwsdr_read_async_cb_t)(unsigned char *buf, unsigned int len, void *ctx);

typedef struct tvwsdr_dev {
	/* libusb info */
	libusb_context *ctx;
	struct libusb_device_handle *devh;
	struct libusb_transfer **xfer;
	unsigned char **xfer_buf;
	tvwsdr_read_async_cb_t cb;
	void *cb_ctx;
	/* tuner info */
	struct dvb_frontend fe;
	/* saved configuration information */
	uint32_t freq;
	/* isoch work buffer */
	unsigned char work_buf[8*1024];
	unsigned int work_buflen;
} tvwsdr_dev_t;

/*
 * Acquire a device handle.
 */
int tvwsdr_open(tvwsdr_dev_t **dev, uint32_t freq);

/*
 * Close and free a device handle.
 */
int tvwsdr_close(tvwsdr_dev_t *dev);

/*
 * Initiate USB ISOCH data transfers.
 */
int tvwsdr_read_async(tvwsdr_dev_t *dev, tvwsdr_read_async_cb_t cb, void *ctx);

#ifdef __cplusplus
}
#endif

#endif
