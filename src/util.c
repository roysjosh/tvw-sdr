
#include <endian.h>
#include <err.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include <libusb.h>

#include "tvw-reg.h"
#include "util.h"

#define BULK_TIMEOUT 0

int
msleep(int msec) {
	return usleep(msec * 1000);
}

int
internal_read_reg(tvwsdr_dev_t *dev, uint16_t reg, uint16_t page, void *buf, uint16_t len) {
	int ret, n_read;
	uint16_t flags = 0x0000, len_le, rlen = len;
	unsigned char tmpbuf[12] = {0};

	/* ugly hack for one register read following the firmware upload */
	if (reg == 0x0000 && page == 0x8000 && len == 0x0000) {
		flags = 0xbfc1;
		rlen = 4;
	}

	reg = htole16(reg);
	page = htole16(page);
	len_le = htole16(len);
	flags = htole16(flags);
	memcpy(tmpbuf + 0, &reg, 2);
	memcpy(tmpbuf + 2, &page, 2);
	memcpy(tmpbuf + 4, &len_le, 2);
	memcpy(tmpbuf + 6, &flags, 2);
	tmpbuf[7] |= (1 << 7);
	reg = ~reg;
	page = ~page;
	memcpy(tmpbuf + 8, &reg, 2);
	memcpy(tmpbuf + 10, &page, 2);

	/* write offset */
	ret = libusb_bulk_transfer(dev->devh, 0x03, tmpbuf, 12, &n_read, BULK_TIMEOUT);
	if (ret) {
		return ret;
	}

	/* read data */
	ret = libusb_bulk_transfer(dev->devh, 0x82, buf, rlen, &n_read, BULK_TIMEOUT);

	return ret;
}

int
internal_write_reg(tvwsdr_dev_t *dev, uint16_t reg, uint16_t page, void *buf, uint16_t len) {
	int ret, n_read;
	uint16_t len_le;
	unsigned char tmpbuf[12] = {0};

	reg = htole16(reg);
	page = htole16(page);
	len_le = htole16(len);
	memcpy(tmpbuf + 0, &reg, 2);
	memcpy(tmpbuf + 2, &page, 2);
	memcpy(tmpbuf + 4, &len_le, 2);
	tmpbuf[7] |= (0 << 7);
	reg = ~reg;
	page = ~page;
	memcpy(tmpbuf + 8, &reg, 2);
	memcpy(tmpbuf + 10, &page, 2);

	/* write offset */
	ret = libusb_bulk_transfer(dev->devh, 0x03, tmpbuf, 12, &n_read, BULK_TIMEOUT);
	if (ret) {
		return ret;
	}

	/* write data */
	ret = libusb_bulk_transfer(dev->devh, 0x03, buf, len, &n_read, BULK_TIMEOUT);

	return ret;
}

bool
tvw_read_reg(tvwsdr_dev_t *dev, uint16_t reg, uint32_t *val) {
	int ret;
	uint32_t tmp;

	ret = internal_read_reg(dev, reg, 0x2000, &tmp, 4);
	if (ret) {
		return false;
	}
	*val = le32toh(tmp);

	if (reg != 0x4400) {
		fprintf(stderr, "R %04x %08x\n", reg, *val);
	}

	return true;
}

bool
tvw_write_reg(tvwsdr_dev_t *dev, uint16_t reg, uint32_t val) {
	if (reg != 0x4400) {
		fprintf(stderr, "W %04x %08x\n", reg, val);
	}
	val = htole32(val);
	return internal_write_reg(dev, reg, 0x2000, &val, 4) == 0;
}

bool
tvw_rmw_reg(tvwsdr_dev_t *dev, uint16_t reg, uint32_t mask, uint32_t val) {
	uint32_t tmp;

	if (!tvw_read_reg(dev, reg, &tmp)) {
		return false;
	}

	fprintf(stderr, "RMW %04x R %08x W %08x\n", reg, tmp, (tmp & mask) | val);
	tmp &= mask;
	tmp |= val;

	return tvw_write_reg(dev, reg, tmp);
}

