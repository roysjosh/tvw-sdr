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

#include <err.h>

#include <libusb.h>
#include <stdlib.h>

struct libusb_device_handle *devh = NULL;

int
main() {
	libusb_init(NULL);

	devh = libusb_open_device_with_vid_pid(NULL, 0x0438, 0xac14);
	if (!devh) {
		err(EXIT_FAILURE, "failed to open usb device");
	}

	if (libusb_claim_interface(devh, 0)) {
		err(EXIT_FAILURE, "failed to claim interface 0");
	}

	//dump_regs(0x2000, 0x0800, 0x00ff);

	tvwsdr_init();

	libusb_release_interface(devh, 0);
	libusb_close(devh);
	libusb_exit(NULL);

	return 0;
}
