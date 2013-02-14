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

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "tvw-sdr.h"

int fdnum;

void
tvwsdr_xfer_cb(unsigned char *buf, unsigned int len, void *ctx) {
	if (write(fdnum, buf, len) < 0) {
		;
	}
	/* HACK */
	if (len & 1) {
		unsigned char zero[1] = { 0 };
		write(fdnum, &zero, 1);
	}
}

int
main() {
	tvwsdr_dev_t *dev;

	if (tvwsdr_open(&dev)) {
		tvwsdr_close(dev);
		return EXIT_FAILURE;
	}

	fdnum = creat("/dev/shm/tvw_sdr.bin", S_IRUSR | S_IWUSR);
	if (fdnum < 0) {
		err(EXIT_FAILURE, "open");
	}

	tvwsdr_read_async(dev, tvwsdr_xfer_cb, NULL);

	close(fdnum);

	tvwsdr_close(dev);

	return EXIT_SUCCESS;
}