bool
tvw_reg_has_bits_set(tvwsdr_dev_t *dev, uint16_t reg, uint32_t mask, uint32_t cmp) {
	uint32_t val;

	if (!tvw_read_reg(dev, reg, &val)) {
		return false;
	}

	fprintf(stderr, "CMP %04x (%08x & %08x) == %08x ? %s\n", reg, val, mask, cmp, (val & mask) == cmp ? "true" : "false");

	return ((val & mask) == cmp);
}

bool
tvw_e0_magic(tvwsdr_dev_t *dev, uint32_t val2, uint32_t val1) {
	uint32_t tmp;
	unsigned char buf[16];

	if (internal_read_reg(dev, 0xe000, 0x1000, buf, 4)) {
		return false;
	}

	/* args for E000:
	 * buf[0] = service id
	 * buf[1] = ???, copied through
	 * buf[2-X] = input
	 */

	tmp = htole32(val1);
	memcpy(buf + 0, &val1, 4);
	tmp = htole32(val2);
	memcpy(buf + 4, &val2, 4);
	memset(buf + 8, 0, 8);
	if (internal_write_reg(dev, 0xe000, 0x1000, buf, 16)) {
		return false;
	}
	fprintf(stderr, "eW %08x%08x\n", *((uint32_t *)(buf + 4)), *((uint32_t *)buf));

	msleep(40);

	/* return in E010:
	 * buf[0] = service id, copied from E000
	 * buf[1] = ???, copied from E001
	 * buf[2] = return value (1 = bad?)
	 * buf[3-X] = output
	 */

	if (internal_read_reg(dev, 0xe010, 0x1000, buf, 16)) {
		return false;
	}
	fprintf(stderr, "eR %08x%08x\n", *((uint32_t *)(buf + 4)), *((uint32_t *)buf));
	memset(buf, 0, 4);
	if (internal_write_reg(dev, 0xe010, 0x1000, buf, 4)) {
		return false;
	}

	return true;
}

/* Register 0x4400 appears to be some sort of communication with a micro.
 * To read something from it, write 0x40XXYYZZ to 0x4400 where XXYY is the
 * register (which seems to start at 0xf000) and ZZ is 0x00.  Then, read
 * from 0x4400.  XXYY will be the same data written previously, but ZZ will
 * be the register's contents.
 * To write something, write 0x80XXYYZZ where XXYY is as before, but ZZ is
 * the data to write.
 */

bool
tvw_uc_read_reg(tvwsdr_dev_t *dev, uint16_t reg, uint8_t *val) {
	unsigned char buf[4];
	uint32_t ucval;

	union {
		uint32_t ucval;
		uint8_t buf[4];
	} x;

	x.buf[0] = 0x00;
	x.buf[1] = reg & 0xff;
	x.buf[2] = reg >> 8;
	x.buf[3] = 0x40;

	if (!tvw_write_reg(dev, 0x4400, htole32(x.ucval))) {
		return false;
	}

	if (!tvw_read_reg(dev, 0x4400, &x.ucval)) {
		return false;
	}

	*val = x.buf[0];

	fprintf(stderr, "uR %04x %02x\n", reg, *val);

	return true;
}

bool
tvw_uc_write_reg(tvwsdr_dev_t *dev, uint16_t reg, uint8_t val) {
	unsigned char buf[4];
	uint32_t ucval;

	union {
		uint32_t ucval;
		uint8_t buf[4];
	} x;

	x.buf[0] = val;
	x.buf[1] = reg & 0xff;
	x.buf[2] = reg >> 8;
	x.buf[3] = 0x80;

	fprintf(stderr, "uW %04x %02x\n", reg, val);

	return tvw_write_reg(dev, 0x4400, htole32(x.ucval));
}

bool
tvw_uc_rmw_reg(tvwsdr_dev_t *dev, uint16_t reg, uint8_t mask, uint8_t val) {
	uint8_t tmp;

	if (!tvw_uc_read_reg(dev, reg, &tmp)) {
		return false;
	}

	fprintf(stderr, "uRMW %04x R %02x W %02x\n", reg, tmp, (tmp & mask) | val);
	tmp &= mask;
	tmp |= val;

	return tvw_uc_write_reg(dev, reg, tmp);
}

