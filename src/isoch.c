/*
 * tvw-sdr, userspace driver for TVW750 devices
 * Copyright (C) 2014  Joshua Roys <roysjosh@gmail.com>
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
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int
main(int argc, char *argv[]) {
	int fd, j;
	off_t bufoff;
	size_t buflen = 0, framelen;
	ssize_t ret;
	unsigned char buf[8192];
	unsigned char *bufptr;

	if (argc < 2) {
		errx(EXIT_FAILURE, "<isoch data file>");
	}

	fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		err(EXIT_FAILURE, "open");
	}

	while ( (ret = read(fd, buf + buflen, sizeof(buf) - buflen)) > 0) {
		buflen += (size_t)ret;
		bufoff = 0;

		do {
			/* look in the newly read data for the EAV marker */
			bufptr = memchr(buf + bufoff, '\xff', buflen - bufoff);
			if (!bufptr) {
				break;
			}

			framelen = bufptr - (buf + bufoff);

			/* make sure that there is enough data available to scan for the EAV */
			if (framelen + 3 + 3 > buflen - bufoff) { /* EAV[2-4] + ANC header */
				break;
			}

			/* check for 2nd-4th bytes of EAV */
			if (bufptr[1] != 0x00 || bufptr[2] != 0x00 || (bufptr[3] & 0x10) != 0x10) {
				warnx("bad EAV: %02x %02x %02x %02x", bufptr[0], bufptr[1], bufptr[2], bufptr[3]);
				break;
			}

			/* check for ANC header */
			if (bufptr[4] != 0x00 || bufptr[5] != 0xff || bufptr[6] != 0xff) {
				warnx("bad ANC: %02x %02x %02x", bufptr[4], bufptr[5], bufptr[6]);
				break;
			}

			printf("%4zi: ", framelen);
			for (j = 0; j < 32; j++) {
				printf("%02x ", *(bufptr + j));
			}
			printf("\n");

			bufoff += framelen + 32;
		} while (1);

		buflen -= bufoff;
		memmove(buf, buf + bufoff, buflen);
	}
	if (ret < 0) {
		errx(EXIT_FAILURE, "read");
	}

	close(fd);

	return EXIT_SUCCESS;
}
