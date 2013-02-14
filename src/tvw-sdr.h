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

typedef struct tvwsdr_dev tvwsdr_dev_t;

/*
 * Acquire a device handle.
 */
int tvwsdr_open(tvwsdr_dev_t **dev);

/*
 * Close and free a device handle.
 */
int tvwsdr_close(tvwsdr_dev_t *dev);

/*
 * Get the currently tuned frequency.
 */
uint32_t tvwsdr_get_center_freq(tvwsdr_dev_t *dev);

/*
 * Set the currently tuned frequency.
 */
int tvwsdr_set_center_freq(tvwsdr_dev_t *dev, uint32_t freq);

/*
 * Callback function type for USB ISOCH data handling.
 */
typedef void(*tvwsdr_read_async_cb_t)(unsigned char *buf, unsigned int len, void *ctx);

/*
 * Initiate USB ISOCH data transfers.
 */
int tvwsdr_read_async(tvwsdr_dev_t *dev, tvwsdr_read_async_cb_t cb, void *ctx);

/*
 * Set device sample rate.
 */
int tvwsdr_set_sample_rate(tvwsdr_dev_t *dev, uint32_t rate);

#ifdef __cplusplus
}
#endif

#endif