bool
tvw_uc_reg_has_bits_set(tvwsdr_dev_t *dev, uint16_t reg, uint8_t mask, uint8_t cmp) {
	uint8_t val;

	if (!tvw_uc_read_reg(dev, reg, &val)) {
		return false;
	}

	fprintf(stderr, "CMP %04x (%02x & %02x) == %02x ? %s\n", reg, val, mask, cmp, (val & mask) == cmp ? "true" : "false");

	return ((val & mask) == cmp);
}

int
tvw_wait_bits(tvwsdr_dev_t *dev, uint16_t reg, uint16_t page, uint32_t mask, uint32_t val, int msec, int loop) {
	int count;
	uint32_t rval;

	for(count = 0; count < loop; count++) {
		if (internal_read_reg(dev, reg, page, &rval, sizeof(rval))) {
			return -1;
		}
		rval = le32toh(rval);
		if ((rval & mask) == val) {
			return 0;
		}
		msleep(msec);
	}

	return -1;
}

int
tvw_load_fw(tvwsdr_dev_t *dev, const char *fn) {
	char *buf;
	int fd;
	size_t flen, fpos;
	ssize_t rret;
	struct stat fileinfo;
	uint16_t wlen;

	fd = open(fn, O_RDONLY);
	if (fd < 0) {
		warn("failed to open %s", fn);
		return -1;
	}

	if (fstat(fd, &fileinfo)) {
		warn("failed to stat fd");
		return -1;
	}

	if (fileinfo.st_size <= 0) {
		warnx("invalid file size");
		return -1;
	}

	flen = (size_t)fileinfo.st_size;
	if (NULL == (buf = malloc(flen))) {
		warn("failed to allocate file buffer");
		return -1;
	}

	rret = read(fd, buf, flen);
	if (rret < 0 || (size_t)rret != flen) {
		warn("failed to read file");
		free(buf);
		return -1;
	}

	for(fpos = 0; fpos < flen; fpos += 512) {
		if (flen < 512 || flen - 512 < fpos) {
			wlen = flen - fpos;
		}
		else {
			wlen = 512;
		}
		if (internal_write_reg(dev, (uint16_t)fpos, 0x1001, buf + fpos, wlen)) {
			warnx("failed firmware transfer at 0x%04x", (uint16_t)fpos);
			free(buf);
			return -1;
		}
		if (flen - fpos == 512) {
			/* according to the USB spec, we need to send a 0 byte packet
			 * to terminate the transmission
			 */
			/* FIXME */
		}
	}

	free(buf);

	return 0;
}

