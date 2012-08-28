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
#include <stdio.h>
#include <stdlib.h>

#include <libusb.h>

struct libusb_device_handle *devh = NULL;

#define NUM_ISOCH_XFERS 15
struct libusb_transfer *isoch_xfers[NUM_ISOCH_XFERS];
unsigned char xferbuf[3*1024 * 32];
int async_status = 0;

static void LIBUSB_CALL
tvwsdr_xfer_cb(struct libusb_transfer *xfer) {
	unsigned int i;

	if (async_status) {
		return;
	}

	for(i = 0; i < 32; i++) {
		printf("%u:%u ", i, xfer->iso_packet_desc[i].actual_length);
	}
	printf("\n");

	if (LIBUSB_TRANSFER_COMPLETED == xfer->status) {
		libusb_submit_transfer(xfer);
	}
}

int
tvwsdr_start_async() {
	unsigned int i;

	for(i = 0; i < sizeof(isoch_xfers) / sizeof(struct libusb_transfer *); i++) {
		isoch_xfers[i] = libusb_alloc_transfer(32);
		libusb_fill_iso_transfer(isoch_xfers[i], devh, 0x81, xferbuf, sizeof(xferbuf), 32, tvwsdr_xfer_cb, NULL, 1000);
		libusb_set_iso_packet_lengths(isoch_xfers[i], 3*1024);
		libusb_submit_transfer(isoch_xfers[i]);
	}

	return 0;
}

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

	tvwsdr_start_async();
	tvwsdr_init7();
	while (libusb_handle_events(NULL) >= 0) {
		;
	}
	async_status = 1;
	sleep(2);

	libusb_release_interface(devh, 0);
	libusb_close(devh);
	libusb_exit(NULL);

	return 0;
}
