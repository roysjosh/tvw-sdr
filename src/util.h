
#ifndef UTIL_H
#define UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "tvw-sdr.h"

#define RR(dev, reg, val) tvw_read_reg(dev, reg, val)
#define WR(dev, reg, val) tvw_write_reg(dev, reg, val)
#define MR(dev, reg, mask, val) tvw_rmw_reg(dev, reg, mask, val)
#define XR(dev, reg, mask, cmp) tvw_reg_has_bits_set(dev, reg, mask, cmp)

#define RU(dev, reg, val) tvw_uc_read_reg(dev, reg, val)
#define WU(dev, reg, val) tvw_uc_write_reg(dev, reg, val)
#define MU(dev, reg, mask, val) tvw_uc_rmw_reg(dev, reg, mask, val)
#define XU(dev, reg, mask, cmp) tvw_uc_reg_has_bits_set(dev, reg, mask, cmp)

#define MSLEEP(ms) (usleep(ms * 1000) == 0)

#define ALL 0xffffffff
#define UC_ALL 0xff

/* wrapper for usleep */
int msleep(int msec);

/* low-level read/write functions */
int internal_read_reg(tvwsdr_dev_t *dev, uint16_t reg, uint16_t page, void *buf, uint16_t len);
int internal_write_reg(tvwsdr_dev_t *dev, uint16_t reg, uint16_t page, void *buf, uint16_t len);

/* read/write to TVW */
bool tvw_read_reg(tvwsdr_dev_t *dev, uint16_t reg, uint32_t *val);
bool tvw_write_reg(tvwsdr_dev_t *dev, uint16_t reg, uint32_t val);
bool tvw_rmw_reg(tvwsdr_dev_t *dev, uint16_t reg, uint32_t mask, uint32_t val);
bool tvw_reg_has_bits_set(tvwsdr_dev_t *dev, uint16_t reg, uint32_t mask, uint32_t cmp);

/* service call to TVW */
bool tvw_e0_magic(tvwsdr_dev_t *dev, uint32_t val2, uint32_t val1);

/* read/write to DSP (?) */
bool tvw_uc_read_reg(tvwsdr_dev_t *dev, uint16_t reg, uint8_t *val);
bool tvw_uc_write_reg(tvwsdr_dev_t *dev, uint16_t reg, uint8_t val);
bool tvw_uc_rmw_reg(tvwsdr_dev_t *dev, uint16_t reg, uint8_t mask, uint8_t val);
bool tvw_uc_reg_has_bits_set(tvwsdr_dev_t *dev, uint16_t reg, uint8_t mask, uint8_t cmp);

/* load firmware to both the TVW & DSP */
int tvw_load_fw(tvwsdr_dev_t *dev, const char *fn);

/* read/write to tuner behind TVW */
int tvw_read_i2c(tvwsdr_dev_t *dev, unsigned char *outbuf, uint8_t len);
int tvw_write_i2c(tvwsdr_dev_t *dev, unsigned char *wrbuf, uint8_t len);

#ifdef __cplusplus
}
#endif

#endif