int
tvw_read_i2c(tvwsdr_dev_t *dev, unsigned char *outbuf, uint8_t len) {
	uint8_t addr, count;
	uint32_t ctrl, tmpbuf;

	/* address discovery? */
	ctrl = ((0x0811 << TVW_I2C_PRESCALE_SHIFT) |
	        TVW_I2C_DONE |
	        TVW_I2C_NACK |
	        TVW_I2C_HALT |
	        TVW_I2C_SOFT_RST |
	        TVW_I2C_DRIVE_EN |
	        TVW_I2C_DRIVE_SEL |
	        TVW_I2C_START |
	        TVW_I2C_RECEIVE |
	        0);
	if (internal_write_reg(dev, TVW_I2C_CNTL_0, 0x2000, &ctrl, 4)) {
		return -1;
	}

	ctrl = 0;
	if (internal_write_reg(dev, TVW_I2C_CNTL_1, 0x2000, &ctrl, 4)) {
		return -1;
	}

	/* read discovered address? */
	if (internal_read_reg(dev, TVW_I2C_CNTL_0, 0x2000, &ctrl, 4)) {
		return -1;
	}
	addr = (uint8_t)ctrl;
	if (internal_write_reg(dev, TVW_I2C_CNTL_0, 0x2000, &ctrl, 4)) {
		return -1;
	}

	/* write transaction length */
	ctrl = (((len > 30 ? 30 : len) << TVW_I2C_DATA_COUNT_SHIFT) |
	        0x0100 |
	        (0x10 << TVW_I2C_TIME_LIMIT_SHIFT) |
	        0);
	if (internal_write_reg(dev, TVW_I2C_CNTL_1, 0x2000, &ctrl, 4)) {
		return -1;
	}

	/* write i2c address with "read" bit set */
	ctrl = addr | 1;
	if (internal_write_reg(dev, TVW_I2C_DATA, 0x2000, &ctrl, 4)) {
		return -1;
	}

	if (internal_read_reg(dev, TVW_I2C_CNTL_0, 0x2000, &ctrl, 4)) {
		return -1;
	}
	ctrl |= TVW_I2C_GO;
	if (internal_write_reg(dev, TVW_I2C_CNTL_0, 0x2000, &ctrl, 4)) {
		return -1;
	}

	/* wait for i2c ready? */
	if (tvw_wait_bits(dev, TVW_I2C_CNTL_0, 0x2000, TVW_I2C_GO, 0, 10, 100)) {
		return -1;
	}

	for(count = 0; count < len; count++) {
		if (count && count % 30 == 0) {
			/* ??? */
			if (internal_read_reg(dev, TVW_I2C_CNTL_0, 0x2000, &ctrl, 4)) {
				return -1;
			}
			ctrl &= ~TVW_I2C_START;
			if (internal_write_reg(dev, TVW_I2C_CNTL_0, 0x2000, &ctrl, 4)) {
				return -1;
			}
			if (tvw_wait_bits(dev, TVW_I2C_CNTL_0, 0x2000, TVW_I2C_DONE, 0, 10, 100)) {
				return -1;
			}

			/* ??? */
			ctrl |= TVW_I2C_STOP;
			if (internal_write_reg(dev, TVW_I2C_CNTL_0, 0x2000, &ctrl, 4)) {
				return -1;
			}

			/* ??? */
			if (internal_read_reg(dev, TVW_I2C_CNTL_1, 0x2000, &ctrl, 4)) {
				return -1;
			}
			ctrl &= ~0x0100;
			if (internal_write_reg(dev, TVW_I2C_CNTL_1, 0x2000, &ctrl, 4)) {
				return -1;
			}
			if (tvw_wait_bits(dev, TVW_I2C_CNTL_1, 0x2000, 0x0100, 0, 10, 100)) {
				return -1;
			}

			/* write remaining length */
			ctrl |= (len - count > 30 ? 30 : len - count);
			if (internal_write_reg(dev, TVW_I2C_CNTL_1, 0x2000, &ctrl, 4)) {
				return -1;
			}

			/* ??? */
			if (internal_read_reg(dev, TVW_I2C_CNTL_0, 0x2000, &ctrl, 4)) {
				return -1;
			}
			ctrl |= TVW_I2C_GO;
			if (internal_write_reg(dev, TVW_I2C_CNTL_0, 0x2000, &ctrl, 4)) {
				return -1;
			}
			if (tvw_wait_bits(dev, TVW_I2C_CNTL_0, 0x2000, TVW_I2C_GO, 0, 10, 100)) {
				return -1;
			}
		}

		if (internal_read_reg(dev, TVW_I2C_DATA, 0x2000, &tmpbuf, 4)) {
			return -1;
		}
		outbuf[count] = tmpbuf & 0xff;
	}

	return 0;
}

