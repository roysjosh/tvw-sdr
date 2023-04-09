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
#include <unistd.h>

#include "tvw-sdr.h"

void
tvwsdr_xfer_cb(unsigned char *buf, unsigned int len, void *ctx) {
	uint16_t frame_id;

	// skip timing reference codes & ancillary header (8 bit)
	// FF 00 00 XY (BT.656 SAV/EAV)
	// 00 FF FF (BT.1364 ancillary data flag [ADF])
	// 00 00 (BT.1364 data identification [DID], data block/secondary data ID)
	// 10 (BT.1364 data count)
	// XX YY (non-standard frame ID)
	// ... data bytes of length <data count>
	frame_id = buf[10] << 8 | buf[11];
	if (buf[12] != 0 && buf[13] != 0 && buf[14] != 0 && buf[15] != 0) {
#if 0
		if (frame_id % 0x100 == 0) {
			fprintf(stderr, "%04hx\n", frame_id);
		}
#endif
		write(STDOUT_FILENO, buf + 12, buf[9]);
	}
}

int
main(int argc, char *argv[]) {
	char *ptr;
	uint32_t freq, khz, mhz;
	tvwsdr_dev_t *dev;

	if (argc > 1) {
		mhz = strtol(argv[1], &ptr, 10);
		if (argv[0] == ptr && mhz == 0) {
			warnx("invalid frequency: not a number");
			return EXIT_FAILURE;
		}
		if (*ptr == '\0') {
			// assume user provided hz
			freq = mhz;
		} else if (*ptr == '.') {
			khz = strtol(ptr + 1, NULL, 10);
			freq = mhz * 1000000 + khz * 100000;
		} else {
			warnx("invalid frequency");
			return EXIT_FAILURE;
		}
	} else {
		freq = 96100000;
	}

	fprintf(stderr, "freq = %u\n", freq);

	if (tvwsdr_open(&dev, freq)) {
		tvwsdr_close(dev);
		return EXIT_FAILURE;
	}

	tvwsdr_read_async(dev, tvwsdr_xfer_cb, NULL);

	tvwsdr_close(dev);

	return EXIT_SUCCESS;
}