int
tvw_write_i2c(tvwsdr_dev_t *dev, unsigned char *wrbuf, uint8_t len) {
	uint8_t addr, count;
	uint32_t ctrl, tmpbuf;

	/* address discovery? */
	ctrl = ((0x0811 << TVW_I2C_PRESCALE_SHIFT) |
	        TVW_I2C_DONE |
	        TVW_I2C_NACK |
	        TVW_I2C_HALT |
	        TVW_I2C_SOFT_RST |
	        TVW_I2C_DRIVE_EN |
	        TVW_I2C_DRIVE_SEL |
	        TVW_I2C_START |
	        0);
	if (internal_write_reg(dev, TVW_I2C_CNTL_0, 0x2000, &ctrl, 4)) {
		return -1;
	}

	/* ??? */
	ctrl = 0;
	if (internal_write_reg(dev, TVW_I2C_CNTL_1, 0x2000, &ctrl, 4)) {
		return -1;
	}
	ctrl = ((0x10 << TVW_I2C_TIME_LIMIT_SHIFT) |
	        0);
	if (internal_write_reg(dev, TVW_I2C_CNTL_1, 0x2000, &ctrl, 4)) {
		return -1;
	}

	/* read discovered address? */
	if (internal_read_reg(dev, TVW_I2C_CNTL_0, 0x2000, &ctrl, 4)) {
		return -1;
	}
	addr = (uint8_t)ctrl;
	if (len <= 29) {
		ctrl |= TVW_I2C_STOP;
	}
	if (internal_write_reg(dev, TVW_I2C_CNTL_0, 0x2000, &ctrl, 4)) {
		return -1;
	}

	/* ??? */
	if (internal_read_reg(dev, TVW_I2C_CNTL_1, 0x2000, &ctrl, 4)) {
		return -1;
	}
	ctrl &= ~0x0000ff00;
	ctrl |= 0x00000100;
	if (internal_write_reg(dev, TVW_I2C_CNTL_1, 0x2000, &ctrl, 4)) {
		return -1;
	}

	/* write transaction length */
	if (internal_read_reg(dev, TVW_I2C_CNTL_1, 0x2000, &ctrl, 4)) {
		return -1;
	}
	ctrl |= (len > 29 ? 29 : len);
	if (internal_write_reg(dev, TVW_I2C_CNTL_1, 0x2000, &ctrl, 4)) {
		return -1;
	}

	/* write chip address */
	ctrl = addr;
	if (internal_write_reg(dev, TVW_I2C_DATA, 0x2000, &ctrl, 4)) {
		return -1;
	}

	tmpbuf = 0;
	for(count = 0; count < len; count++) {
		if (count && count % 29 == 0) {
			/* ??? */
			if (internal_read_reg(dev, TVW_I2C_CNTL_0, 0x2000, &ctrl, 4)) {
				return -1;
			}
			ctrl |= TVW_I2C_GO;
			if (internal_write_reg(dev, TVW_I2C_CNTL_0, 0x2000, &ctrl, 4)) {
				return -1;
			}
			if (tvw_wait_bits(dev, TVW_I2C_CNTL_0, 0x2000, TVW_I2C_GO, 0, 10, 100)) {
				return -1;
			}

			/* ??? */
			ctrl |= TVW_I2C_DONE | TVW_I2C_NACK | TVW_I2C_HALT;
			ctrl &= ~0x0000ff00;
			if (internal_write_reg(dev, TVW_I2C_CNTL_0, 0x2000, &ctrl, 4)) {
				return -1;
			}
			if (internal_read_reg(dev, TVW_I2C_CNTL_0, 0x2000, &ctrl, 4)) {
				return -1;
			}
			ctrl |= TVW_I2C_STOP;
			if (internal_write_reg(dev, TVW_I2C_CNTL_0, 0x2000, &ctrl, 4)) {
				return -1;
			}

			/* ??? */
			if (internal_read_reg(dev, TVW_I2C_CNTL_1, 0x2000, &ctrl, 4)) {
				return -1;
			}
			ctrl &= ~0x0100;
			if (internal_write_reg(dev, TVW_I2C_CNTL_1, 0x2000, &ctrl, 4)) {
				return -1;
			}
			if (tvw_wait_bits(dev, TVW_I2C_CNTL_1, 0x2000, 0x0100, 0, 10, 100)) {
				return -1;
			}

			/* write remaining length */
			ctrl |= (len - count > 29 ? 29 : len - count);
			if (internal_write_reg(dev, TVW_I2C_CNTL_1, 0x2000, &ctrl, 4)) {
				return -1;
			}
		}

		tmpbuf = wrbuf[count];
		if (internal_write_reg(dev, TVW_I2C_DATA, 0x2000, &tmpbuf, 4)) {
			return -1;
		}
	}

	/* ??? */
	if (internal_read_reg(dev, TVW_I2C_CNTL_0, 0x2000, &ctrl, 4)) {
		return -1;
	}
	ctrl |= TVW_I2C_GO;
	if (internal_write_reg(dev, TVW_I2C_CNTL_0, 0x2000, &ctrl, 4)) {
		return -1;
	}
	/* wait for i2c ready? */
	if (tvw_wait_bits(dev, TVW_I2C_CNTL_0, 0x2000, TVW_I2C_GO, 0, 10, 100)) {
		return -1;
	}

	return 0;
}
