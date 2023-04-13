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

#include <endian.h>
#include <err.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "tvw-reg.h"
#include "tvw-sdr.h"
#include "util.h"

#define NUM_ISOCH_XFERS 15
#define NUM_ISOCH_PACKETS 32

bool
tvw_init_tvw_1(tvwsdr_dev_t *dev) {
	return
			XR(dev, TVW_IC_STATUS, 0xf00000, 0x100000) &&
			WR(dev, 0x08a8, 0x0000f0a0) &&
			WR(dev, 0x08ac, 0x0000f0a8) &&
			WR(dev, TVW_MPLL_CONTROL, 0x0001a008) &&
			WR(dev, TVW_FPLL_CONTROL, 0x87000024) &&
			WR(dev, TVW_XPLL_CONTROL, 0x0018c600) &&
			WR(dev, TVW_VPLL_CONTROL_0, 0x00c00000) &&
			WR(dev, TVW_VPLL_CONTROL_1, 0x00000001) &&
			WR(dev, 0x0938, 0x0001a008) &&
			WR(dev, 0x093c, 0x87000024) &&
			WR(dev, 0x0940, 0x004a6400) &&
			WR(dev, 0x0944, 0x33950000) &&
			WR(dev, 0x0948, 0x000ad437) &&
			WR(dev, TVW_CLOCK_CONTROL_0, 0x008ffb3f) &&
			WR(dev, TVW_CLOCK_CONTROL_1, 0x22070155) &&
			WR(dev, TVW_RESET_CONTROL, 0x00000200) &&
			XR(dev, TVW_RESET_CONTROL, ALL, 0x00000200) &&
			WR(dev, TVW_USC_CTL, 0x00600000) &&
			WR(dev, TVW_RESET_CONTROL, 0x00000000) &&
			WR(dev, TVW_RESET_CONTROL, 0x00000180) &&
			MR(dev, TVW_RESET_CONTROL, ALL, 0x10) &&
			MR(dev, TVW_MPLL_CONTROL, 0xffffffef, 0) &&
			MR(dev, TVW_MPLL_CONTROL, 0xffffffec, 0) &&
			XR(dev, TVW_MPLL_CONTROL, ALL, 0x0001a008) &&
			MR(dev, 0x0938, 0xffffffef, 0) &&
			MR(dev, 0x0938, 0xffffffec, 0) &&
			WR(dev, TVW_RBBM_CTRL, 0x00000001) &&
			WR(dev, TVW_CLOCK_CONTROL_1, 0x00070155) &&
			WR(dev, TVW_CLOCK_CONTROL_0, 0x00fffa37) &&
			MR(dev, TVW_RESET_CONTROL, 0xffffffef, 0) &&
			MR(dev, TVW_CPU_HALF_BAND, 0, 0x80048004) &&
			WR(dev, TVW_GPIO_PORT_CONTROL, 0x07ff0000) &&
			WR(dev, 0x088c, 0x0001cfff) &&
			WR(dev, TVW_DVSO_PORT_CONTROL, 0x07ff0000) &&
			WR(dev, TVW_TSI_PORT_CONTROL, 0x07ff0000) &&
			WR(dev, TVW_AGC_PORT_CONTROL, 0x23ff0000) &&
			WR(dev, TVW_PCI_PORT_CONTROL, 0x07ff0000) &&
			WR(dev, 0x096c, 0x00014eff) &&
			//XR(dev, 0x00a8, ALL, 0x00000000) &&

#if 1
			MR(dev, TVW_AVBS1R_SETTINGS, ALL, 0x00000004) &&
			WR(dev, TVW_AVBS1R_OFFSET, 0x00013400) &&
			WR(dev, TVW_AVBS1R_BUF_SIZE, 0x00001c00) &&
			WR(dev, TVW_AVBS1W_OFFSET, 0x00013400) &&
			WR(dev, TVW_AVBS1W_BUF_SIZE, 0x00001c00) &&
			WR(dev, TVW_AVBS2R_OFFSET, 0x00015000) &&
			WR(dev, TVW_AVBS2R_BUF_SIZE, 0x00001c00) &&
			WR(dev, TVW_AVBS2W_OFFSET, 0x00015000) &&
			WR(dev, TVW_AVBS2W_BUF_SIZE, 0x00001c00) &&
			MR(dev, TVW_FEAW_SETTINGS, ALL, 0x00001000) &&
			WR(dev, TVW_FEAW_OFFSET, 0x00012400) &&
			MR(dev, TVW_RAR_SETTINGS, ALL, 0x00001000) &&
			WR(dev, TVW_RAR_BUF_SIZE, 0x00001000) &&
			WR(dev, TVW_RAR_OFFSET, 0x00012400) &&
			MR(dev, TVW_FEVW_SETTINGS, ALL, 0x00001800) &&
			WR(dev, TVW_FEVW_FIELDS_INT_THR, 0x00000001) &&
			WR(dev, TVW_DRS_CONTROL, 0x00000012) &&
			WR(dev, TVW_FEVW_LUM_INT_OFFSET_1, 0x00010c00) &&
			WR(dev, TVW_FEVW_LUM_INT_PITCH, 0x00300000) &&
			MR(dev, TVW_FEVW_SETTINGS, 0xffffffef, 0) &&
			WR(dev, TVW_FEVW_N_FRAME_BUFS, 0x00000003) &&
			WR(dev, TVW_FEVW_CHR_INT_OFFSET_1, 0x00011800) &&
			WR(dev, TVW_FEVW_CHR_INT_PITCH, 0x00300000) &&
			WR(dev, TVW_FEVW_VBI_PITCH, 0x00000000) &&
			MR(dev, TVW_RVR_SETTINGS, ALL, 0x00001000) &&
			WR(dev, TVW_RVR_LUM_INT_OFFSET_1, 0x00010c00) &&
			WR(dev, TVW_RVR_LUM_INT_PITCH, 0x00300000) &&
			MR(dev, TVW_RVR_SETTINGS, 0xffffffbf, 0) &&
			WR(dev, TVW_RVR_N_FRAME_BUFS, 0x00000003) &&
			WR(dev, TVW_RVR_CHR_INT_OFFSET_1, 0x00011800) &&
			WR(dev, TVW_RVR_CHR_INT_PITCH, 0x00300000) &&
			WR(dev, TVW_RVR_VBI_PITCH, 0x00000000) &&
#endif

			// turn on tuner?
			MR(dev, TVW_GPIO_PORT_CONTROL, ALL, 0x00000001) &&

#if 1
			// XXX
			WR(dev, TVW_DVSO_CRTC_VLINE_INTR, 0x03ff03ff) &&

			MR(dev, TVW_USC_CTL, ALL, 0x00101100) &&
			WR(dev, TVW_USC_ANC_WIDTH, 0x00100000) &&
			WR(dev, TVW_USC_VB_START, 0x00020002) &&
			WR(dev, TVW_USC_VB_FINISH, 0x000e000e) &&
			WR(dev, TVW_USC_VD_START, 0x000f000f) &&
			WR(dev, TVW_USC_VD_FINISH, 0x00ff00ff) &&

			MR(dev, TVW_DRS_CONTROL, ALL, 0) && // XXX
			MR(dev, TVW_DRS_CONTROL, ALL, 0) && // XXX

			WR(dev, TVW_DVSO_ANC_ID, 0x01014142) &&
			MR(dev, TVW_DVSO_VIP2_CTL2, ALL, 0) && // XXX
			MR(dev, TVW_DVSO_VIP2_CTL, ALL, 0) && // XXX

			MR(dev, TVW_VIDEO_ADC_0_CONTROL, ALL, 0) && // XXX
			MR(dev, TVW_VIDEO_ADC_1_CONTROL, ALL, 0) && // XXX
			MR(dev, TVW_RESET_CONTROL, ALL, 0) && // XXX

			//MR(dev, 0x0938, ALL, 0x00000013) &&
			//MR(dev, 0x0940, 0xffffd6ff, 0x0000d600) &&
			//MR(dev, 0x0948, )
			WR(dev, 0x0938, 0x0001a01b) &&
			WR(dev, 0x0940, 0x004ad600) &&
			WR(dev, 0x0944, 0x33950000) &&
			WR(dev, 0x0948, 0x0002dc37) &&
			WR(dev, 0x0938, 0x0001a00b) &&
			WR(dev, 0x0938, 0x0001a008) &&

			XR(dev, TVW_IC_STATUS, 0xffffffff, 0x00100797) && // XXX

			MR(dev, TVW_DVSO_PORT_CONTROL, ALL, 0) && // XXX
			MR(dev, TVW_AUD_SCRAMBLE_CONTROL, ALL, 0) && // XXX

			MR(dev, TVW_RESET_CONTROL, 0xffffff7f, 0) &&
			MR(dev, TVW_AVBS2W_SETTINGS, ALL, 0) && // XXX
			MR(dev, TVW_TSI_PORT_CONTROL, ALL, 0x00000001) &&
			XR(dev, TVW_IC_STATUS, 0xffffffff, 0x00100797) && // XXX
			MR(dev, TVW_RESET_CONTROL, ALL, 0x00000080) &&
			//MR(dev, TVW_RESET_CONTROL, 0xfffff000, 0) &&
			WR(dev, TVW_RESET_CONTROL, 0) &&
			WR(dev, TVW_ADEC_CONTROL, 0x000004c7) &&

			XR(dev, TVW_IC_STATUS, 0xffffffff, 0x00100797) && // XXX
			MR(dev, TVW_RESET_CONTROL, ALL, 0) && // XXX
#endif

			true;
}

bool
tvw_init_tvw_2(tvwsdr_dev_t *dev, int stage) {
	return
#if 1
			MR(dev, TVW_CLOCK_CONTROL_0, 0xff7ffffd, 0) &&
			MR(dev, TVW_VIDEO_ADC_0_CONTROL, ALL, 0x00000004) &&
			MR(dev, TVW_VIDEO_ADC_1_CONTROL, ALL, 0x00000004) &&
			MR(dev, 0x08a8, ALL, 0x00000100) &&
			MR(dev, TVW_CLOCK_CONTROL_0, ALL, 0x00800002) &&
			// XXX
			XR(dev, TVW_RESET_CONTROL, 0, 0) && // XXX
			XR(dev, TVW_RESET_CONTROL, 0, 0) && // XXX
			XR(dev, TVW_RESET_CONTROL, 0, 0) && // XXX
			XR(dev, TVW_RESET_CONTROL, 0, 0) && // XXX
			XR(dev, TVW_RESET_CONTROL, 0, 0) && // XXX
			MR(dev, TVW_RESET_CONTROL, 0xfffffdff, 0) &&
			XR(dev, TVW_RESET_CONTROL, 0, 0) && // XXX
			XR(dev, TVW_RESET_CONTROL, 0, 0) && // XXX
			XR(dev, TVW_RESET_CONTROL, 0, 0) && // XXX
			XR(dev, TVW_RESET_CONTROL, 0, 0) && // XXX
			XR(dev, TVW_RESET_CONTROL, 0, 0) && // XXX
			MR(dev, TVW_CPU_HALF_BAND, ALL, 0x80048004) &&

			// disable something
			(stage == 2 ? tvw_e0_magic(dev, 0x00000000, 0x00000150) : true) &&

			WR(dev, 0x6860, 0x00004080) &&
			WR(dev, 0x6868, 0x0000000c) &&
			WR(dev, TVW_PEAK_WHITE, 0x00000580) &&
			WR(dev, 0x6864, 0x00000000) &&
			WR(dev, 0x6868, 0x0000020c) &&
			WR(dev, 0x686c, 0x00000410) &&
			WR(dev, TVW_HLOCK_ACQUIRE_REC_EDGE, 0x00000002) &&
			WR(dev, 0x6870, 0x00000000) &&
			WR(dev, 0x6874, 0x00000000) &&
			WR(dev, 0x6878, 0x00000000) &&
			WR(dev, TVW_HLOCK_ACQUIRE_LOW_LL_WIN, 0x00001c1a) &&
			WR(dev, TVW_HLOCK_ACQUIRE_LOW_LOST, 0x00000516) &&
			WR(dev, 0x687c, 0x00000000) &&
			WR(dev, 0x6880, 0x10000000) &&
			WR(dev, 0x6884, 0x00000000) &&
			WR(dev, 0x6888, 0x00000000) &&
			WR(dev, 0x688c, 0x00000000) &&
			WR(dev, TVW_SYNC_STRIP_TP_CTRL, 0x00000000) &&
			WR(dev, TVW_HLOCK_ACQUIRE_REC_LL, 0x00000000) &&
			WR(dev, TVW_HLOCK_ACQUIRE_SYNC_SEL, 0x00000002) &&
			WR(dev, 0x6890, 0x00000000) &&
			WR(dev, TVW_RESAMP_COUT_SHFT, 0x00000000) &&
			WR(dev, 0x6894, 0xfb000000) &&
			WR(dev, 0x689c, 0x00000000) &&
			WR(dev, TVW_RESAMP_YOUT_SCALE, 0x00000000) &&
			WR(dev, TVW_MEASURE_SYNCTIP_TH1, 0x00000000) &&
			WR(dev, 0x68a0, 0xe8003000) &&
			WR(dev, TVW_MEASURE_SYNCTIP_BP_LP, 0x00005800) &&
			WR(dev, TVW_HLOCK_ACQUIRE_REC_LOST, 0x00008302) &&
			WR(dev, TVW_HLOCK_ACQUIRE_FOUND, 0x00008070) &&
			WR(dev, TVW_CB_PLL_ACC7, 0x00000040) &&
			WR(dev, TVW_AGC_CONTROL_4_5, (stage == 1 ? 0x81008000 : 0x84008000)) &&
			WR(dev, TVW_AGC_CONTROL_6_7, 0x00000000) &&
			WR(dev, TVW_AGC_ACC_MODE, 0x00000000) &&
			WR(dev, TVW_FRAME_SEN_CTRL, 0x00000000) &&
			WR(dev, TVW_VBI_ADDR, 0x00000000) &&
			WR(dev, TVW_VBI_RD_DATA, 0x00000000) &&
			WR(dev, TVW_LAST_TBC_DB7, 0x00c27f44) &&
			WR(dev, TVW_DEBUG_CONV_1_2, 0x003e0000) &&
			WR(dev, TVW_DEBUG_CONV_3_4, 0x00000007) &&
			WR(dev, TVW_VDEC_MEM_EN, 0x00000000) &&
			WR(dev, TVW_LINE_COUNT4, 0x00001000) &&
			WR(dev, TVW_GAIN_AGC, 0x00000000) &&
			WR(dev, TVW_BURST_AMP, 0x00000000) &&
			WR(dev, TVW_NEW_COMB_CTRL, 0x00008032) &&
			WR(dev, TVW_HLOCK_ACQUIRE_SYNC_LOW_TH, 0x87f507f8) &&
			WR(dev, TVW_HLOCK_ACQUIRE_SYNC_REC_TH, 0x00000000) &&
			WR(dev, TVW_MEASURE_SYNCTIP_SYNC_REC_CONTROL, 0x00000000) &&
			WR(dev, TVW_MEASURE_SYNCTIP_TH0, 0x00000000) &&
			WR(dev, TVW_D2_COEFF_3_2, 0x0fe01000) &&
			WR(dev, TVW_D2_COEFF_5_4, 0x0ff00ff0) &&
			WR(dev, TVW_D2_COEFF_7_6, 0x0fe00000) &&
			WR(dev, 0x68c0, 0x3c000000) &&
			WR(dev, TVW_D2_COEFF_9_8, 0x00000000) &&
			WR(dev, TVW_D2_COEFF_11_10, 0x00000000) &&
			WR(dev, TVW_D2_COEFF_13_12, 0x00000000) &&
			WR(dev, 0x68c4, 0x00000000) &&
			WR(dev, TVW_LAST_TBC_DB5_6, 0x00000000) &&
			WR(dev, TVW_CONTROL_1D1_2, 0x00001000) &&
			WR(dev, TVW_CONTROL_1D3_4, 0x00001000) &&
			WR(dev, TVW_CONTROL_TWO, 0x00000020) &&
			WR(dev, TVW_CBPLL_PROG, 0x10089180) &&
			WR(dev, TVW_CBCR_OFFSET, 0x00000080) &&
			WR(dev, TVW_DEBUG_READ, 0x0e00bf00) &&
			WR(dev, TVW_SECAM_CTRL, 0x00508650) &&
			WR(dev, TVW_LAST_TBC_DB1_2, 0x00000000) &&
			WR(dev, TVW_D2_COEFF_15_14, 0x00000000) &&
			WR(dev, TVW_CB_PLL_ACC5_6, 0x009320ac) &&
			WR(dev, TVW_CB_PLL_ACC3_4, 0x00000280) &&
			WR(dev, TVW_CB_PLL_TEST_9_10, 0x000001a3) &&
			WR(dev, TVW_CB_PLL_ACC1_2, 0x02800080) &&
			WR(dev, TVW_CB_PLL_TEST_5_6, 0x00100010) &&
			WR(dev, TVW_CB_PLL_TEST_3_4, 0xfc3ffc3f) &&
			WR(dev, TVW_RISING_PROG1_2, 0x87000674) &&
			WR(dev, TVW_TBC_BP_ST3, 0x00000007) &&
			WR(dev, TVW_RISING_PROG3, 0x01010406) &&
			WR(dev, TVW_TBC_BP_ST1_2, 0x80c4782e) &&
			WR(dev, TVW_COMB_3D_ADJUST, 0x000020ac) &&
			WR(dev, TVW_CB_PLL_TEST_7_8, 0x00100010) &&
			WR(dev, TVW_CB_PLL_TEST_1_2, 0x86d016a8) &&
			WR(dev, TVW_BURST_RISING1_2, 0x00000020) &&
			WR(dev, TVW_YELLOW_DB1_2, 0x04020406) &&
			WR(dev, TVW_VBI_END_FIELD, 0x80c47828) &&
			WR(dev, TVW_CATV_DB1_2, (stage == 1 ? 0xe85b281e : 0x285b281e)) &&
			WR(dev, TVW_IIR_BW, 0xc1850c01) &&
			WR(dev, TVW_PAUSE_FIX1_2, 0x02d709d3) &&
			WR(dev, TVW_PAUSE_FIX3_4, 0x00007733) &&
			WR(dev, TVW_AGC_CONTROL_2_3, 0x00000006) &&
			WR(dev, TVW_CATV_FIELD, 0x2030800c) &&
			WR(dev, 0x68d0, 0x20000001) &&
			WR(dev, TVW_FIELD_RST1, 0x19e72035) &&
			WR(dev, TVW_FV_WIDE1_2, 0x20002000) &&
			WR(dev, 0x68d4, 0x18202000) &&
			WR(dev, TVW_FV_WIDE3_5, 0xffff7800) &&
			WR(dev, TVW_RESAMP_POWER_THRESHOLD_HIGH, 0x01000120) &&
			WR(dev, TVW_PAUSE_FIX5_6, 0x02240253) &&
			WR(dev, TVW_NM_PROG1_2, 0x0a0a1010) &&
			WR(dev, TVW_NM_PROG3_4, 0x04040707) &&
			WR(dev, TVW_AFE_AGC_CONTROL, 0x0000b000) &&
			WR(dev, TVW_SYNC_DETECT_DB1_2, 0x00000022) &&
			WR(dev, TVW_RESAMP_POWER_THRESHOLD_LOW, 0x0000800b) &&
			WR(dev, TVW_RESAMP_STATUS, 0x80810001) &&
			WR(dev, TVW_PEAK_WHITE_THRESH, 0x00006100) &&
			WR(dev, TVW_PEAK_WHITE_HWIN, 0x03000006) &&
			WR(dev, TVW_PEAK_WHITE_VWIN, 0x08000000) &&
			WR(dev, TVW_PEAK_WHITE_AVERAGE, 0x00000800) &&
			WR(dev, TVW_DEBUG_CONV_5, 0x00000011) &&
			WR(dev, 0x68d8, 0x00005022) &&
			WR(dev, TVW_OFFSET_CALC_STATUS, 0x00000000) &&
			WR(dev, TVW_OFFSET_CALC_LOG, 0x00000000) &&
			WR(dev, TVW_OFFSET_CALC_COUNT, 0x00001000) &&
			WR(dev, TVW_VIDEO_TIMING, 0x00000000) &&
			WR(dev, 0x68c8, 0x00004200) &&
			WR(dev, TVW_D2_COEFF_17_16, 0x00000000) &&
			WR(dev, TVW_D2_COEFF_19_18, 0x00740000) &&
			WR(dev, TVW_DEBUG_MUX_SEL, 0x00000000) &&
			WR(dev, TVW_FINAL_DB1_2, 0x00000074) &&
			WR(dev, 0x68cc, 0x406d3080) &&
			WR(dev, TVW_FINAL_DB3_4, 0xfe01008e) &&
			WR(dev, TVW_FINAL_DB5_6, 0x50f030ff) &&
			WR(dev, TVW_FINAL_DB7_8, 0x00800000) &&
			WR(dev, TVW_FINAL_DB9_10, 0x00200080) &&
			WR(dev, TVW_FINAL_DB11_12, 0x00328400) &&
			WR(dev, TVW_LINE_COUNT1, 0x000b1770) &&
			WR(dev, TVW_FINAL_DB13_14, 0x00000000) &&
			WR(dev, TVW_FINAL_DB15_16, 0x00000000) &&
			WR(dev, TVW_FINAL_DB17_18, 0x00000000) &&
			WR(dev, TVW_RESAMP_YOUT_SHFT, 0x000002cc) &&
			WR(dev, TVW_FINAL_DB19_20, 0x0b0067be) &&
			WR(dev, TVW_FINAL_DB21_22, 0x40003fe0) &&
			WR(dev, TVW_FINAL_DB23_24, 0x00700c32) &&
			WR(dev, TVW_LINE_COUNT2, 0x00c03020) &&
			WR(dev, TVW_LINE_COUNT3, 0x0000003c) &&
			WR(dev, TVW_FINAL_DB25_26, 0x40bf3100) &&
			WR(dev, TVW_FINAL_DB27, 0x18300140) &&
			WR(dev, TVW_ACC_CONTROL, 0x00000660) &&
			WR(dev, TVW_ACC_SPEED, 0x0000e000) &&
			WR(dev, TVW_ACC_TH_ADJ, 0x22220608) &&
			WR(dev, TVW_ACC_BURST_POS1_2, 0x00008e8e) &&
			WR(dev, TVW_ACC_BURST_POS3_4, 0x00000002) &&
			WR(dev, TVW_BURST_WIDTH1_2, 0x00000000) &&
			WR(dev, TVW_BURST_WIDTH3_4, 0x0000b030) &&
			WR(dev, TVW_BURST_WIDTH5_6, 0x00000080) &&
			WR(dev, TVW_BURST_WIDTH7, 0x091d0020) &&
			WR(dev, TVW_SYNC_POS1_2, 0x00000010) &&
			WR(dev, TVW_SYNC_POS3_4, 0x20000106) &&
			WR(dev, TVW_SYNC_POS5_6, 0x00000000) &&
			WR(dev, TVW_SYNC_POS7_8, 0x5000030f) &&
			WR(dev, TVW_FV_WIDE5, 0x04000100) &&
			WR(dev, TVW_SNAP_DB1_2, 0x01800100) &&
			WR(dev, TVW_SNAP_DB3, 0xff9f0100) &&
			WR(dev, TVW_TEAR_DB1, 0x00000000) &&
			WR(dev, TVW_SYNC_POS9_10, 0x00037002) &&
			WR(dev, TVW_CCR_ADJUST, 0x2f0e2202) &&
			WR(dev, TVW_NP_CONTROL, 0x00003def) &&
			WR(dev, TVW_Q_CONTROL, 0x00204f00) &&
			WR(dev, 0x6898, 0x3820000d) &&
			WR(dev, TVW_SYNC_STRIP_V_DETECT_1_2, 0x00600040) &&
			WR(dev, TVW_SYNC_STRIP_V_DETECT_3_4, 0x40800e40) &&
			WR(dev, TVW_SYNC_STRIP_V_DETECT_5_6, 0x01000ecc) &&
			WR(dev, TVW_SYNC_STRIP_V_DETECT_7_8, 0x08000600) &&
			WR(dev, TVW_SYNC_STRIP_V_DETECT_9, 0x10000a00) &&
			WR(dev, TVW_BACK_PORCH, 0x30002000) &&
			WR(dev, TVW_LAST_TBC_DB3_4, 0x00000011) &&
			WR(dev, TVW_NEG_BP_CTR1_2, 0x00000000) &&
			WR(dev, TVW_NEG_BP_CTR3_4, 0xd7970580) &&
			WR(dev, TVW_NEG_BP_CTR4_5, 0x00005717) &&
			WR(dev, TVW_PAL_MOTION_CTRL1_2, 0x01cc3f3c) &&
			WR(dev, TVW_MOTION_3D, 0x000010f0) &&
			WR(dev, TVW_SYNC_LOW_DB1_2, 0x2eec80ff) &&
			WR(dev, TVW_SYNC_LOW_DB3_4, 0x00004942) &&
			WR(dev, TVW_SYNC_LOW_DB5, 0x00000294) &&
			WR(dev, TVW_SYNC_POS11_12, 0x023a8237) &&
			WR(dev, TVW_VCR_DB1_2, 0x0000aa89) &&
			WR(dev, TVW_VLOCK_ACQUIRE_CTRL, 0x00002098) &&
			WR(dev, TVW_VLOCK_ACQUIRE_FIELD_WIN, 0x83001000) &&
			WR(dev, TVW_VLOCK_ACQUIRE_PAL_FIELD, 0x44444444) &&
			WR(dev, TVW_GENLOCK_FLAG, 0xa8644444) &&
			WR(dev, TVW_GENLOCK_VCR, 0x44444444) &&
			WR(dev, TVW_GENLOCK_REC_SELECT, 0x4c0000b8) &&
			WR(dev, 0x68e8, 0xf0006200) &&
			WR(dev, TVW_GENLOCK_TRICK_MODE_1_2, 0x0000070c) &&
			WR(dev, TVW_RESAMP_CONTROL, 0x00ff3040) &&
			WR(dev, TVW_GENLOCK_TRICK_MODE_3_4, 0x80c00c08) &&
			WR(dev, TVW_GENLOCK_TRICK_MODE_5, 0x30402000) &&
			WR(dev, TVW_GENLOCK_VALID_3D, 0x4030a010) &&
			WR(dev, TVW_VLOCK_ACQUIRE_FIELD_LOCK, 0x40302010) &&
			WR(dev, TVW_GENLOCK_BURST_GAIN, 0x00001100) &&
			WR(dev, TVW_AGC_FAST_20P, 0x00000000) &&
			WR(dev, TVW_AGC_FAST_HIGH_TH, 0x05000002) &&
			WR(dev, TVW_OFFSET_CALC_SCH_VALUE, 0x00000e0e) &&
			WR(dev, TVW_AGC_FAST_LOW_TH, 0x0000000b) &&
			WR(dev, TVW_AGC_FAST_PEAK_BLACK_TH, 0x000004fc) &&
			WR(dev, TVW_IF_COEF1, 0x02020e00) &&
			WR(dev, TVW_VDEC_REC_HLOCK_FLAG, 0x39657666) &&
			WR(dev, TVW_IF_COEF2, 0x00000000) &&
			WR(dev, TVW_IF_COEF3, 0x00000000) &&
			WR(dev, TVW_AFE_DEBUG_AGC, 0x00220000) &&
			WR(dev, TVW_VDEC_STATUS, 0xffb9ffd6) &&
			WR(dev, TVW_VDEC_SYNC_LEVEL, 0xff6e00f0) &&
			WR(dev, TVW_VDEC_CLAMP_OUT, 0x037afe6e) &&
			WR(dev, TVW_VDEC_LOW_HLOCK_FLAG, 0xf88bfef2) &&
			WR(dev, TVW_VDEC_LOW_LINE_MEAS, 0x296211df) &&
			WR(dev, 0x693c, 0x000002bc) &&
			WR(dev, TVW_AGC_FAST_SPEED_0_1, 0x0a030008) &&
			WR(dev, TVW_AGC_FAST_SPEED_2_3, 0x00200010) &&
			WR(dev, TVW_VDEC_REC_LINE_MEAS, 0x00000003) &&
			WR(dev, 0x6940, 0x00000020) &&
			WR(dev, 0x6944, 0x0000000f) &&
			WR(dev, 0x6948, 0x27727777) &&
			WR(dev, 0x694c, 0x00004060) &&
			WR(dev, 0x6950, 0x00005090) &&
			WR(dev, 0x6954, 0x000003cc) &&
			WR(dev, 0x6958, 0x00000000) &&
			WR(dev, TVW_SECAM_DIAG, 0x00020000) &&
			WR(dev, 0x6c7c, 0x00000590) &&
			WR(dev, 0x6908, 0x00000000) &&
			WR(dev, 0x691c, 0x00000005) &&
			WR(dev, TVW_VDEC_VIDEO_FLAGS, 0x0410fdcf) &&
			WR(dev, TVW_CB_PLL_ACCUM, 0x14401c0a) &&
			WR(dev, TVW_CB_PLL_FLAG, 0x60404180) &&
			WR(dev, TVW_CP_CTRL, 0x8c28f098) &&
			WR(dev, TVW_MEASURED_NOISE, 0xfc005420) &&
			WR(dev, TVW_NON_STANDARD_FLAG1, 0x00137fff) &&
			WR(dev, TVW_NON_STANDARD_FLAG2, 0xffec0000) &&
			WR(dev, TVW_NON_STANDARD_FLAG3, 0x1010f82f) &&
			WR(dev, TVW_NON_STANDARD_FLAG4, 0x430a4810) &&
			WR(dev, TVW_BP_LEVEL, 0xf08c194f) &&
			WR(dev, TVW_NEW_AGC_GAIN, 0x06060200) &&
			WR(dev, TVW_ACC_GAIN, 0x06080606) &&
			WR(dev, TVW_DEMOD_IIR, 0x04380808) &&
			WR(dev, TVW_ABS_SQ_BURST_IIR, 0x01200120) &&
			WR(dev, TVW_FIELD_V_POS, 0x03100000) &&
			WR(dev, TVW_RESAMP0_DEBUG, 0x01200100) &&
			WR(dev, TVW_CLAMP_DEBUG, 0x000204ff) &&
			WR(dev, TVW_VFLIP_PROG, 0x20f20560) &&
			WR(dev, TVW_PAL_VFLIP_TH1_2, 0x304a0a20) &&
			WR(dev, TVW_PAL_VFLIP_TH3_4, 0x1e40f20a) &&
			WR(dev, TVW_VFLIP_MAX1_2, 0x0a404400) &&
			WR(dev, TVW_VFLIP_MAX3_4, 0x2832423c) &&
			WR(dev, TVW_VFLIP_MAX5_6, 0xff404100) &&
			WR(dev, TVW_VFLIP_CK_SNAP, 0x050e4000) &&
			WR(dev, TVW_STANDARD_CONTROL1_2, 0x7010020e) &&
			WR(dev, TVW_AUTO_SM1, 0x0a085000) &&
			WR(dev, TVW_AUTO_VFLIP1_2, 0x64648440) &&
			WR(dev, TVW_AUTO_VFLIP3_4, 0x8b000240) &&
			WR(dev, TVW_AUTO_VFLIP5_6, 0x04b41eb4) &&
			WR(dev, TVW_SCART_CTRL, 0x58ff2000) &&
			WR(dev, TVW_AUTO_READ1, 0x20640650) &&
			WR(dev, TVW_VFLIP1_2_ON, 0x00392064) &&
			WR(dev, TVW_CBPLL_ABS, 0x019a01ea) &&
			WR(dev, TVW_LPF_LL, 0x004005fa) &&
			WR(dev, TVW_FPLL_DBG_PORT, 0x817a400a) &&
			WR(dev, TVW_NOISE_MEASURE_CONTROL, 0x06406040) &&
			WR(dev, TVW_NOISE_MEASURE_VCOUNT_WINDOW, 0x009b0000) &&
			WR(dev, TVW_SECAM_SEP_DR6, 0x00000f68) &&
			WR(dev, TVW_SECAM_SEP_DR7, 0x28000000) &&
			WR(dev, TVW_SECAM_SEP_DR8, 0x00000000) &&
			WR(dev, TVW_SECAM_SEP_DR9, 0xfffff000) &&
			WR(dev, TVW_SECAM_SEP_DR10, 0x11302020) &&
			WR(dev, TVW_SECAM_SEP_DR11, 0x000a0a0a) &&
			WR(dev, TVW_SECAM_NARROW_TRAP16, 0x00000000) &&
			WR(dev, TVW_SECAM_CONTROL3, 0x00000000) &&
			WR(dev, TVW_SECAM_CONTROL5, 0x00000000) &&
			WR(dev, 0x66d8, 0x0f802f00) &&
			WR(dev, 0x66dc, 0x4a001f80) &&
			WR(dev, TVW_TBC_SM_CTRL, 0x00000a0a) &&
			WR(dev, TVW_TBC_SM_VBI_WIND_START, 0x14001400) &&
			WR(dev, TVW_MEMORY_MARGIN_CTL0, 0x7c0081be) &&
			WR(dev, TVW_MEMORY_MARGIN_CTL1, 0x00000000) &&
			WR(dev, TVW_FSC_PROG1_2, 0x00000000) &&
			WR(dev, 0x650c, 0x0e1c0000) &&
			WR(dev, TVW_FSC_PROG3_4, 0x3b80ffff) &&
			WR(dev, TVW_VSYNC_SLICE1_4, 0x00000000) &&
			WR(dev, TVW_ABS_SQ_BURST_IIR_N_EN, 0x00000000) &&
			WR(dev, TVW_ABS_SQ_BURST_IIR_P_EP, 0x00000000) &&
			WR(dev, TVW_VDEC_FIFO_CTL, 0x000f0000) &&
			WR(dev, TVW_VDEC_INTERRUPT_ENABLE, 0xffff2440) &&
			WR(dev, TVW_VDEC_INTERRUPT_CLEAR, 0x0000ff00) &&
			WR(dev, TVW_VDEC_LOCK_INTR_CTRL, 0xffffffff) &&
			WR(dev, TVW_VDEC_MV_INTR_CTRL, 0x00ff0000) &&
			WR(dev, TVW_VDEC_TRICK_MODE_INTR_CTRL, 0x20802020) &&
			WR(dev, TVW_VDEC_STD_INTR_CTRL, 0x40c64040) &&
			WR(dev, TVW_VDEC_NOISE_MEASURE_HIGH_INTR_CTRL, 0x0000c280) &&
			WR(dev, TVW_VDEC_NOISE_MEASURE_LOW_INTR_CTRL, 0x10101410) &&
			WR(dev, TVW_VDEC_NOISE_INTR_TRIGGER, 0x8f781058) &&
			WR(dev, TVW_VDEC_VSYNC_INTR_CTRL, 0xf532146f) &&
			WR(dev, TVW_VDEC_FIELD_ID_INTR_CTRL, 0x1e1e241e) &&
			WR(dev, TVW_VDEC_DEBUG1_INTR_CTRL, 0x8e8108b7) &&
			WR(dev, TVW_VDEC_MD_CTRL, 0xf8319999) &&
			WR(dev, TVW_MOTION_1D_2D, 0xd562b06c) &&
			WR(dev, TVW_TRICK_AUTO_STANDARD1_2, 0xdc826001) &&
			WR(dev, TVW_VSYNC_SLICE3, 0x28124f3d) &&
			WR(dev, TVW_SAME_CH_3D1_2, 0x0998cf2b) &&
			WR(dev, 0x6568, 0xf1f54b1b) &&
			WR(dev, 0x656c, 0x3c32ef3d) &&
			WR(dev, TVW_SECAM_SEP_DB0, 0xa9983320) &&
			WR(dev, TVW_SECAM_SEP_DB1, 0x34d9433e) &&
			WR(dev, TVW_SECAM_SEP_DB2, 0x1fa39825) &&
			WR(dev, TVW_SECAM_SEP_DB3, 0x4a9e522b) &&
			WR(dev, TVW_SECAM_SEP_DB4, 0x5cfb4e7b) &&
			WR(dev, TVW_SECAM_SEP_DB5, 0x2452d0c3) &&
			WR(dev, TVW_SECAM_SEP_DB6, 0xea9efd2b) &&
			WR(dev, TVW_SECAM_SEP_DB7, 0xd8f051db) &&
			WR(dev, TVW_SECAM_SEP_DB8, 0x8020bf3d) &&
			WR(dev, TVW_SECAM_SEP_DB9, 0x14188025) &&
			WR(dev, TVW_SECAM_SEP_DB10, 0x0d601c1f) &&
			WR(dev, TVW_SECAM_SEP_DB11, 0x20000240) &&
			WR(dev, TVW_SECAM_SEP_DB12, 0x88888888) &&
			WR(dev, TVW_SECAM_SEP_DB13, 0x88887888) &&
			WR(dev, TVW_SECAM_SEP_DB14, 0x88886788) &&
			WR(dev, TVW_SECAM_SEP_DB15, 0x56780004) &&
			WR(dev, TVW_SECAM_SEP_DB16, 0x45680023) &&
			WR(dev, TVW_SECAM_SEP_DR0, 0x23460002) &&
			WR(dev, TVW_SECAM_SEP_DR1, 0x12350001) &&
			WR(dev, TVW_SECAM_SEP_DR2, 0x00000000) &&
			WR(dev, TVW_SECAM_SEP_DR3, 0x44030a05) &&
			WR(dev, TVW_SECAM_SEP_DR12, 0x70002050) &&
			WR(dev, TVW_SECAM_SEP_DR13, 0x08608300) &&
			WR(dev, TVW_SECAM_SEP_DR14, 0x28144000) &&
			WR(dev, TVW_SECAM_SEP_DR15, 0x28142800) &&
			WR(dev, TVW_SECAM_SEP_DR16, 0x14288000) &&
			WR(dev, 0x65f8, 0x70f0001f) &&
			WR(dev, TVW_SECAM_AGC_CTRL1, 0x0a283e40) &&
			WR(dev, TVW_SECAM_AGC_CTRL2, 0xbe100b38) &&
			WR(dev, TVW_SECAM_AGC_CTRL3, 0x04ff1010) &&
			WR(dev, TVW_SECAM_CTRL0, 0x08467f57) &&
			WR(dev, TVW_SECAM_CTRL1, 0x0a603032) &&
			WR(dev, TVW_SECAM_CTRL2, 0x10080b20) &&
			WR(dev, TVW_SECAM_BURST2, 0x000027fd) &&
			WR(dev, TVW_SECAM_CLAMP_CTRL3, 0x00000008) &&
			WR(dev, TVW_SECAM_DEEMPHASIS_COEFF, 0x000007f0) &&
			WR(dev, TVW_SECAM_SEPARATION_CNTRL, 0x0000001d) &&
			WR(dev, TVW_SECAM_EMPHASIS_BOOST, 0x000007cd) &&
			WR(dev, TVW_SECAM_DIAG_DATA, 0x00000064) &&
			WR(dev, TVW_SECAM_CLAMP_CTRL4, 0x020006bd) &&
			WR(dev, TVW_SECAM_AGC_CTRL4, 0x07d80003) &&
			WR(dev, TVW_SECAM_SEPARATION_CTRL1, 0x001107f3) &&
			WR(dev, TVW_SECAM_CONTROL, 0x0009001e) &&
			WR(dev, TVW_SECAM_CLOCHE_COEFB0, 0x07da07dc) &&
			WR(dev, TVW_SECAM_CLOCHE_COEFB1, 0x00530018) &&
			WR(dev, TVW_SECAM_CLOCHE_COEFA1, 0x07920032) &&
			WR(dev, TVW_SECAM_NOISE_SCALING_THRESH_LOW, 0x028606d2) &&
			WR(dev, TVW_SECAM_NOISE_SCALING_SLOPE, 0xc8c802c8) &&
			WR(dev, TVW_SECAM_NOISE_MEASURE_CONTROL, 0x000014a7) &&
			WR(dev, TVW_SECAM_NOISE_MEASURE_VCOUNT_WINDOW, 0x80001460) &&
			WR(dev, TVW_SECAM_NOISE_MEASUREMENT, 0x00000000) &&
			WR(dev, TVW_SECAM_CARRIER_AMP_CTRL, 0x00010000) &&
			WR(dev, TVW_SECAM_NARROW_TRAP1, 0x00000000) &&
			WR(dev, TVW_SECAM_NARROW_TRAP2, 0x00000000) &&
			WR(dev, TVW_SECAM_NARROW_TRAP3, 0x00000000) &&
			WR(dev, TVW_SECAM_NARROW_TRAP4, 0x00080000) &&
			WR(dev, TVW_SECAM_NARROW_TRAP5, 0x112a0028) &&
			WR(dev, TVW_SECAM_NARROW_TRAP6, 0x00440e58) &&
			WR(dev, TVW_SECAM_NARROW_TRAP7, 0x80be2204) &&
			WR(dev, TVW_SECAM_NARROW_TRAP8, 0x04042cb4) &&
			WR(dev, TVW_SECAM_NARROW_TRAP9, 0x00000ac3) &&
			WR(dev, TVW_SECAM_NARROW_TRAP10, 0x61500000) &&
			WR(dev, TVW_SECAM_NARROW_TRAP11, 0x64304180) &&
			WR(dev, TVW_SECAM_NARROW_TRAP12, 0xa028a028) &&
			WR(dev, TVW_SECAM_NARROW_TRAP13, 0x001402ff) &&
			WR(dev, TVW_SECAM_NARROW_TRAP14, 0x00000000) &&
			WR(dev, TVW_SECAM_SEP_DR4, 0x8f7f12f0) &&
			WR(dev, TVW_SECAM_SEP_DR5, 0x00b13b34) &&
			WR(dev, 0x6b74, 0x00000043) &&
			WR(dev, 0x6b78, 0x00036fc8) &&
			WR(dev, 0x6b7c, 0x00041eaf) &&
			WR(dev, 0x6b80, 0x0006df91) &&
			WR(dev, 0x6b88, 0x00083d5f) &&
			WR(dev, 0x6b8c, 0x0000800f) &&
			WR(dev, 0x6b54, 0x00000110) &&
			WR(dev, 0x6b58, 0x00040400) &&
			WR(dev, 0x6940, 0x00000020) &&
			WR(dev, 0x6908, 0x00000000) &&
			WR(dev, 0x6974, 0x00000000) &&
			WR(dev, 0x6978, 0x00000000) &&
			WR(dev, TVW_QM_CONTROL, 0x003f0003) &&
			WR(dev, 0x6abc, 0x0c000202) &&
			WR(dev, TVW_TBC_SM_VBI_V_START, 0xf0080c00) &&
			WR(dev, TVW_TBC_SM_VBI_V_END, 0x070b0008) &&
			WR(dev, TVW_PWM3_CLAMP_GAIN, 0x00000000) &&
			WR(dev, TVW_VPLL_DBG_PORT, 0x00000001) &&
			WR(dev, 0x697c, 0x00000001) &&
			WR(dev, 0x6ac4, 0x00000001) &&
			WR(dev, 0x6ac0, 0x00000000) &&
			WR(dev, TVW_TBC_CBCR_DELAY, 0x00000000) &&
			WR(dev, TVW_TBC_OFFSET_SKEW, 0x00000000) &&
			WR(dev, TVW_TBC_PAUSE, (stage == 1 ? 0xf8000000 : 0x00000000)) &&
			WR(dev, TVW_TBC_PLL_OVER_GAIN, (stage == 1 ? 0x00003d78 : 0x00000000)) &&
			WR(dev, TVW_TBC_LINE_BUF_FREQ_DIF, 0x00000000) &&
			WR(dev, TVW_TBC_LINE_BUF_READ_DISTANCE, (stage == 1 ? 0xffffffff : 0x00000000)) &&
			WR(dev, 0x68a4, 0x00000000) &&
			WR(dev, 0x6ae8, 0x00000000) &&
			WR(dev, 0x6ad0, 0x02000200) &&
			WR(dev, 0x6ad4, 0x00000000) &&
			WR(dev, 0x6ad8, 0x02000200) &&
			WR(dev, 0x6adc, 0x00000000) &&
			WR(dev, 0x6ae0, 0x02000200) &&
			WR(dev, 0x6ae4, 0x00000000) &&
			WR(dev, 0x6aec, 0x00000000) &&
			WR(dev, 0x6af0, 0x07f80000) &&
			WR(dev, 0x6af4, 0x00001000) &&
			WR(dev, 0x6af8, 0x00000000) &&
			WR(dev, 0x6afc, 0x00000000) &&
			WR(dev, 0x6b00, 0x00000000) &&
			WR(dev, 0x6b04, 0x00000000) &&
			WR(dev, 0x6a10, 0x00000000) &&
			WR(dev, 0x6a14, 0x00000000) &&
			WR(dev, 0x6a18, 0x00000000) &&
			WR(dev, 0x6a1c, 0x00000000) &&
			WR(dev, 0x6a20, 0x00000000) &&
			WR(dev, 0x6a58, 0x00000004) &&
			WR(dev, 0x6a24, 0x0000001c) &&
			WR(dev, 0x6a28, 0x00000000) &&
			WR(dev, 0x6a2c, 0x00000000) &&
			WR(dev, 0x6a30, 0x00000000) &&
			WR(dev, 0x6a40, 0x00000000) &&
			WR(dev, 0x6a44, 0x00000000) &&
			WR(dev, 0x6a48, 0x00000003) &&
			WR(dev, 0x6b08, 0x00008000) &&
			WR(dev, 0x6a4c, 0x00000000) &&
			WR(dev, 0x6a50, 0x07380060) &&
			WR(dev, 0x6a54, 0x0000063c) &&
			WR(dev, 0x6a5c, 0x000000c0) &&
			WR(dev, 0x6a60, 0x00000200) &&
			WR(dev, 0x6a64, 0xc1220000) &&
			WR(dev, 0x6a68, 0x93281c00) &&
			WR(dev, 0x6a6c, 0x608f0000) &&
			WR(dev, 0x6a70, 0x00000a00) &&
			WR(dev, 0x6a74, 0xeca86420) &&
			WR(dev, 0x6a78, 0x00001040) &&
			WR(dev, 0x6a7c, 0x01f40023) &&
			WR(dev, 0x6a80, 0x00000032) &&
			WR(dev, 0x6a84, 0x00000000) &&
			WR(dev, 0x6a88, 0x00000000) &&
			WR(dev, 0x68f0, 0x00000000) &&
			WR(dev, 0x6a9c, 0x00200000) &&
			WR(dev, 0x6aa0, 0x38080010) &&
			WR(dev, 0x6aa4, 0x0038022f) &&
			WR(dev, 0x6aa8, 0x00003038) &&
			WR(dev, 0x6ab0, 0x00001020) &&
			WR(dev, 0x6ab4, 0x00800040) &&
			WR(dev, 0x6ab8, 0x010000c0) &&
			WR(dev, TVW_CKILL_5_6, 0x00000080) &&
			WR(dev, 0x68f8, 0x00000006) &&
			WR(dev, 0x68a8, 0x00002000) &&
			WR(dev, 0x696c, 0x00000000) &&
			WR(dev, 0x6938, 0x00000000) &&
			WR(dev, 0x6b50, 0x01000700) &&
			WR(dev, 0x6b20, 0x000000a6) &&
			WR(dev, 0x6b28, 0x000000e4) &&
			WR(dev, 0x6b24, 0x000000e4) &&
			MR(dev, 0x6904, 0x00f0ef70, (stage == 1 ? 0xe400100f : 0xe40f000f)) &&
			MR(dev, TVW_CPU_BURST_PHASE, 0xfffcf800, 0x000306aa) &&
			MR(dev, TVW_CPU_PAL_MOD, ALL, 0x00020000) &&
			WR(dev, TVW_CKILL_5_6, 0x00000080) &&
			MR(dev, TVW_CKILL_DB3_4, 0xfff8ffff, 0x00020000) &&
			WR(dev, TVW_W9_10, 0x01000100) &&
			WR(dev, TVW_W11, 0x01000100) &&
			WR(dev, TVW_YC_CONTROL, 0x00000001) &&
			WR(dev, TVW_YC_DB1, 0x00000079) &&
			WR(dev, TVW_NM_DB1_2, 0x00005ceb) &&
			WR(dev, TVW_CKILL_DB1_2, 0x001c001c) &&
			WR(dev, 0x6c1c, 0x0002a25a) &&
			WR(dev, 0x6cac, 0x00000000) &&
			WR(dev, 0x6cb0, 0x00000000) &&
			WR(dev, 0x6cb4, 0x00000000) &&
			WR(dev, 0x6cb8, 0x00000000) &&
			WR(dev, 0x6cbc, 0x00000000) &&
			WR(dev, 0x6cc0, 0x00000000) &&
			WR(dev, 0x6cc4, 0x00000000) &&
			WR(dev, 0x6cc8, 0x00000000) &&
			WR(dev, 0x6ccc, 0x00000000) &&
			WR(dev, 0x6cd0, 0x00000000) &&
			WR(dev, 0x6cd4, 0x00000000) &&
			WR(dev, 0x6cd8, 0x00001001) &&
			WR(dev, 0x6cdc, 0x00009009) &&
			WR(dev, 0x6ce0, 0x00f3cf3c) &&
			WR(dev, 0x6ce4, 0x0013b13b) &&
			WR(dev, 0x6ce8, 0x00161161) &&
			WR(dev, 0x6cec, 0x0043c43c) &&
			WR(dev, 0x6c7c, 0x00001d90) &&
			WR(dev, 0x6cf0, 0x03008740) &&
			WR(dev, 0x6c5c, 0x0f1a8080) &&
			WR(dev, 0x6c78, 0x20202cc0) &&
			MR(dev, 0x6ca0, ALL, 0x00000003) &&
			WR(dev, 0x6c8c, 0xff0000ff) &&
			WR(dev, 0x6c90, 0xffffffff) &&
			WR(dev, 0x6cf4, 0x676780d0) &&
#endif

			true;
}

bool
tvw_init_tvw_3(tvwsdr_dev_t *dev) {
	return
			MR(dev, 0x6ad4, ALL, 0) && // XXX unknown mask
			MR(dev, 0x6ad0, ALL, 0) && // XXX unknown mask
			MR(dev, 0x6a90, ALL, 0) && // XXX unknown mask
			MR(dev, 0x6ae0, ALL, 0) && // XXX unknown mask
			MR(dev, 0x6ad8, ALL, 0) && // XXX unknown mask
			MR(dev, 0x68f0, ALL, 0) && // XXX unknown mask
			WR(dev, 0x6b3c, 0x00000022) &&
			MR(dev, 0x6b40, ALL, 0) && // XXX unknown mask
			WR(dev, 0x6b44, 0x1b2f806b) &&
			MR(dev, TVW_RBBM_CTRL, ALL, 0) && // XXX unknown mask
			MR(dev, TVW_CLOCK_CONTROL_0, ALL, 0) && // XXX unknown mask
			MR(dev, TVW_CLOCK_CONTROL_0, ALL, 0) && // XXX unknown mask
			MR(dev, TVW_CLOCK_CONTROL_0, ALL, 0) && // XXX unknown mask
			MR(dev, TVW_DVSO_PORT_CONTROL, ALL, 0) && // XXX unknown mask
			MR(dev, TVW_CLOCK_CONTROL_0, 0xfffffff8, 0) && // XXX
			MR(dev, TVW_CLOCK_CONTROL_0, 0xffffdfff, 0) &&
			MR(dev, TVW_CLOCK_CONTROL_0, 0xffff5fff, 0) &&

			true;
}

bool
tvw_finish_init_tvw(tvwsdr_dev_t *dev) {
	return
			MR(dev, TVW_GPIO_OUTPUT_ENABLE, ALL, 0x0000033e) &&
			MR(dev, TVW_GPIO_OUTPUT_VALUE, ALL, 0x0000000c) &&

			//WR(dev, TVW_SPIC_PRESCALE, 0x0000000a) &&
			//WR(dev, TVW_SPIC_PRESCALE, 0x00000001) &&

			true;
}

bool
tvw_init_8051(tvwsdr_dev_t *dev) {
	return
			MR(dev, TVW_GPIO_OUTPUT_VALUE, ALL, 0) && // XXX unknown values

			MR(dev, 0x08f8, 0xfffff0f0, 0) &&
			MR(dev, TVW_VIDEO_INPUT_0_CONTROL, ALL, 0xa01f0000) &&
			MR(dev, TVW_VIDEO_ADC_0_CONTROL, 0x20003080, 0x000f8524) &&
			MR(dev, TVW_VIDEO_INPUT_1_CONTROL, ALL, 0x001f0000) &&
			MR(dev, TVW_VIDEO_ADC_1_CONTROL, 0x20003080, 0x020f853e) &&
			// power up 8051?
			WU(dev, 0xf008, 0xff) &&
			WU(dev, 0xf009, 0x7f) &&
			WU(dev, 0xf00a, 0xdf) &&
			WU(dev, 0xf101, 0x00) &&
			WU(dev, 0xf00d, 0xff) &&
			MU(dev, 0xf00b, UC_ALL, 0xc0) &&
			MU(dev, 0xf301, UC_ALL, 0x1f) &&
			WU(dev, 0xf00e, 0x80) &&
			WU(dev, 0xf01a, 0xff) &&
			WU(dev, 0xf01b, 0xfc) &&
			WU(dev, 0xf01e, 0xff) &&
			MU(dev, 0xf02b, UC_ALL, 0x30) &&
			MU(dev, 0xf00b, 0xd8, 0) &&
			// ?
			WU(dev, 0xf008, 0x50) &&
			WU(dev, 0xf009, 0x00) &&
			WU(dev, 0xf00a, 0x00) &&
			MU(dev, 0xf02b, 0xef, 0) &&
			MU(dev, 0xf00b, 0xbf, 0) &&
			MU(dev, 0xf00d, 0x9f, 0) &&
			// ?
			WU(dev, 0xf00d, 0xff) &&
			MU(dev, 0xf00c, UC_ALL, 0x07) &&
			MU(dev, 0xf00b, UC_ALL, 0xc0) &&
			WU(dev, 0xf00d, 0) &&
			MU(dev, 0xf00c, 0xf8, 0) &&
			MU(dev, 0xf00b, 0x3f, 0) &&
			// ensure 8051 is held in reset
			MU(dev, 0xf089, UC_ALL, 0x80) &&
			// clear out service call registers
			WU(dev, 0xf09f, 0) &&
			WU(dev, 0xf09c, 0) &&
			WU(dev, 0xf09d, 0) &&
			WU(dev, 0xf09e, 0) &&
			// enable firmware upload
			WU(dev, 0xf02a, 0x40) &&
			// ?
			WU(dev, 0xf081, 0) &&
			WU(dev, 0xf082, 0) &&
			WU(dev, 0xf083, 0) &&
			WU(dev, 0xf084, 0) &&
			WU(dev, 0xf085, 0) &&
			WU(dev, 0xf086, 0) &&
			WU(dev, 0xf090, 0) &&
			WU(dev, 0xf091, 0) &&
			WU(dev, 0xf092, 0) &&
			WU(dev, 0xf093, 0) &&
			WU(dev, 0xf094, 0) &&
			WU(dev, 0xf095, 0) &&
			WU(dev, 0xf096, 0);
}

bool
tvw_finish_init_8051(tvwsdr_dev_t *dev) {
	return
			// disable firmware upload
			MU(dev, 0xf02a, ~0x40, 0) &&
			// seed service call registers with firmware info
			// 0x8e1a is T507.bin fw size
			XU(dev, 0xf09f, 0x80, 0) &&
			WU(dev, 0xf0a0, 0x8e) &&
			WU(dev, 0xf0a1, 0x1a) &&
			// ?
			WU(dev, 0xf0a2, 0xef) &&
			WU(dev, 0xf0a3, 0x36) &&
			// service #
			WU(dev, 0xf09f, 0x80 | 0x3c) &&
			// clear 8051 reset
			MU(dev, 0xf089, ((uint8_t)~0x80), 0) &&
			// wait for 8051 to start
			MSLEEP(1000) &&
			// XXX wait for bits
			XU(dev, 0xf09c, 0x80, 0x80) &&
			WU(dev, 0xf09c, 0x00) &&
			// XXX wait for bits

			// ?
			MU(dev, 0xf00b, 0xbf, 0) &&
			MU(dev, 0xf00b, 0x7f, 0) &&
			MU(dev, 0xf024, 0x7f, 0) &&

#if 1 // XXX 0
			// get version
			XU(dev, 0xf024, 0x7f, 0x00) &&
			XU(dev, 0xf025, 0xff, 0x04) &&
			XU(dev, 0xf026, 0xff, 0x17) &&

			// these service calls seem to follow a further pattern
			// the first argument is some identifier
			// the second argument is number of byte pairs
			// the remaining arguments are the pairs of bytes
			tvw_uc_service_call(dev, 0x1c, 2, 0x0c, 0x00) &&
			tvw_uc_service_call(dev, 0x1c, 2, 0x0d, 0x00) &&
			tvw_uc_service_call(dev, 0x1c, 2, 0x0e, 0x00) &&
			tvw_uc_service_call(dev, 0x1c, 2, 0x0f, 0x00) &&
			tvw_uc_service_call(dev, 0x1c, 2, 0x10, 0x00) &&
			tvw_uc_service_call(dev, 0x1c, 2, 0x11, 0x00) &&
			tvw_uc_service_call(dev, 0x1c, 2, 0x12, 0x00) &&
			tvw_uc_service_call(dev, 0x1c, 2, 0x13, 0x00) &&
			tvw_uc_service_call(dev, 0x1c, 2, 0x14, 0x00) &&
			tvw_uc_service_call(dev, 0x1c, 2, 0x15, 0x00) &&
			tvw_uc_service_call(dev, 0x1c, 2, 0x16, 0x00) &&
			tvw_uc_service_call(dev, 0x1c, 2, 0x17, 0x00) &&
			tvw_uc_service_call(dev, 0x1c, 2, 0x18, 0x00) &&
			tvw_uc_service_call(dev, 0x1c, 4, 0x0c, 0x01, 0xc2, 0x60) &&
			tvw_uc_service_call(dev, 0x1c, 4, 0x0d, 0x01, 0xc2, 0x60) &&
			tvw_uc_service_call(dev, 0x1c, 4, 0x0e, 0x01, 0xc2, 0x60) &&
			tvw_uc_service_call(dev, 0x1c, 16, 0x1d, 0x07, 0xc2, 0x60, 0xd7, 0x07, 0xc8, 0x22, 0xd5, 0x33, 0xc6, 0x35, 0xcd, 0x15, 0xc3, 0x04) &&
			tvw_uc_service_call(dev, 0x1c, 10, 0x1e, 0x04, 0xd5, 0x00, 0xc6, 0x00, 0xcd, 0x00, 0xc3, 0x00) &&
			tvw_uc_service_call(dev, 0x1c, 4, 0x0f, 0x01, 0xc2, 0x60) &&
			tvw_uc_service_call(dev, 0x1c, 16, 0x1f, 0x07, 0xc2, 0x60, 0xd7, 0x07, 0xc8, 0x22, 0xd5, 0x33, 0xc6, 0x35, 0xcd, 0x15, 0xc3, 0x04) &&
			tvw_uc_service_call(dev, 0x1c, 10, 0x20, 0x04, 0xd5, 0x00, 0xc6, 0x00, 0xcd, 0x00, 0xc3, 0x00) &&
			tvw_uc_service_call(dev, 0x1c, 4, 0x10, 0x01, 0xc2, 0x60) &&
			tvw_uc_service_call(dev, 0x1c, 16, 0x21, 0x07, 0xc2, 0x60, 0xd7, 0x07, 0xc8, 0x22, 0xd5, 0x33, 0xc6, 0x35, 0xcd, 0x15, 0xc3, 0x04) &&
			tvw_uc_service_call(dev, 0x1c, 10, 0x22, 0x04, 0xd5, 0x00, 0xc6, 0x00, 0xcd, 0x00, 0xc3, 0x00) &&
			// this one is different...
			tvw_uc_service_call(dev, 0x1c, 11, 0x03, 0x01, 0x32, 0x18, 0x6a, 0x18, 0x6a, 0x18, 0x6a, 0x18, 0x6a) &&
#endif

			MU(dev, 0xf117, 0x3f, 0x70) &&

			MU(dev, 0xf00e, 0x7f, 0) && // XXX
			MU(dev, 0xf00f, 0x00, 0x18) && // XXX

			// ???
			MU(dev, 0xf480, 0xbf, 0) &&
			MU(dev, 0xf00d, UC_ALL, 0x10) &&
			MU(dev, 0xf00d, 0xef, 0) &&
			MU(dev, 0xf480, UC_ALL, 0x0c) && // XXX
			WU(dev, 0xf481, 0x00) && // XXX
			WU(dev, 0xf48d, 0x00) && // XXX
			WU(dev, 0xf482, 0x80) && // XXX
			WU(dev, 0xf483, 0x00) && // XXX
			WU(dev, 0xf484, 0x00) && // XXX
			WU(dev, 0xf488, 0x06) && // XXX
			WU(dev, 0xf48c, 0x00) && // XXX
			WU(dev, 0xf48b, 0x00) && // XXX
			MU(dev, 0xf480, UC_ALL, 0x40) &&

			// ???
			MR(dev, 0x08f8, 0xfffff0f0, 0) &&
			MR(dev, TVW_VIDEO_INPUT_0_CONTROL, ALL, 0xa01f0000) &&
			MR(dev, TVW_VIDEO_ADC_0_CONTROL, 0x20003080, 0x000f8524) &&
			MR(dev, TVW_VIDEO_INPUT_1_CONTROL, ALL, 0x001f0000) &&
			MR(dev, TVW_VIDEO_ADC_1_CONTROL, 0x20003080, 0x020f853e) &&

			MU(dev, 0xf00d, UC_ALL, 0x01) &&
			MU(dev, 0xf00d, 0xfe, 0) &&

			WR(dev, TVW_TSI_PORT_CONTROL, 0x07ff0000) &&
			WR(dev, TVW_PATCH_REG, 0x00000001) &&
			WU(dev, 0xf0b2, 0) &&
			WU(dev, 0xf0b3, 0) &&
			WU(dev, 0xf0b4, 0) &&
			WU(dev, 0xf0b5, 0) &&

			// ???
			MU(dev, 0xf00b, UC_ALL, 0x40) &&
			MU(dev, 0xf00b, 0xbf, 0) &&
			MU(dev, 0xf02b, 0, 0) && // XXX
			MU(dev, 0xf00d, UC_ALL, 0x01) &&
			MU(dev, 0xf00d, 0xfe, 0) &&
			MU(dev, 0xf00d, 0xfc, 0x02) &&
			MU(dev, 0xf00d, 0xfc, 0) &&
			tvw_uc_service_call(dev, 0x1a, 5, 0xe2, 0xed, 0x8b, 0x9b, 0x00) &&
			// ???
			WU(dev, 0xf101, 0x00) &&
			WU(dev, 0xf100, 0x00) &&
			WU(dev, 0xf117, 0x07) &&
			WU(dev, 0xf11c, 0x00) &&
			WU(dev, 0xf11d, 0x00) &&
			WU(dev, 0xf106, 0x00) &&
			WU(dev, 0xf10d, 0x00) &&
			WU(dev, 0xf103, 0x00) &&
			WU(dev, 0xf102, 0x60) &&
			WU(dev, 0xf101, 0x04) &&
			tvw_uc_service_call(dev, 0x1c, 3, 0x01, 0x02, 0x00) &&
			// ???
			MU(dev, 0xf480, 0xbf, 0) &&
			MU(dev, 0xf00d, UC_ALL, 0x10) &&
			MU(dev, 0xf00d, 0xef, 0) &&
			MU(dev, 0xf480, UC_ALL, 0x04) && // XXX
			WU(dev, 0xf481, 0x00) && // XXX
			WU(dev, 0xf48d, 0x00) && // XXX
			WU(dev, 0xf482, 0x80) && // XXX
			WU(dev, 0xf483, 0x00) && // XXX
			WU(dev, 0xf484, 0x00) && // XXX
			WU(dev, 0xf488, 0x00) && // XXX
			WU(dev, 0xf48c, 0x00) && // XXX
			WU(dev, 0xf48b, 0x00) && // XXX
			MU(dev, 0xf480, UC_ALL, 0x40) &&
			// ???
			WU(dev, 0xf090, 0x80) &&
			MSLEEP(10) &&
			// XXX
			WU(dev, 0xf063, 0x01) &&
			WU(dev, 0xf101, 0x04) &&

			MR(dev, TVW_CLOCK_CONTROL_0, ALL, 0) && // XXX unknown values

			true;
}

bool
tvw_init56_tvw(tvwsdr_dev_t *dev) {
	return
			MR(dev, TVW_DVSO_PORT_CONTROL, ALL, 0) && // XXX unknown values
			MR(dev, TVW_CLOCK_CONTROL_0, ALL, 0) && // XXX unknown values
			MR(dev, TVW_DVSO_PORT_CONTROL, ALL, 0) && // XXX unknown values

			// XXX
			MR(dev, 0x0938, ALL, 0x00000100) &&
			// XXX
			WR(dev, TVW_AVSYNC_STC_WINDOW, 0x000cdfe5) &&
			WR(dev, TVW_AVSYNC_SCLK_WINDOW, 0x000176ff) &&
			WR(dev, TVW_AVSYNC_AUDIO_GAIN, 0x28800880) &&
			MR(dev, TVW_GPIO_OUTPUT_VALUE, ALL, 0) && // XXX unknown values
			MR(dev, TVW_GPIO_OUTPUT_VALUE, ALL, 0) && // XXX unknown values
			MR(dev, TVW_DVSO_PORT_CONTROL, ALL, 0x00000002) &&
			WR(dev, TVW_AVSYNC_STC_WINDOW, 0x000cdfe5) &&
			WR(dev, TVW_AVSYNC_SCLK_WINDOW, 0x000176ff) &&
			WR(dev, TVW_AVSYNC_AUDIO_GAIN, 0x28800880) &&

			// XXX something with E0_MAGIC
			MR(dev, TVW_CLOCK_CONTROL_0, ALL, 0x00005000) &&
			// enable something
			tvw_e0_magic(dev, 0x00000000, 0x00000151) &&
			WR(dev, TVW_CLOCK_CONTROL_1, 0x20070154) &&
			WR(dev, 0x0938, 0x0001a119) &&
			WU(dev, 0xf00d, 0xe0) &&
			WU(dev, 0xf00c, 0x07) &&
			WR(dev, TVW_CLOCK_CONTROL_0, 0x001f8a30) &&
			MR(dev, 0x08ac, 0xfffffff7, 0) &&
			MR(dev, TVW_VOLTAGE_CONTROL0, ALL, 0x00ff0001) &&
			MR(dev, TVW_USB_PATCH_REG, 0xfffffffb, 0) &&

			/* INIT 6 */
			MR(dev, TVW_GPIO_PORT_CONTROL, ALL, 0x00000002) &&

			MR(dev, TVW_CLOCK_CONTROL_0, ALL, 0) && // XXX unknown values

			WR(dev, 0x0350, 0x01e001e0) &&
			WR(dev, 0x036c, 0x000f0064) &&
			WR(dev, 0x0364, 0x0001ffff) &&
			MR(dev, TVW_USC_CTL, ALL, 0x00000400) &&
			MR(dev, TVW_USC_CTL, 0xfffffbff, 0x00000200) &&
			WR(dev, TVW_USC_ANC_WIDTH, 0x00100000) &&
			WR(dev, TVW_USC_VB_START, 0x00010001) &&
			WR(dev, TVW_USC_VB_FINISH, 0x00130013) &&
			WR(dev, TVW_USC_VD_START, 0x00140014) &&
			WR(dev, TVW_USC_VD_FINISH, 0x01340134) &&
			MR(dev, TVW_DVSO_PORT_CONTROL, 0xfffffffd, 0) &&

			MR(dev, TVW_CLOCK_CONTROL_0, ALL, 0) && // XXX unknown values

			MR(dev, TVW_RESET_CONTROL, ALL, 0x00000180) &&
			MR(dev, TVW_RESET_CONTROL, 0xfffffe7f, 0) &&

#if 1
			WR(dev, TVW_VIDEO_INPUT_0_CONTROL, 0) &&
			WR(dev, TVW_VIDEO_INPUT_1_CONTROL, 0) &&
			WR(dev, TVW_VIDEO_ADC_0_CONTROL, 0) &&
			WR(dev, TVW_VIDEO_ADC_1_CONTROL, 0) &&
#endif

			MR(dev, TVW_CLOCK_CONTROL_0, ALL, 0x00005000) &&
			// enable something
			tvw_e0_magic(dev, 0x00000000, 0x00000151) &&
			WR(dev, TVW_CLOCK_CONTROL_1, 0x20070154) &&
			WR(dev, 0x0938, 0x0001a119) &&
			WU(dev, 0xf00d, 0xe0) &&
			WU(dev, 0xf00c, 0x07) &&
			WR(dev, TVW_CLOCK_CONTROL_0, 0x001f8a30) &&
			MR(dev, 0x08ac, 0xfffffff7, 0) &&
			MR(dev, TVW_VOLTAGE_CONTROL0, ALL, 0x00ff0001) &&
			MR(dev, TVW_USB_PATCH_REG, 0xfffffffb, 0) &&

			MR(dev, TVW_RESET_CONTROL, ALL, 0x00000180) &&
			MR(dev, TVW_RESET_CONTROL, 0xfffffe7f, 0) &&
			//MSLEEP(10) &&

			MR(dev, TVW_DVSO_PORT_CONTROL, ALL, 0) && // XXX unknown values
			MR(dev, TVW_CLOCK_CONTROL_0, ALL, 0) && // XXX unknown values
			MR(dev, TVW_GPIO_OUTPUT_VALUE, ALL, 0) && // XXX unknown values
			MR(dev, TVW_DVSO_PORT_CONTROL, ALL, 0) && // XXX unknown values
			MR(dev, TVW_DVSO_PORT_CONTROL, ALL, 0) && // XXX unknown values
			MR(dev, TVW_CLOCK_CONTROL_1, ALL, 0x00000001) && // XXX unknown values
			MR(dev, TVW_CLOCK_CONTROL_0, ALL, 0x00000005) && // XXX unknown values

			MR(dev, TVW_RESET_CONTROL, ALL, 0x00000180) &&
			//WR(dev, 0x0938, 0x0001a01b) &&
			MR(dev, 0x0938, ALL, 0x00000002) && // XXX unknown values
			WR(dev, 0x0940, 0x004ad600) &&
			WR(dev, 0x0944, 0x33950000) &&
			WR(dev, 0x0948, 0x0002dc37) &&
			MR(dev, 0x0938, 0xffffffef, 0) &&
			WR(dev, 0x0938, 0x0001a108) &&
			MR(dev, TVW_RESET_CONTROL, 0xffffff7f, 0) &&

			MR(dev, TVW_ADEC_MATRIX_CONTROL, ALL, 0) && // XXX unknown mask
			MR(dev, TVW_ADEC_RAMPING_PERIOD, ALL, 0) && // XXX unknown mask
			WR(dev, TVW_ADEC_RAMPING_TARGET_VOLUME, 0x000000ff) &&

			true;
}

bool
tvw_init_adec(tvwsdr_dev_t *dev) {
	return
			// file "11"

			MR(dev, TVW_ADEC_CONTROL, ALL, 0) && // XXX unknown mask
			WR(dev, TVW_ADEC_MIXER1_DTO, 0) && // XXX different in "22"
			MR(dev, TVW_RESET_CONTROL, ALL, 0x00000180) &&
			MR(dev, TVW_RESET_CONTROL, 0xfffffe7f, 0) &&
			WR(dev, TVW_ADEC_MIXER1_DTO, 0x00155a61) &&

			// XXX ADEC FIR XXX

			WR(dev, TVW_ADEC_DEMOD1_CONTROL, 0x00000900) &&
			WR(dev, TVW_ADEC_MIXER3_DTO1, 0x001fb3c2) &&
			WR(dev, TVW_ADEC_MIXER3_DTO2, 0x001f6784) &&
			WR(dev, TVW_ADEC_MIXER3_DTO3, 0x001e82d7) &&
			WR(dev, TVW_ADEC_PILOT_CHANNEL, 0x00000a2c) &&
			WR(dev, TVW_ADEC_SAP_CHANNEL, 0x00000a09) &&
			WR(dev, TVW_ADEC_POWER_LPF1, 0x000000aa) &&
			WR(dev, TVW_ADEC_POWER_LPF2, 0x000000ff) &&
			WR(dev, TVW_ADEC_SC1_POWER_MEASURE, 0x00002a00) &&
			WR(dev, TVW_ADEC_SC1_POWER_THRD_HIGH, 0x00000000) &&
			WR(dev, TVW_ADEC_SC1_POWER_THRD_LOW, 0x00000000) &&
			WR(dev, TVW_ADEC_SC2_POWER_MEASURE, 0x000027e0) &&
			WR(dev, TVW_ADEC_SC2_POWER_THRD_HIGH, 0x00000000) &&
			WR(dev, TVW_ADEC_SC2_POWER_THRD_LOW, 0x00000000) &&
			WR(dev, TVW_ADEC_PILOT_POWER_MEASURE, 0x01ae1e60) &&
			WR(dev, TVW_ADEC_PILOT_POWER_THRD_HIGH, 0x00001800) &&
			WR(dev, TVW_ADEC_PILOT_POWER_THRD_LOW, 0x00001200) &&
			WR(dev, TVW_ADEC_SAP_POWER_MEASURE, 0x0006007d) &&
			WR(dev, TVW_ADEC_SAP_POWER_THRD_HIGH, 0x00000300) &&
			WR(dev, TVW_ADEC_SAP_POWER_THRD_LOW, 0x00000250) &&
			WR(dev, TVW_ADEC_DECIMATOR_BYPASS_CONTROL, 0x00080140) &&
			WR(dev, TVW_ADEC_FIR2_CONTROL, 0x00037f05) &&
			WR(dev, TVW_ADEC_DEMOD2_CONTROL, 0x000000e8) &&
			WR(dev, TVW_ADEC_UPSAMPLER_GAIN, 0x00000e00) &&
			WR(dev, TVW_ADEC_EXPANDER_WIDEBAND_GAIN, 0x00003ff0) &&
			WR(dev, TVW_ADEC_EXPANDER_SPECTRAL_GAIN, 0x00002c0a) &&
			WR(dev, TVW_ADEC_MATRIX_CONTROL, 0x00000019) &&
			WR(dev, TVW_ADEC_AVC_LPF, 0x00000020) &&
			WR(dev, TVW_ADEC_AVC_THRESHOLD, 0x00000080) &&
			WR(dev, TVW_ADEC_NOTCH_ZERO, 0x00800010) &&
			WR(dev, TVW_ADEC_NOTCH_POLE, 0x007fc01a) &&
			WR(dev, TVW_ADEC_RAMPING_TARGET_VOLUME, 0x00000080) &&
			WR(dev, TVW_ADEC_RAMPING_PERIOD, 0x0000000a) &&
			WR(dev, TVW_ADEC_UPDATENCO_NCO, 0x008ce2aa) &&
			WR(dev, TVW_ADEC_UPDATENCO_ACQUIRE_LPF, 0x00000010) &&
			WR(dev, TVW_ADEC_UPDATENCO_TRACKING_LPF, 0x00000010) &&
			WR(dev, TVW_ADEC_UPDATENCO_FIFO_K1, 0x00000576) &&
			WR(dev, TVW_ADEC_UPDATENCO_FIFO_K2, 0x0000001d) &&
			WR(dev, TVW_ADEC_UPDATENCO_THROTTLE_FACTOR, 0x00000008) &&
			WR(dev, TVW_ADEC_UPDATENCO_THROTTLE_THRD, 0x00000008) &&
			WR(dev, TVW_ADEC_FIFO_STATUS, 0x00000007) &&
			WR(dev, TVW_ADEC_FIFO_OVERFLOW_CNTR, 0x000000ff) &&
			WR(dev, TVW_ADEC_FIFO_UNDERFLOW_CNTR, 0x000000ff) &&
			WR(dev, TVW_ADEC_I2S_WS_OFFSET, 0x00000000) &&
			WR(dev, TVW_ADEC_I2S_CONTROL, 0x00002018) &&
			WR(dev, TVW_ADEC_DECODER_DIAG, 0x0000007e) &&
			WR(dev, TVW_ADEC_DECODER_DIAG_DATA, 0x00125c00) &&
			WR(dev, TVW_ADEC_DECODER_CONTROL, 0x00000320) &&
			WR(dev, TVW_ADEC_TEST_MUX_SELECT, 0x00000012) &&
			WR(dev, TVW_ADEC_NICAM_ERROR_THRESHOLD, 0x00000010) &&
			WR(dev, TVW_ADEC_NICAM_ERROR_CNTR, 0x000001fe) &&
			WR(dev, TVW_ADEC_NICAM_NCO, 0x00aaaaab) &&
			WR(dev, TVW_ADEC_NICAM_COEFF_DQPSK_RESAMPL, 0x78200000) &&
			WR(dev, TVW_ADEC_NICAM_COEFF_DQPSK_LPF, 0x00000400) &&
			WR(dev, TVW_ADEC_DC_OFFSET, 0x0002dbfc) &&
			WR(dev, TVW_ADEC_TESTMUX_DATA, 0x00000033) &&
			WR(dev, 0x50f8, 0x00000087) &&
			WR(dev, 0x50fc, 0x00007003) &&
			WR(dev, 0x5100, 0x0001700d) &&
			WR(dev, 0x5104, 0x00035024) &&
			WR(dev, 0x5108, 0x0006304a) &&
			WR(dev, 0x510c, 0x0009c07e) &&
			WR(dev, 0x5110, 0x000d60ba) &&
			WR(dev, 0x5114, 0x001050f0) &&
			WR(dev, 0x5118, 0x0011b113) &&
			WR(dev, 0x511c, 0x05930a65) &&
			WR(dev, 0x5120, 0x0004fffc) &&
			WR(dev, 0x5124, 0x0004fffc) &&
			WR(dev, 0x5128, 0xfff02200) &&
			WR(dev, 0x512c, 0x00000006) &&
			WR(dev, 0x5130, 0x0000000a) &&
			WR(dev, 0x5134, 0x00000005) &&
			WR(dev, 0x5138, 0x0000000f) &&
			WR(dev, 0x513c, 0x00000006) &&
			WR(dev, 0x5140, 0x0000aaaa) &&
			WR(dev, 0x5144, 0x0000cccc) &&
			WR(dev, 0x5148, 0x0000f0f0) &&
			WR(dev, 0x514c, 0x0000ff00) &&
			WR(dev, 0x5154, 0x001fb3c5) &&
			WR(dev, 0x5158, 0x400a4e50) &&
			WR(dev, 0x515c, 0x0020f808) &&
			WR(dev, 0x5160, 0x002000a6) &&
			WR(dev, 0x5164, 0x00000000) &&
			WR(dev, 0x5168, 0xff9dcc48) &&
			WR(dev, 0x516c, 0x004e6632) &&
			WR(dev, 0x5170, 0xffe0ffe0) &&
			WR(dev, 0x5174, 0x0000120f) &&
			WR(dev, 0x5178, 0x01ffff8e) &&
			WR(dev, 0x517c, 0x049f0d49) &&
			WR(dev, 0x5180, 0x0008049f) &&
			WR(dev, 0x5184, 0x112c6616) &&
			WR(dev, 0x5188, 0x00a40923) &&
			WR(dev, 0x518c, 0x00000000) &&
			WR(dev, 0x5190, 0x00000000) &&
			WR(dev, 0x5194, 0x00000000) &&
			WR(dev, 0x5198, 0x00000000) &&
			WR(dev, 0x519c, 0x00000000) &&
			WR(dev, 0x51a0, 0x01028c00) &&
			WR(dev, 0x51a4, 0x0202bb00) &&
			WR(dev, 0x51a8, 0x00350502) &&
			WR(dev, 0x51ac, 0x00322802) &&
			WR(dev, 0x51b0, 0x00000000) &&
			WR(dev, 0x51b4, 0x00000000) &&
			WR(dev, TVW_ADEC_CONTROL, 0x000004c7) &&

			// XXX ADEC FIR XXX

			WR(dev, TVW_ADEC_MIXER1_DTO, 0x00155a61) &&
			WR(dev, TVW_ADEC_MIXER3_DTO1, 0x001fa3ef) &&
			WR(dev, 0x5154, 0x001fa3ef) &&
			WR(dev, TVW_ADEC_MIXER3_DTO2, 0x001f47df) &&
			WR(dev, TVW_ADEC_MIXER3_DTO3, 0x001fa3ef) &&
			MR(dev, TVW_ADEC_MATRIX_CONTROL, ALL, 0x00000080) &&
			WR(dev, TVW_ADEC_PILOT_POWER_THRD_HIGH, 0x00000060) &&
			WR(dev, TVW_ADEC_PILOT_POWER_THRD_LOW, 0x00000020) &&
			MR(dev, TVW_ADEC_TESTMUX_DATA, 0, 0x00eab4c2) && // XXX unknown values, from 0x00000033
			MR(dev, TVW_RESET_CONTROL, ALL, 0x00000180) &&
			MR(dev, TVW_RESET_CONTROL, 0xfffffe7f, 0x00000000) &&
			WR(dev, TVW_ADEC_UPDATENCO_NCO, 0x008ce2aa) && // no change
			WR(dev, TVW_ADEC_NICAM_NCO, 0x00aaaaab) && // no change

			MR(dev, TVW_ADEC_MATRIX_CONTROL, 0xfffffff7, 0) &&
			WR(dev, TVW_ADEC_AVC_THRESHOLD, 0x00000080) &&
			MR(dev, TVW_ADEC_MATRIX_CONTROL, 0xfffffffe, 0) &&
			WR(dev, TVW_ADEC_RAMPING_PERIOD, 0x0000000a) &&
			WR(dev, TVW_ADEC_MATRIX_CONTROL, 0x00000090) &&
			MR(dev, TVW_ADEC_RAMPING_PERIOD, 0xffffff5f, 0) &&
			WR(dev, TVW_ADEC_RAMPING_TARGET_VOLUME, 0x00000000) &&
			MR(dev, TVW_ADEC_MATRIX_CONTROL, ALL, 0x00000002) &&
			WR(dev, TVW_ADEC_CONTROL, 0x000004c7) &&
			WR(dev, TVW_ADEC_MIXER1_DTO, 0x00155a61) &&
			MR(dev, TVW_RESET_CONTROL, ALL, 0x00000180) &&
			MR(dev, TVW_RESET_CONTROL, 0xfffffe7f, 0) &&
			WR(dev, TVW_ADEC_I2S_CONTROL, 0x00002018) &&

			// end of file "11"

			true;
}

bool
tvw_init_unknown_1(tvwsdr_dev_t *dev) {
	return
			// enable ADEC?
			tvw_e0_magic(dev, 0x00000000, 0x00140130) &&
			// initialize ADEC threshold?
			tvw_e0_magic(dev, 0x00000000, 0x00000131) &&

			MR(dev, TVW_ADEC_MATRIX_CONTROL, ALL, 0) &&
			MR(dev, TVW_ADEC_RAMPING_PERIOD, 0xfffffff5, 0) &&
			WR(dev, TVW_ADEC_RAMPING_TARGET_VOLUME, 0) &&

			WR(dev, TVW_VIDEO_ADC_0_CONTROL, 0x000f8520) &&
			WR(dev, TVW_VIDEO_ADC_1_CONTROL, 0x020f853e) &&
			WR(dev, TVW_TEST_CONTROL, 0) &&
			MR(dev, TVW_CLOCK_CONTROL_1, ALL, 0x10000000) &&
			WR(dev, TVW_VIDEO_INPUT_1_CONTROL, 0x001f0000) &&
			MR(dev, TVW_CLOCK_CONTROL_0, ALL, 0x00000002) &&

			MR(dev, TVW_RESET_CONTROL, ALL, 0x00000180) &&
			// XXX unknown values
			WR(dev, 0x0938, 0x0001a11b) &&
			WR(dev, 0x0940, 0x004ad600) &&
			WR(dev, 0x0944, 0x33950000) &&
			WR(dev, 0x0948, 0x0002dc37) &&
			WR(dev, 0x0938, 0x0001a10b) &&
			WR(dev, 0x0938, 0x0001a108) &&
			MR(dev, TVW_CLOCK_CONTROL_0, ALL, 0x00005000) &&
			// enable something
			tvw_e0_magic(dev, 0x00000000, 0x00000151) &&

			MR(dev, TVW_USC_CTL, ALL, 0x00000400) &&

			MR(dev, TVW_RESET_CONTROL, ALL, 0x00000200) &&

			true;
}

bool
tvw_init_unknown_2(tvwsdr_dev_t *dev) {
	return
			MR(dev, TVW_USC_CTL, 0xfffffbff, 0) &&

			// XXX init56 prefix?
			MR(dev, 0x0938, ALL, 0x00000100) &&
			// XXX
			WR(dev, TVW_AVSYNC_STC_WINDOW, 0x000cdfe5) &&
			WR(dev, TVW_AVSYNC_SCLK_WINDOW, 0x000176ff) &&
			WR(dev, TVW_AVSYNC_AUDIO_GAIN, 0x28800880) &&

			// XXX unknown values
			MR(dev, 0x6ad4, ALL, 0) &&
			MR(dev, 0x6ad0, ALL, 0) &&
			MR(dev, 0x6a90, ALL, 0) &&
			MR(dev, 0x6ae0, ALL, 0) &&
			MR(dev, 0x6ad8, ALL, 0) &&
			MR(dev, 0x68f0, ALL, 0) &&
			MR(dev, 0x6940, ALL, 0) &&
			MR(dev, 0x6860, ALL, 0x00000010) &&
			MR(dev, 0x6940, ALL, 0) &&
			WR(dev, 0x6868, 0x00000220) &&
			WR(dev, 0x686c, 0x00000000) &&
			WR(dev, TVW_CONTROL_TWO, 0x00000020) &&
			WR(dev, TVW_PAL_MOTION_CTRL1_2, 0x01cc3f3c) &&
			WR(dev, TVW_HLOCK_ACQUIRE_LOW_LL_WIN, 0x00001c1a) &&
			WR(dev, TVW_HLOCK_ACQUIRE_LOW_LOST, 0x00001e18) &&
			WR(dev, TVW_HLOCK_ACQUIRE_FOUND, 0x00008070) &&
			WR(dev, TVW_HLOCK_ACQUIRE_REC_LL, 0x00000002) &&
			WR(dev, TVW_CONTROL_1D1_2, 0x00001000) &&
			WR(dev, TVW_CONTROL_1D3_4, 0x00001000) &&
			WR(dev, TVW_LINE_COUNT3, 0x0000013c) &&
			WR(dev, TVW_IF_COEF1, 0x02020e01) &&
			WR(dev, TVW_HLOCK_ACQUIRE_REC_LOST, 0x00008302) &&
			WR(dev, TVW_LINE_COUNT2, 0xe0c07010) &&
			WR(dev, TVW_LINE_COUNT1, 0x000b1770) &&
			WR(dev, 0x6870, 0x000007fd) &&
			WR(dev, 0x6a58, 0x00000504) &&
			WR(dev, 0x6a5c, 0x000001c0) &&
			WR(dev, 0x6898, 0x3f3f000d) &&
			WR(dev, TVW_SYNC_STRIP_V_DETECT_3_4, 0x40805020) &&
			WR(dev, TVW_SYNC_STRIP_V_DETECT_7_8, 0x08000200) &&
			WR(dev, TVW_SYNC_STRIP_V_DETECT_9, 0x28000800) &&
			WR(dev, TVW_BACK_PORCH, 0x30002800) &&
			WR(dev, TVW_SYNC_DETECT_DB1_2, 0x00000033) &&
			WR(dev, TVW_DEBUG_CONV_1_2, 0x00350000) &&
			WR(dev, TVW_LAST_TBC_DB7, 0x00c60f42) &&
			WR(dev, TVW_AGC_CONTROL_4_5, 0x81008000) &&
			WR(dev, TVW_NEG_BP_CTR3_4, 0x57170580) &&
			WR(dev, TVW_D2_COEFF_17_16, 0x00000000) &&
			WR(dev, TVW_CATV_DB1_2, 0x285b281e) &&
			WR(dev, TVW_RESAMP_STATUS, 0x80810001) &&
			WR(dev, TVW_PEAK_WHITE_THRESH, 0x00006500) &&
			WR(dev, TVW_PEAK_WHITE_HWIN, 0x03000010) &&
			WR(dev, TVW_PEAK_WHITE_VWIN, 0x08000000) &&
			WR(dev, TVW_PEAK_WHITE_AVERAGE, 0x00000800) &&
			WR(dev, 0x688c, 0x003e0000) &&
			WR(dev, 0x68ec, 0x00000500) &&
			WR(dev, TVW_CATV_FIELD, 0x2030800c) &&
			WR(dev, TVW_RESAMP_POWER_THRESHOLD_HIGH, 0x01000100) &&
			WR(dev, TVW_VDEC_REC_LINE_MEAS, 0x00000003) &&
			WR(dev, 0x6aa4, 0x0038222f) &&
			WR(dev, TVW_SECAM_DIAG, 0x00020000) &&
			WR(dev, 0x6ab4, 0x00600010) &&
			WR(dev, 0x6ab8, 0x014000f0) &&
			WR(dev, 0x6b34, 0x00001000) &&
			WR(dev, 0x6b38, 0x00000010) &&
			WR(dev, 0x6c7c, 0x00000590) &&
			WR(dev, 0x6908, 0x00000000) &&
			WR(dev, 0x691c, 0x00000005) &&
			WR(dev, TVW_TBC_CBCR_DELAY, 0x00000000) &&
			WR(dev, TVW_TBC_OFFSET_SKEW, 0x00000000) &&
			WR(dev, TVW_TBC_PAUSE, 0xf8000000) &&
			WR(dev, TVW_TBC_PLL_OVER_GAIN, 0x00003d78) &&
			WR(dev, TVW_TBC_LINE_BUF_FREQ_DIF, 0x00000000) &&
			WR(dev, TVW_TBC_LINE_BUF_READ_DISTANCE, 0xffffffff) &&
			WR(dev, TVW_MEMORY_MARGIN_CTL0, 0x7c0081be) &&
			WR(dev, TVW_MEMORY_MARGIN_CTL1, 0x00000000) &&
			WR(dev, TVW_FSC_PROG1_2, 0x00000000) &&
			WR(dev, 0x650c, 0x0e1c0000) &&
			WR(dev, TVW_FSC_PROG3_4, 0x3b80ffff) &&
			WR(dev, TVW_VSYNC_SLICE1_4, 0x00000000) &&
			WR(dev, TVW_ABS_SQ_BURST_IIR_N_EN, 0x00000000) &&
			WR(dev, TVW_ABS_SQ_BURST_IIR_P_EP, 0x00000000) &&
			WR(dev, TVW_VDEC_FIFO_CTL, 0x00070000) &&
			WR(dev, TVW_CB_PLL_ACCUM, 0x14401c0a) &&
			WR(dev, TVW_CB_PLL_FLAG, 0x60304182) &&
			WR(dev, TVW_CP_CTRL, 0x3c284094) &&
			WR(dev, TVW_MEASURED_NOISE, 0xfc00542e) &&
			WR(dev, TVW_TBC_SM_CTRL, 0x00000a0a) &&
			WR(dev, TVW_TBC_SM_VBI_WIND_START, 0x00001400) &&
			WR(dev, TVW_NON_STANDARD_FLAG1, 0x00137fff) &&
			WR(dev, TVW_NON_STANDARD_FLAG2, 0xffec0000) &&
			WR(dev, TVW_NON_STANDARD_FLAG3, 0x0004f80f) &&
			WR(dev, TVW_NON_STANDARD_FLAG4, 0x430a380a) &&
			WR(dev, TVW_BP_LEVEL, 0xf08c194f) &&
			WR(dev, TVW_NEW_AGC_GAIN, 0x04040000) &&
			WR(dev, TVW_ACC_GAIN, 0x04040404) &&
			WR(dev, TVW_DEMOD_IIR, 0x04350404) &&
			WR(dev, TVW_ABS_SQ_BURST_IIR, 0x00500050) &&
			WR(dev, TVW_FIELD_V_POS, 0x01000000) &&
			WR(dev, TVW_RESAMP0_DEBUG, 0x02000100) &&
			WR(dev, TVW_CLAMP_DEBUG, 0x000202ff) &&
			WR(dev, TVW_VFLIP_PROG, 0x1000142f) &&
			WR(dev, TVW_PAL_VFLIP_TH1_2, 0x204a0832) &&
			WR(dev, TVW_PAL_VFLIP_TH3_4, 0x1e30c2ff) &&
			WR(dev, TVW_VFLIP_MAX1_2, 0x0a400000) &&
			WR(dev, TVW_VFLIP_MAX3_4, 0x00224120) &&
			WR(dev, TVW_VFLIP_MAX5_6, 0xff404000) &&
			WR(dev, TVW_VFLIP_CK_SNAP, 0x051e4000) &&
			WR(dev, TVW_STANDARD_CONTROL1_2, 0x70100f19) &&
			WR(dev, TVW_AUTO_SM1, 0x0a085000) &&
			WR(dev, TVW_AUTO_VFLIP1_2, 0x64648440) &&
			WR(dev, TVW_AUTO_VFLIP3_4, 0xff0002ff) &&
			WR(dev, TVW_AUTO_VFLIP5_6, 0x04b41eb4) &&
			WR(dev, TVW_SCART_CTRL, 0x48ff1c00) &&
			WR(dev, TVW_AUTO_READ1, 0x20640650) &&
			WR(dev, TVW_VFLIP1_2_ON, 0x003f2064) &&
			WR(dev, TVW_CBPLL_ABS, 0x019a0254) &&
			WR(dev, TVW_LPF_LL, 0x005005fa) &&
			WR(dev, TVW_FPLL_DBG_PORT, 0xa440200a) &&
			WR(dev, TVW_NOISE_MEASURE_CONTROL, 0x04403434) &&
			WR(dev, TVW_NOISE_MEASURE_VCOUNT_WINDOW, 0x009b0000) &&
			WR(dev, TVW_SECAM_SEP_DR6, 0x00000f48) &&
			WR(dev, TVW_SECAM_SEP_DR7, 0x28000000) &&
			WR(dev, TVW_SECAM_SEP_DR8, 0x00000000) &&
			WR(dev, TVW_SECAM_SEP_DR9, 0xfffff001) &&
			WR(dev, TVW_SECAM_SEP_DR10, 0x11302020) &&
			WR(dev, TVW_SECAM_SEP_DR11, 0x000000ff) &&
			WR(dev, TVW_SECAM_NARROW_TRAP16, 0x00000000) &&
			WR(dev, TVW_SECAM_CONTROL3, 0x00000000) &&
			WR(dev, TVW_SECAM_CONTROL5, 0x00000000) &&
			WR(dev, 0x66d8, 0x00000000) &&
			WR(dev, 0x66dc, 0x4a000000) &&
			WR(dev, TVW_VDEC_INTERRUPT_ENABLE, 0xffff2740) &&
			WR(dev, TVW_VDEC_INTERRUPT_CLEAR, 0x0000ff00) &&
			WR(dev, TVW_VDEC_LOCK_INTR_CTRL, 0xffffffff) &&
			WR(dev, TVW_VDEC_MV_INTR_CTRL, 0x00ff0000) &&
			WR(dev, TVW_VDEC_TRICK_MODE_INTR_CTRL, 0x50603058) &&
			WR(dev, TVW_SECAM_CTRL1, 0x0a806060) &&
			WR(dev, TVW_SECAM_CTRL2, 0xff101028) &&
			WR(dev, TVW_SECAM_BURST2, 0x000007fd) &&
			WR(dev, TVW_SECAM_CLAMP_CTRL3, 0x00000008) &&
			WR(dev, TVW_SECAM_DEEMPHASIS_COEFF, 0x000007f0) &&
			WR(dev, TVW_VDEC_STD_INTR_CTRL, 0x10603210) &&
			WR(dev, TVW_SECAM_SEPARATION_CNTRL, 0x0000001d) &&
			WR(dev, TVW_SECAM_EMPHASIS_BOOST, 0x000007cd) &&
			WR(dev, TVW_SECAM_DIAG_DATA, 0x00000064) &&
			WR(dev, TVW_SECAM_CLAMP_CTRL4, 0x020006bd) &&
			WR(dev, TVW_SECAM_AGC_CTRL4, 0x07d80003) &&
			WR(dev, TVW_SECAM_SEPARATION_CTRL1, 0x001107f3) &&
			WR(dev, TVW_SECAM_CONTROL, 0x0009001e) &&
			WR(dev, TVW_SECAM_CLOCHE_COEFB0, 0x07da07dc) &&
			WR(dev, TVW_SECAM_CLOCHE_COEFB1, 0x00530018) &&
			WR(dev, TVW_SECAM_CLOCHE_COEFA1, 0x07920032) &&
			WR(dev, TVW_VDEC_NOISE_MEASURE_HIGH_INTR_CTRL, 0x00206080) &&
			WR(dev, TVW_SECAM_NOISE_SCALING_THRESH_LOW, 0x028606d2) &&
			WR(dev, TVW_SECAM_NOISE_SCALING_SLOPE, 0xc8c802c8) &&
			WR(dev, TVW_SECAM_NOISE_MEASURE_CONTROL, 0x000014a7) &&
			WR(dev, TVW_SECAM_NOISE_MEASURE_VCOUNT_WINDOW, 0x80001460) &&
			WR(dev, TVW_SECAM_NOISE_MEASUREMENT, 0x00000000) &&
			WR(dev, TVW_SECAM_CARRIER_AMP_CTRL, 0x00010000) &&
			WR(dev, TVW_SECAM_NARROW_TRAP1, 0x00000000) &&
			WR(dev, TVW_SECAM_NARROW_TRAP2, 0x00000000) &&
			WR(dev, TVW_VDEC_NOISE_MEASURE_LOW_INTR_CTRL, 0x10101410) &&
			WR(dev, TVW_SECAM_NARROW_TRAP3, 0x00000000) &&
			WR(dev, TVW_SECAM_NARROW_TRAP4, 0x00080000) &&
			WR(dev, TVW_SECAM_NARROW_TRAP5, 0x112a0028) &&
			WR(dev, TVW_SECAM_NARROW_TRAP6, 0x00440e58) &&
			WR(dev, TVW_SECAM_NARROW_TRAP7, 0x80be2204) &&
			WR(dev, TVW_SECAM_NARROW_TRAP8, 0x20202cb4) &&
			WR(dev, TVW_SECAM_NARROW_TRAP9, 0x000004c3) &&
			WR(dev, TVW_SECAM_NARROW_TRAP10, 0x61500000) &&
			WR(dev, TVW_SECAM_NARROW_TRAP11, 0x64304180) &&
			WR(dev, TVW_SECAM_NARROW_TRAP12, 0x18381838) &&
			WR(dev, TVW_VDEC_NOISE_INTR_TRIGGER, 0x8f601064) &&
			WR(dev, TVW_SECAM_NARROW_TRAP13, 0x00020077) &&
			WR(dev, TVW_SECAM_NARROW_TRAP14, 0x00000011) &&
			WR(dev, TVW_VDEC_VSYNC_INTR_CTRL, 0xf5f214cc) &&
			WR(dev, TVW_VDEC_FIELD_ID_INTR_CTRL, 0x0000241e) &&
			WR(dev, TVW_VDEC_DEBUG1_INTR_CTRL, 0x80a5c623) &&
			WR(dev, TVW_VDEC_MD_CTRL, 0x10f2a9f3) &&
			WR(dev, TVW_MOTION_1D_2D, 0x041b414b) &&
			WR(dev, TVW_TRICK_AUTO_STANDARD1_2, 0x2ea24a41) &&
			WR(dev, TVW_VSYNC_SLICE3, 0x3582ec37) &&
			WR(dev, TVW_SAME_CH_3D1_2, 0xb8022d27) &&
			WR(dev, 0x6568, 0xdded450c) &&
			WR(dev, 0x656c, 0x37523c37) &&
			WR(dev, TVW_SECAM_SEP_DB0, 0xc8021f26) &&
			WR(dev, TVW_SECAM_SEP_DB1, 0x0aea44dd) &&
			WR(dev, TVW_SECAM_SEP_DB2, 0x13149b94) &&
			WR(dev, TVW_SECAM_SEP_DB3, 0x6be57925) &&
			WR(dev, TVW_SECAM_SEP_DB4, 0x5a1049ca) &&
			WR(dev, TVW_SECAM_SEP_DB5, 0xe31ea3c9) &&
			WR(dev, TVW_SECAM_SEP_DB6, 0x97fe3679) &&
			WR(dev, TVW_SECAM_SEP_DB7, 0xca863cf3) &&
			WR(dev, TVW_SECAM_SEP_DB8, 0x802073c9) &&
			WR(dev, TVW_SECAM_SEP_DB9, 0x04088025) &&
			WR(dev, TVW_SECAM_SEP_DB10, 0x04800c0f) &&
			WR(dev, TVW_SECAM_SEP_DB11, 0x20000140) &&
			WR(dev, TVW_SECAM_SEP_DB12, 0x88888888) &&
			WR(dev, TVW_SECAM_SEP_DB13, 0x88887888) &&
			WR(dev, TVW_SECAM_SEP_DB14, 0x88886788) &&
			WR(dev, TVW_SECAM_SEP_DB15, 0x56780004) &&
			WR(dev, TVW_SECAM_SEP_DB16, 0x45680023) &&
			WR(dev, TVW_SECAM_SEP_DR0, 0x23460002) &&
			WR(dev, TVW_SECAM_SEP_DR1, 0x12350001) &&
			WR(dev, TVW_SECAM_SEP_DR2, 0x00000000) &&
			WR(dev, TVW_SECAM_SEP_DR3, 0x44030a05) &&
			WR(dev, TVW_SECAM_SEP_DR12, 0x20102840) &&
			WR(dev, TVW_SECAM_SEP_DR13, 0x08800600) &&
			WR(dev, TVW_SECAM_SEP_DR14, 0x281440ff) &&
			WR(dev, TVW_SECAM_SEP_DR15, 0x28142801) &&
			WR(dev, TVW_SECAM_SEP_DR16, 0x14288000) &&
			WR(dev, 0x65f8, 0x60ff283f) &&
			WR(dev, TVW_SECAM_AGC_CTRL1, 0x0b10302a) &&
			WR(dev, TVW_SECAM_AGC_CTRL2, 0xbe108b6b) &&
			WR(dev, TVW_SECAM_AGC_CTRL3, 0x04ff1010) &&
			WR(dev, TVW_SECAM_CTRL0, 0x0044796a) &&
			WR(dev, TVW_VDEC_VSYNC_INTR_CTRL, 0xcaf214cc) &&
			WR(dev, TVW_SECAM_SEP_DR13, 0x08808600) &&
			WR(dev, TVW_SECAM_NOISE_SCALING_SLOPE, 0xc8c800c8) &&
			WR(dev, TVW_SECAM_NARROW_TRAP9, 0x00000443) &&
			WR(dev, TVW_SECAM_SEP_DR12, 0x600b2880) &&
			WR(dev, TVW_SECAM_AGC_CTRL1, 0x0b103040) &&
			WR(dev, TVW_VDEC_VIDEO_FLAGS, 0x04107dcf) &&
			WR(dev, TVW_NON_STANDARD_FLAG4, 0x430a480a) &&
			WR(dev, TVW_SECAM_NOISE_MEASURE_CONTROL, 0x100014a3) &&
			WR(dev, TVW_SECAM_CTRL2, 0xff200405) &&
			MR(dev, TVW_SECAM_NARROW_TRAP14, ALL, 0x00000007) &&
			WR(dev, 0x6b28, 0x0000010c) &&
			WR(dev, 0x6938, 0x000080ff) &&
			WR(dev, 0x6938, 0x000000ff) &&
			WR(dev, 0x6938, 0x000001ff) &&
			MR(dev, 0x68ec, ALL, 0) &&
			MR(dev, TVW_W9_10, ALL, 0x80008000) &&
			MR(dev, TVW_W11, ALL, 0x80008000) &&
			MR(dev, 0x6860, ALL, 0x00000004) &&
			WR(dev, 0x6904, 0xe40f100f) &&
			WR(dev, TVW_PWM1_CLAMP_CTRL, 0xe4e4000e) &&
			WR(dev, TVW_AGC_CONTROL_4_5, 0x81008000) &&
			WR(dev, 0x6b70, 0x00013000) &&
			MR(dev, TVW_MEMORY_MARGIN_CTL0, ALL, 0x00200000) &&
			WR(dev, 0x6b50, 0x01000000) &&
			WR(dev, 0x6938, 0x00008000) &&
			MR(dev, TVW_MEMORY_MARGIN_CTL0, ALL, 0) &&
			MR(dev, TVW_VDEC_NOISE_INTR_TRIGGER, ALL, 0) &&
			MR(dev, TVW_VDEC_FIFO_CTL, ALL, 0x00080000) &&
			WR(dev, TVW_HLOCK_ACQUIRE_LOW_LL_WIN, 0x00001c1a) &&
			WR(dev, TVW_HLOCK_ACQUIRE_LOW_LOST, 0x00001e18) &&
			MR(dev, TVW_DVSO_CRTC_SETTINGS, 0xfff3fffb, 0x00700010) &&
			WR(dev, TVW_DVSO_CRTC_VSTART, 0x01390001) &&
			WR(dev, TVW_DVSO_CRTC_VBI_START_LINE, 0x01400007) &&
			WR(dev, TVW_DVSO_CRTC_VID_START_LINE, 0x0153001a) &&
			MR(dev, TVW_VBIST_GENERATOR, 0xffffff3f, 0) &&
			MR(dev, TVW_VBIST_GENERATOR, ALL, 0x000000c8) &&
			MR(dev, TVW_VBIST_GENERATOR, ALL, 0x00000002) &&
			WR(dev, TVW_VBIST_VBI_HEIGHT, 0x00000002) &&
			WR(dev, TVW_VBIST_VID_WIDTH, 0x000002d0) &&
			WR(dev, TVW_VBIST_VID_HEIGHT, 0x0000011a) &&
			MR(dev, TVW_CPU_HALF_BAND, ALL, 0) &&
			MR(dev, TVW_CPU_HALF_BAND, ALL, 0) &&
			WR(dev, 0x68e4, 0x0000011f) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x00000000) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x00000001) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x00000002) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x00000003) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x00000004) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x00000005) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x00000006) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x00000007) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x00000008) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x00000009) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x0000000a) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x0000000b) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x0000000c) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x0000000d) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x0000000e) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x0000000f) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x00000010) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x00000011) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x00000012) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x00000013) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x00000014) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x00000015) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x00000016) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x00000017) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x00000018) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x00000019) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x0000001a) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x0000001b) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x0000001c) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x0000001d) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x0000001e) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x0000001f) &&
			WR(dev, 0x68e0, 0x00000000) &&
			WR(dev, 0x68dc, 0x0000000a) &&
			WR(dev, 0x68e0, 0x000000ff) &&
			MR(dev, 0x6b34, ALL, 0) &&
			WR(dev, TVW_SYNC_LOW_DB5, 0x00000294) &&
			WR(dev, 0x68e4, 0x0000011f) &&
			MR(dev, 0x6b20, ALL, 0) &&
			MR(dev, 0x6ad0, ALL, 0) &&
			MR(dev, 0x6ad4, ALL, 0) &&
			WR(dev, 0x68e0, 0x00000ffc) &&
			// XXX 2x read of 0x68f8 (ret 0x6) before each write
			WR(dev, 0x68dc, 0x00000000) &&
			WR(dev, 0x68e0, 0x00000ffc) &&
			WR(dev, 0x68dc, 0x00000001) &&
			WR(dev, 0x68e0, 0x00000ffc) &&
			WR(dev, 0x68dc, 0x00000002) &&
			WR(dev, 0x68e0, 0x00000ffc) &&
			WR(dev, 0x68dc, 0x00000003) &&
			WR(dev, 0x68e0, 0x00000ffc) &&
			WR(dev, 0x68dc, 0x00000004) &&
			WR(dev, 0x68e0, 0x00000ffc) &&
			WR(dev, 0x68dc, 0x00000005) &&
			WR(dev, 0x68e0, 0x00000ffc) &&
			WR(dev, 0x68dc, 0x00000006) &&
			WR(dev, 0x68e0, 0x00000ffc) &&
			WR(dev, 0x68dc, 0x00000007) &&
			WR(dev, 0x68e0, 0x00000ffc) &&
			WR(dev, 0x68dc, 0x00000008) &&
			WR(dev, 0x68e0, 0x00000ffc) &&
			WR(dev, 0x68dc, 0x00000009) &&
			WR(dev, 0x68e0, 0x00000ffc) &&
			WR(dev, 0x68dc, 0x0000000a) &&
			WR(dev, 0x68e0, 0x00000ffc) &&
			WR(dev, 0x68dc, 0x0000000b) &&
			WR(dev, 0x68e0, 0x00000ffc) &&
			WR(dev, 0x68dc, 0x0000000c) &&
			WR(dev, 0x68e0, 0x00000ffc) &&
			WR(dev, 0x68dc, 0x0000000d) &&
			WR(dev, 0x68e0, 0x00000ffc) &&
			WR(dev, 0x68dc, 0x0000000e) &&
			WR(dev, 0x68e0, 0x00000ffc) &&
			WR(dev, 0x68dc, 0x0000000f) &&
			WR(dev, 0x68e0, 0x00000ffc) &&
			WR(dev, 0x68dc, 0x00000010) &&
			WR(dev, 0x68e0, 0x00000ffc) &&
			WR(dev, 0x68dc, 0x00000011) &&
			WR(dev, 0x68e0, 0x00000ffc) &&
			WR(dev, 0x68dc, 0x00000012) &&
			WR(dev, 0x68e0, 0x0000000c) &&
			WR(dev, 0x68dc, 0x00000013) &&
			WR(dev, 0x68e0, 0x0000000c) &&
			WR(dev, 0x68dc, 0x00000014) &&
			WR(dev, 0x68e0, 0x0000000c) &&
			WR(dev, 0x68dc, 0x00000015) &&
			WR(dev, 0x68e0, 0x0000000c) &&
			WR(dev, 0x68dc, 0x00000016) &&
			WR(dev, 0x68e0, 0x0000000c) &&
			WR(dev, 0x68dc, 0x00000017) &&
			WR(dev, 0x68e0, 0x0000000c) &&
			WR(dev, 0x68dc, 0x00000018) &&
			WR(dev, 0x68e0, 0x0000000c) &&
			WR(dev, 0x68dc, 0x00000019) &&
			WR(dev, 0x68e0, 0x0000000c) &&
			WR(dev, 0x68dc, 0x0000001a) &&
			WR(dev, 0x68e0, 0x0000000c) &&
			WR(dev, 0x68dc, 0x0000001b) &&
			WR(dev, 0x68e0, 0x0000000c) &&
			WR(dev, 0x68dc, 0x0000001c) &&
			WR(dev, 0x68e0, 0x0000000c) &&
			WR(dev, 0x68dc, 0x0000001d) &&
			WR(dev, 0x68e0, 0x0000000c) &&
			WR(dev, 0x68dc, 0x0000001e) &&
			WR(dev, 0x68e0, 0x0000000c) &&
			WR(dev, 0x68dc, 0x0000001f) &&
			// XXX 2x read before all of these RMW, none in between
			MR(dev, 0x6b34, ALL, 0) &&
			MR(dev, 0x6b38, ALL, 0x00000002) &&
			MR(dev, 0x68a8, ALL, 0) &&
			// XXX 4x read to finish it off

			MR(dev, 0x6b20, ALL, 0) &&
			MR(dev, TVW_CATV_DB1_2, ALL, 0xc0000000) &&
			MR(dev, 0x6ad4, ALL, 0) &&
			MR(dev, 0x6ad0, ALL, 0) &&
			MR(dev, 0x6a90, ALL, 0) &&
			MR(dev, 0x6ae0, ALL, 0) &&
			MR(dev, 0x6ad8, ALL, 0) &&
			MR(dev, 0x68f0, ALL, 0) &&

			// XXX this E0 magic seems to trigger ISOCH! XXX
			// enable ISOCH
			tvw_e0_magic(dev, 0x50060000, 0x02070120) &&
			MR(dev, TVW_RESET_CONTROL, 0xfffff7ff, 0) &&
			// TVW_ADEC_MATRIX_CONTROL
			// TVW_ADEC_RAMPING_PERIOD
			WR(dev, TVW_ADEC_RAMPING_TARGET_VOLUME, 0) &&

			true;
}

bool
tvw_tune_tvw(tvwsdr_dev_t *dev) {
	return
			// ???
			MU(dev, 0xf101, UC_ALL, 0x02) &&
			WU(dev, 0xf090, 0) &&
			MSLEEP(10) &&
			// read 0xf091
			WR(dev, TVW_PATCH_REG, 0) &&
			WU(dev, 0xf0b2, 0) &&
			WU(dev, 0xf0b3, 0) &&
			WU(dev, 0xf0b4, 0) &&
			WU(dev, 0xf0b5, 0) &&

			MR(dev, TVW_CLOCK_CONTROL_0, ALL, 0x00400000) &&

			// ???
			MR(dev, 0x08f8, 0xfffff0f0, 0) &&
			MR(dev, TVW_VIDEO_INPUT_0_CONTROL, ALL, 0xa01f0000) &&
			MR(dev, TVW_VIDEO_ADC_0_CONTROL, 0x20003080, 0x000f8524) &&
			MR(dev, TVW_VIDEO_INPUT_1_CONTROL, ALL, 0x001f0000) &&
			MR(dev, TVW_VIDEO_ADC_1_CONTROL, 0x20003080, 0x020f853e) &&

			MU(dev, 0xf00d, UC_ALL, 0x01) &&
			MU(dev, 0xf00d, 0xfe, 0) &&

			WU(dev, 0xfb3d, 0) &&

			// ???
			MU(dev, 0xf00b, UC_ALL, 0x40) &&
			MU(dev, 0xf00b, 0xbf, 0) &&
			MU(dev, 0xf02b, 0, 0x04) &&
			MU(dev, 0xf00d, UC_ALL, 0x01) &&
			MU(dev, 0xf00d, 0xfe, 0) &&
			MU(dev, 0xf00d, 0xfc, 0x02) &&
			MU(dev, 0xf00d, 0xfc, 0) &&

			WU(dev, 0xf201, 0xfa) &&
			WU(dev, 0xf202, 0x15) &&
			WU(dev, 0xf203, 0xc4) &&
			WU(dev, 0xf209, 0x55) &&
			WU(dev, 0xf20a, 0x2c) &&
			WU(dev, 0xf20b, 0xf4) &&
			WU(dev, 0xf200, 0x03) &&

			WU(dev, 0xf255, 0x96) &&
			WU(dev, 0xf256, 0x01) &&
			WU(dev, 0xf257, 0x3c) &&
			WU(dev, 0xf258, 0x91) &&

			WU(dev, 0xf140, 0x03) &&
			WU(dev, 0xf141, 0x00) &&
			WU(dev, 0xf142, 0x00) &&
			WU(dev, 0xf143, 0x10) &&
			WU(dev, 0xf144, 0x00) &&
			WU(dev, 0xf145, 0x0e) &&
			WU(dev, 0xf146, 0x1a) &&
			WU(dev, 0xf147, 0x10) &&
			WU(dev, 0xf148, 0x00) &&
			WU(dev, 0xf149, 0x0e) &&
			WU(dev, 0xf14a, 0x1a) &&
			WU(dev, 0xf1d9, 0x43) &&
			WU(dev, 0xf1da, 0xd3) &&
			WU(dev, 0xf1db, 0x3f) &&
			WU(dev, 0xf1dc, 0xeb) &&
			WU(dev, 0xf14b, 0x00) &&
			WU(dev, 0xf14c, 0x44) &&
			WU(dev, 0xf153, 0x01) &&
			WU(dev, 0xf0b2, 0xff) &&
			WU(dev, 0xf0b3, 0x66) &&
			WU(dev, 0xf0b4, 0x00) &&
			WU(dev, 0xf0b5, 0xbc) &&
			WU(dev, 0xf0b0, 0xff) &&
			WU(dev, 0xf0b1, 0x6c) &&
			WU(dev, 0xf154, 0x0f) &&
			WU(dev, 0xf155, 0x6c) &&
			WU(dev, 0xf156, 0x59) &&
			WU(dev, 0xf157, 0x13) &&
			WU(dev, 0xf158, 0xea) &&
			WU(dev, 0xf159, 0x7f) &&
			WU(dev, 0xf15a, 0xb4) &&
			WU(dev, 0xf15b, 0x49) &&
			WU(dev, 0xf15c, 0x89) &&
			WU(dev, 0xf15d, 0xc4) &&
			WU(dev, 0xf15e, 0xe0) &&
			WU(dev, 0xf15f, 0x31) &&
			WU(dev, 0xf160, 0x72) &&
			WU(dev, 0xf161, 0x71) &&
			WU(dev, 0xf162, 0x35) &&
			WU(dev, 0xf163, 0x71) &&
			WU(dev, 0xf164, 0x19) &&
			WU(dev, 0xf165, 0xf6) &&
			WU(dev, 0xf166, 0x34) &&
			WU(dev, 0xf167, 0x0e) &&
			WU(dev, 0xf168, 0xd1) &&
			WU(dev, 0xf169, 0xbd) &&
			WU(dev, 0xf16a, 0x10) &&
			WU(dev, 0xf16b, 0x57) &&
			WU(dev, 0xf16c, 0xfc) &&
			WU(dev, 0xf16d, 0x82) &&
			WU(dev, 0xf16e, 0x57) &&
			WU(dev, 0xf16f, 0x23) &&
			WU(dev, 0xf170, 0xbb) &&
			WU(dev, 0xf171, 0xaf) &&
			WU(dev, 0xf172, 0xdd) &&
			WU(dev, 0xf173, 0x75) &&
			WU(dev, 0xf174, 0x76) &&
			WU(dev, 0xf175, 0x28) &&
			WU(dev, 0xf176, 0xa4) &&
			WU(dev, 0xf177, 0x01) &&
			WU(dev, 0xf178, 0x00) &&
			WU(dev, 0xf179, 0xd9) &&
			WU(dev, 0xf17a, 0x4d) &&
			WU(dev, 0xf17b, 0x03) &&
			WU(dev, 0xf17c, 0x80) &&
			WU(dev, 0xf17d, 0x6d) &&
			WU(dev, 0xf17e, 0x1f) &&
			WU(dev, 0xf17f, 0x00) &&
			WU(dev, 0xf181, 0x04) &&
			WU(dev, 0xf182, 0x08) &&
			WU(dev, 0xf183, 0x00) &&
			WU(dev, 0xf184, 0x0f) &&
			WU(dev, 0xf185, 0x09) &&
			WU(dev, 0xf186, 0x43) &&
			WU(dev, 0xf187, 0x6e) &&
			WU(dev, 0xf188, 0x89) &&
			WU(dev, 0xf189, 0x56) &&
			WU(dev, 0xf18a, 0x73) &&
			WU(dev, 0xf18b, 0x33) &&
			WU(dev, 0xf18c, 0xa8) &&
			WU(dev, 0xf190, 0x08) &&
			WU(dev, 0xf191, 0x0c) &&
			WU(dev, 0xf192, 0x18) &&
			WU(dev, 0xf194, 0x00) &&
			WU(dev, 0xf195, 0x03) &&
			WU(dev, 0xf196, 0x06) &&
			WU(dev, 0xf197, 0x08) &&
			WU(dev, 0xf198, 0x10) &&
			WU(dev, 0xf19a, 0x06) &&
			WU(dev, 0xf19b, 0x00) &&
			WU(dev, 0xf19c, 0x0f) &&
			WU(dev, 0xf1e4, 0x00) &&
			WU(dev, 0xf1e3, 0x00) &&
			WU(dev, 0xf1a3, 0x0a) &&
			WU(dev, 0xf1a5, 0x4a) &&
			WU(dev, 0xf1a6, 0x5a) &&
			WU(dev, 0xf1a7, 0x0a) &&
			WU(dev, 0xf1a8, 0x09) &&
			WU(dev, 0xf1a9, 0x0f) &&
			WU(dev, 0xf1aa, 0x58) &&
			WU(dev, 0xf1ab, 0x1e) &&
			WU(dev, 0xf1ac, 0xb0) &&
			WU(dev, 0xf1ad, 0xf9) &&
			WU(dev, 0xf1ae, 0xdd) &&
			WU(dev, 0xf1af, 0x06) &&
			WU(dev, 0xf1b0, 0x23) &&
			WU(dev, 0xf1b1, 0xf0) &&
			WU(dev, 0xf1b2, 0xa8) &&
			WU(dev, 0xf1b3, 0x0f) &&
			WU(dev, 0xf1b4, 0x58) &&
			WU(dev, 0xf1b5, 0xd9) &&
			WU(dev, 0xf1b6, 0xa3) &&
			WU(dev, 0xf1b7, 0x13) &&
			WU(dev, 0xf1b8, 0xf3) &&
			WU(dev, 0xf1b9, 0x05) &&
			WU(dev, 0xf1ba, 0x0f) &&
			WU(dev, 0xf1bc, 0x40) &&
			WU(dev, 0xf1e1, 0x12) &&
			WU(dev, 0xf1bd, 0x04) &&
			WU(dev, 0xf1be, 0x9a) &&
			WU(dev, 0xf1d5, 0x45) &&
			WU(dev, 0xf1d6, 0x20) &&
			WU(dev, 0xf1d7, 0x66) &&
			WU(dev, 0xf1bf, 0x4c) &&
			WU(dev, 0xf1c0, 0x3d) &&
			WU(dev, 0xf1c1, 0x30) &&
			WU(dev, 0xf1c2, 0x31) &&
			WU(dev, 0xf1c3, 0x13) &&
			WU(dev, 0xf1c4, 0x15) &&
			WU(dev, 0xf1c5, 0x10) &&
			WU(dev, 0xf1c6, 0x02) &&
			WU(dev, 0xf1c7, 0x14) &&
			WU(dev, 0xf1c9, 0x84) &&
			WU(dev, 0xf1d8, 0x0e) &&
			WU(dev, 0xf1ca, 0xff) &&
			WU(dev, 0xf1cb, 0x20) &&
			WU(dev, 0xf1cd, 0x10) &&
			WU(dev, 0xf1ce, 0x21) &&
			WU(dev, 0xf1d3, 0x04) &&
			WU(dev, 0xf1e2, 0x05) &&
			WU(dev, 0xf1d4, 0x39) &&
			WU(dev, 0xf19d, 0x04) &&
			WU(dev, 0xf19e, 0xd9) &&
			WU(dev, 0xf1e5, 0x02) &&
			WU(dev, 0xf1e6, 0x03) &&
			WU(dev, 0xf1e7, 0x00) &&
			WU(dev, 0xf1e8, 0x00) &&
			WU(dev, 0xf1e9, 0x02) &&
			WU(dev, 0xf1ec, 0x00) &&
			WU(dev, 0xf1ed, 0x0c) &&
			WU(dev, 0xf1ef, 0x10) &&
			WU(dev, 0xf1ec, 0x00) &&
			WU(dev, 0xf1ed, 0x0e) &&
			WU(dev, 0xf1ef, 0x11) &&
			WU(dev, 0xf1ec, 0x00) &&
			WU(dev, 0xf1ed, 0x13) &&
			WU(dev, 0xf1ef, 0x12) &&
			WU(dev, 0xf1ec, 0x00) &&
			WU(dev, 0xf1ed, 0x1a) &&
			WU(dev, 0xf1ef, 0x13) &&
			WU(dev, 0xf1ec, 0x00) &&
			WU(dev, 0xf1ed, 0x25) &&
			WU(dev, 0xf1ef, 0x14) &&
			WU(dev, 0xf1ec, 0x00) &&
			WU(dev, 0xf1ed, 0x32) &&
			WU(dev, 0xf1ef, 0x15) &&
			WU(dev, 0xf1ec, 0x00) &&
			WU(dev, 0xf1ed, 0x40) &&
			WU(dev, 0xf1ef, 0x16) &&
			WU(dev, 0xf1ec, 0x00) &&
			WU(dev, 0xf1ed, 0x50) &&
			WU(dev, 0xf1ef, 0x17) &&
			WU(dev, 0xf1ec, 0x00) &&
			WU(dev, 0xf1ed, 0x60) &&
			WU(dev, 0xf1ef, 0x18) &&
			WU(dev, 0xf1ec, 0x00) &&
			WU(dev, 0xf1ed, 0x70) &&
			WU(dev, 0xf1ef, 0x19) &&
			WU(dev, 0xf1ec, 0x00) &&
			WU(dev, 0xf1ed, 0x7e) &&
			WU(dev, 0xf1ef, 0x1a) &&
			WU(dev, 0xf1ec, 0x00) &&
			WU(dev, 0xf1ed, 0x8c) &&
			WU(dev, 0xf1ef, 0x1b) &&
			WU(dev, 0xf1ec, 0x00) &&
			WU(dev, 0xf1ed, 0x97) &&
			WU(dev, 0xf1ef, 0x1c) &&
			WU(dev, 0xf1ec, 0x00) &&
			WU(dev, 0xf1ed, 0x9f) &&
			WU(dev, 0xf1ef, 0x1d) &&
			WU(dev, 0xf1ec, 0x00) &&
			WU(dev, 0xf1ed, 0xa4) &&
			WU(dev, 0xf1ef, 0x1e) &&
			WU(dev, 0xf1ec, 0x00) &&
			WU(dev, 0xf1ed, 0x53) &&
			WU(dev, 0xf1ef, 0x1f) &&
			WU(dev, 0x0017, 0xd0) &&
			WU(dev, 0xf157, 0x09) &&
			WU(dev, 0xf158, 0xeb) &&
			WU(dev, 0xf159, 0x08) &&
			WU(dev, 0xf15a, 0x97) &&
			WU(dev, 0xf15b, 0x67) &&
			WU(dev, 0xf15c, 0x02) &&
			WU(dev, 0xf15d, 0x2b) &&
			WU(dev, 0xf15e, 0x27) &&
			WU(dev, 0xf15f, 0x14) &&
			WU(dev, 0xf160, 0x7f) &&
			WU(dev, 0xf161, 0x13) &&
			WU(dev, 0xf162, 0x82) &&
			WU(dev, 0xf163, 0x7a) &&
			WU(dev, 0xf164, 0x94) &&
			WU(dev, 0xf165, 0x65) &&
			WU(dev, 0xf166, 0xc8) &&
			WU(dev, 0xf167, 0x07) &&
			WU(dev, 0xf168, 0x66) &&
			WU(dev, 0xf169, 0x81) &&
			WU(dev, 0xf16a, 0x70) &&
			WU(dev, 0xf16b, 0x6d) &&
			WU(dev, 0xf16c, 0x46) &&
			WU(dev, 0xf16d, 0xd3) &&
			WU(dev, 0xf16e, 0xf2) &&
			WU(dev, 0xf16f, 0x0f) &&
			WU(dev, 0xf170, 0x68) &&
			WU(dev, 0xf171, 0xde) &&
			WU(dev, 0xf172, 0xeb) &&
			WU(dev, 0xf173, 0x7b) &&
			WU(dev, 0xf174, 0xe9) &&
			WU(dev, 0xf175, 0xe4) &&
			WU(dev, 0xf176, 0x09) &&
			WU(dev, 0xf177, 0x00) &&
			WU(dev, 0xf178, 0xf2) &&
			WU(dev, 0xf0b4, 0x00) &&
			WU(dev, 0xf0b5, 0xb9) &&
			WU(dev, 0xf179, 0xd1) &&
			WU(dev, 0xf17a, 0x84) &&
			WU(dev, 0xf0b2, 0xff) &&
			WU(dev, 0xf0b3, 0x6c) &&
			WU(dev, 0xf154, 0x0f) &&
			WU(dev, 0xf155, 0x2b) &&
			WU(dev, 0xf1ba, 0x0e) &&
			WU(dev, 0xf1bc, 0x17) &&
			WU(dev, 0xfb33, 0x00) &&
			WU(dev, 0xfb34, 0xb1) &&
			WU(dev, 0xf156, 0x40) &&
			WU(dev, 0xf0ba, 0x00) &&
			WU(dev, 0xf101, 0x04) &&

			tvw_uc_service_call(dev, 0x1c, 3, 0x01, 0x02, 0x00) &&

			WU(dev, 0xf092, 0x16) &&
			WU(dev, 0xf090, 0x10) &&
			// needed?
			WU(dev, 0xf201, 0xfa) &&
			WU(dev, 0xf202, 0x15) &&
			WU(dev, 0xf203, 0xc4) &&
			WU(dev, 0xf209, 0x55) &&
			WU(dev, 0xf20a, 0x2c) &&
			WU(dev, 0xf20b, 0xf4) &&
			WU(dev, 0xf200, 0x03) &&
			MSLEEP(10) &&
			WU(dev, 0xf063, 0x01) &&
			WU(dev, 0xf101, 0x04) &&

			true;
}

int
tvwsdr_open(tvwsdr_dev_t **out_dev, uint32_t freq) {
	int ret;
	uint32_t val;
	unsigned char buf[39];
	tvwsdr_dev_t *dev = NULL;

	dev = malloc(sizeof(tvwsdr_dev_t));
	if (NULL == dev) {
		return -ENOMEM;
	}

	memset(dev, 0, sizeof(tvwsdr_dev_t));

	libusb_init(&dev->ctx);

	dev->devh = libusb_open_device_with_vid_pid(dev->ctx, 0x0438, 0xac14);
	if (NULL == dev->devh) {
		warnx("failed to open usb device");
		goto err;
	}

	if (libusb_claim_interface(dev->devh, 0)) {
		warnx("failed to claim interface 0");
		goto err;
	}

	// set up T507
	fprintf(stderr, "Running init 1...\n");
	if (!tvw_init_tvw_1(dev) || !tvw_init_tvw_2(dev, 1) || !tvw_init_tvw_3(dev)) {
		warnx("failed to init T507");
		goto err;
	}

	fprintf(stderr, "Uploading first firmware...\n");
	ret = tvw_load_fw(dev, "CTRLT507-fixed.bin");
	if (ret) {
		warnx("failed to load CTRLT507 firmware");
		goto err;
	}

#if 1
	if (internal_read_reg(dev, TVW_USC_CTL, 0x2000, buf, 4)) {
		fprintf(stderr, "failed to read 0x200c\n");
		return -1;
	}
	if (internal_write_reg(dev, TVW_USC_CTL, 0x2000, buf, 4)) {
		fprintf(stderr, "failed to write 0x200c\n");
		return -1;
	}
#endif

	/* this register read doesn't follow the pattern of the rest... */
	/* but it is critical! see comments in internal_read_reg */
	/* it likely sets the MIPS base address */
	if (internal_read_reg(dev, 0x0000, 0x8000, buf, 0)) {
		fprintf(stderr, "failed to do odd register read\n");
		return -1;
	}

	val = htole32(0x00000050);
	if (internal_write_reg(dev, 0xe000, 0x1000, &val, 4)) {
		warnx("failed to do E0 magic after firmware upload");
		goto err;
	}

	fprintf(stderr, "Running init 2...\n");
	if (!tvw_finish_init_tvw(dev)) {
		warnx("failed to finish T507 init");
		goto err;
	}

#if 0
	// SPI
	WR(dev, TVW_SPIC_PRESCALE, 0x0000000a) &&
	XR(dev, TVW_SPIC_STATUS, 0x00000001, 0x00000000) &&
	WR(dev, TVW_SPIC_INSTR_ADDR, 0x05000000) &&
	XR(dev, TVW_SPIC_STATUS, 0x00000001, 0x00000000) &&
	true;

	// original offset & amount
	//uint16_t addr = 0xfc00, count = 512 >> 3;
	// entire 512kbit flash
	uint16_t addr = 0x0000, count = 65536 >> 3;
	uint32_t val0, val1;
	for (uint16_t off = 0; off < count; off++) {
		WR(dev, TVW_SPIC_INSTR_ADDR, 0xff000000 | addr) &&
		XR(dev, TVW_SPIC_STATUS, 0x00000001, 0x00000000) &&
		RR(dev, TVW_SPIC_RDATA0, &val0) &&
		RR(dev, TVW_SPIC_RDATA1, &val1) &&
		true;
		if (val0 != 0xffffffff && val1 != 0xffffffff) {
			fprintf(stderr, "%04x: %08x %08x\n", addr + (off << 3), be32toh(val0), be32toh(val1));
		}
	}

	WR(dev, TVW_SPIC_INSTR_ADDR, 0x05000000) &&
	XR(dev, TVW_SPIC_STATUS, 0x00000001, 0x00000000) &&
	WR(dev, TVW_SPIC_PRESCALE, 0x00000001) &&
	true;
#endif

	/* switch to bAlternateSetting 1 */
	if (libusb_set_interface_alt_setting(dev->devh, 0, 1)) {
		warnx("failed to set alternate setting 1");
		goto err;
	}

	// set up frontend/DSP/Nxt/Xilleon/Theater/whatever it is
	fprintf(stderr, "Running init 3...\n");
	if (!tvw_init_8051(dev)) {
		warnx("failed to init 8051");
		goto err;
	}

	fprintf(stderr, "Uploading second firmware...\n");
	ret = tvw_load_fw(dev, "T507.bin");
	if (ret) {
		warnx("failed to load T507 firmware");
		goto err;
	}

	/* XXX make into function */
	if (internal_read_reg(dev, 0xe000, 0x1000, buf, 4)) {
		warnx("failed to read 0xe000");
		goto err;
	}

	// 0x1301000001001a8e0000000000000000 
	memset(buf, 0, sizeof(buf));
	buf[0] = 0x13;
	buf[1] = 0x01;
	buf[4] = 0x01;
	/* 0x8e1a is T507.bin fw size */
	buf[6] = 0x1a;
	buf[7] = 0x8e;
	if (internal_write_reg(dev, 0xe000, 0x1000, buf, 16)) {
		warnx("failed to write 0xe000");
		goto err;
	}

	sleep(1);

	if (internal_read_reg(dev, 0xe010, 0x1000, buf, 16)) {
		warnx("failed to read 0xe010");
		goto err;
	}

	memset(buf, 0, 4);
	if (internal_write_reg(dev, 0xe010, 0x1000, buf, 4)) {
		warnx("failed to write 0xe010");
		goto err;
	}

	fprintf(stderr, "Running init 4...\n");
	if (!tvw_finish_init_8051(dev)) {
		warnx("failed to finish 8051 init");
		goto err;
	}

	/* tuner config over I2C */
	fprintf(stderr, "Configuring tuner...\n");
	dev->fe.frontend_priv = dev;
	tda18271_attach(&dev->fe, 0, NULL, NULL);

	// init 5
	// init 6
	fprintf(stderr, "Running init 5...\n");
	if (!tvw_init56_tvw(dev)) {
		warnx("failed to do init56");
		goto err;
	}

	// init adec
#if 1 // XXX 0
	fprintf(stderr, "Running init ADEC...\n");
	if (!tvw_init_adec(dev)) {
		warnx("failed to do init_adec 1/4");
		goto err;
	}
	if (!tvw_init_adec(dev)) { // XXX not 100% match
		warnx("failed to do init_adec 2/4");
		goto err;
	}
#endif

	// init unknown
	fprintf(stderr, "Running init unknown...\n");
	if (!tvw_init_unknown_1(dev) || !tvw_init_tvw_2(dev, 2) || !tvw_init_unknown_2(dev)) {
		warnx("failed to do init_unknown");
		goto err;
	}

	// init adec again?
#if 1
	fprintf(stderr, "Running init ADEC (again)...\n");
	if (!tvw_init_adec(dev)) {
		warnx("failed to do init_adec 3/4");
		goto err;
	}
	if (!tvw_init_adec(dev)) { // XXX not 100% match
		warnx("failed to do init_adec 4/4");
		goto err;
	}
#endif

	// enable ADEC?
	if (!tvw_e0_magic(dev, 0x00000000, 0x00140130)) {
		warnx("failed to do final E0 magic");
		goto err;
	}

	// tune to freq
	u32 bw = 0; // this is okay for analog
	struct tda18271_std_map_item *map = &(((struct tda18271_priv *)dev->fe.tuner_priv)->std.fm_radio);

	((struct tda18271_priv *)dev->fe.tuner_priv)->if_freq = map->if_freq;
	((struct tda18271_priv *)dev->fe.tuner_priv)->frequency = freq;
	((struct tda18271_priv *)dev->fe.tuner_priv)->bandwidth = bw;

	((struct tda18271_priv *)dev->fe.tuner_priv)->mode = TDA18271_ANALOG;
	//((struct tda18271_priv *)dev->fe.tuner_priv)->mode = TDA18271_DIGITAL;
	if (tda18271_tune(&dev->fe, map, freq, bw)) {
		fprintf(stderr, "failed to tune\n");
		goto err;
	}

	// init 7
	fprintf(stderr, "Running init 7...\n");
#if 1 // XXX 0
	if (!tvw_tune_tvw(dev)) {
		warnx("failed to tune tvw 1/3");
		goto err;
	}
	if (!tvw_tune_tvw(dev)) {
		warnx("failed to tune tvw 2/3");
		goto err;
	}
#endif

#if 1
	WR(dev, TVW_ADEC_RAMPING_TARGET_VOLUME, 0x00000080) &&
	MR(dev, TVW_VBW_SETTINGS, ALL, 0x00100000) &&
	WR(dev, TVW_SMGR_SOFT_RESETS, 0x0000002f) &&
	WR(dev, TVW_SMGR_SOFT_RESETS, 0) &&
	WR(dev, TVW_FEVW_HEIGHT, 0x00000120) &&
	MR(dev, TVW_FEVW_VID_START_LINE, ALL, 0x00140014) &&
	WR(dev, TVW_FEVW_VBI_HEIGHT, 0) &&
	true;
#endif

	if (!tvw_tune_tvw(dev)) {
		warnx("failed to tune tvw 3/3");
		goto err;
	}

	WR(dev, TVW_DRS_CONTROL, 0x00000012) &&
	MR(dev, TVW_SMGR_BIST_SEL, ALL, 0x00000001) &&
	MR(dev, TVW_VBIST_RESET, 0xfffffffe, 0) &&
	MR(dev, TVW_RESAMP0_CLAMP_CONTROL, 0xfffffffe, 0x00000010) &&
	MR(dev, TVW_DRS_CONTROL, ALL, 0x00168000) &&
	WR(dev, TVW_CLOCK_CONTROL_0, 0x00dfda37) &&
	WR(dev, TVW_CLOCK_CONTROL_1, 0x50070155) &&
	WR(dev, 0x08a4, 0x0203f03f) &&
	WU(dev, 0xf00a, 0x50) &&
	WU(dev, 0xf009, 0x9c) &&
	WU(dev, 0xf008, 0x77) &&
	WU(dev, 0xf05d, 0xff) &&

	// XXX these writes get audio working! XXX
	// look at FEAW specifically, FEVW alone didn't get audio
	WR(dev, TVW_TBC_CBCR_DELAY, 0x00000000) &&
	WR(dev, TVW_TBC_OFFSET_SKEW, 0x00000000) &&
	WR(dev, TVW_TBC_PAUSE, 0xf8000000) &&
	WR(dev, TVW_TBC_PLL_OVER_GAIN, 0x00007d78) &&
	WR(dev, TVW_TBC_LINE_BUF_FREQ_DIF, 0x00000000) &&
	WR(dev, TVW_TBC_LINE_BUF_READ_DISTANCE, 0x00000000) &&
	XR(dev, TVW_VBW_SETTINGS, ALL, 0x00100000) && // unknown values
	MR(dev, TVW_FEVW_WIDTH, ALL, 0) && // unknown values ... 0x02D0 == 720
	MR(dev, TVW_FEVW_VBI_HEIGHT, ALL, 0x00120012) && // 18, 18
	MR(dev, TVW_FEVW_VBI_START_LINE, ALL, 0x00010001) && // 1, 1
	XR(dev, TVW_RVR_ENGINE_STATUS, ALL, 0x01000000) && // unknown values
	XR(dev, TVW_VBW_LUM_DDT_FLAGS, ALL, 0) && // unknown values
	XR(dev, TVW_VBW_VBI_DDT_FLAGS, ALL, 0) && // unknown values
	WR(dev, TVW_RVR_ADV_ON_OVF, 0x00000002) &&
	// copy FEVW values to RVR?
	WR(dev, TVW_RVR_WIDTH, 0x02d002d0) &&
	WR(dev, TVW_RVR_HEIGHT, 0x00000120) && // 288
	WR(dev, TVW_RVR_WIDTH, 0x02d002d0) &&
	WR(dev, TVW_RVR_VBI_HEIGHT, 0x00120012) && // 18, 18
	XR(dev, TVW_FW_VMOW_STATUS, ALL, 0x00000000) && // unknown values
	XR(dev, TVW_FW_VMW_STATUS, ALL, 0x00000000) && // unknown values

	WR(dev, TVW_FEVW_COMMAND, 0x03000000) &&
	XR(dev, TVW_FEVW_ENGINE_STATUS, ALL, 0x01000000) && // unknown values
	WR(dev, TVW_FEVW_COMMAND, 0x00000000) &&

	XR(dev, TVW_VBW_SETTINGS, ALL, 0x00100000) && // unknown values
	MR(dev, TVW_RAR_SETTINGS, ALL, 0x00000080) &&
	//MR(dev, TVW_USC_CTL, ALL, 0) && // unknown values
	WR(dev, TVW_USC_ANC_WIDTH, 0x00100000) && // 16, 0
	MR(dev, TVW_USC_DEBUG_CTL, ALL, 0x40000000) &&
	MR(dev, TVW_USC_CTL, ALL, 0x10000000) &&
	XR(dev, TVW_RAR_ENGINE_STATUS, ALL, 0x01000000) && // unknown values
	XR(dev, TVW_FEAW_CYCLE_CNT, ALL, 0x00000000) && // unknown values
	XR(dev, TVW_FEAW_WRITE_PTR, ALL, 0x00000000) && // unknown values
	XR(dev, TVW_FEAW_CYCLE_CNT, ALL, 0x00000000) && // unknown values
	WR(dev, TVW_FW_AMOW_STATUS, 0x00000000) &&
	WR(dev, TVW_FW_AMW_STATUS, 0x00000000) &&
	WR(dev, TVW_RAR_COMMAND, 0x00000000) &&

	WR(dev, TVW_FEAW_COMMAND, 0x03000000) &&
	XR(dev, TVW_FEAW_ENGINE_STATUS, ALL, 0x01000000) && // unknown values
	WR(dev, TVW_FEAW_COMMAND, 0x00000000) &&

	true;

	*out_dev = dev;

	return 0;

err:
	if (dev->ctx) {
		libusb_exit(dev->ctx);
	}

	free(dev);

	return -1;
}

int
tvwsdr_close(tvwsdr_dev_t *dev) {
	if (NULL == dev) {
		return -1;
	}

	// XXX wait on async ops?

	libusb_release_interface(dev->devh, 0);
	libusb_close(dev->devh);

	libusb_exit(dev->ctx);

	free(dev);

	return 0;
}

void
tvwsdr_deframe_isoch_data(tvwsdr_dev_t *dev, unsigned char *data, int len) {
	unsigned char *ptr;
	unsigned int frame_length, tmpoffset = 0;

	/* safety check */
	if (dev->work_buflen + len > sizeof(dev->work_buf)) {
		dev->work_buflen = 0;
	}

	/* append the new data to any existing data */
	memcpy(dev->work_buf + dev->work_buflen, data, len);
	dev->work_buflen += len;

	do {
		/* look for the start of boundary mark: 0xff */
		ptr = memchr(dev->work_buf + tmpoffset, '\xff', dev->work_buflen - tmpoffset);
		if (NULL == ptr) {
			break;
		}

		/* how much data do we have before the next boundary section? */
		frame_length = ptr - (dev->work_buf + tmpoffset);
		/* boundary sections are 32 bytes long */
		if (frame_length + 32 > dev->work_buflen - tmpoffset) {
			break;
		}

		dev->cb(ptr, frame_length, dev->cb_ctx);

#if 0
		/* print header (footer?) */
		fprintf(stderr, "%4i: ", frame_length);
		for(j = 0; j < 32; j++) {
			fprintf(stderr, "%02x ", *(ptr + j));
		}
		fprintf(stderr, "\n");
#endif

		tmpoffset += frame_length + 32;
	} while(1);

	/* move data around */
	dev->work_buflen -= tmpoffset;
	memmove(dev->work_buf, dev->work_buf + tmpoffset, dev->work_buflen);
}

static void LIBUSB_CALL
tvwsdr_xfer_cb(struct libusb_transfer *xfer) {
	int actual_length, i;
	tvwsdr_dev_t *dev = (tvwsdr_dev_t *)xfer->user_data;
	unsigned char *pkt;

	if (xfer->status != LIBUSB_TRANSFER_COMPLETED) {
		return;
	}

	if (dev->cb) {
		for(i = 0; i < NUM_ISOCH_PACKETS; i++) {
			pkt = libusb_get_iso_packet_buffer_simple(xfer, i);
			actual_length = xfer->iso_packet_desc[i].actual_length;

			if (actual_length > 0) {
				tvwsdr_deframe_isoch_data(dev, pkt, actual_length);
			}
		}
	}

	libusb_submit_transfer(xfer);
}

int
tvwsdr_read_async(tvwsdr_dev_t *dev, tvwsdr_read_async_cb_t cb, void *ctx) {
	int i;

	if (NULL == dev) {
		return -1;
	}

	dev->cb = cb;
	dev->cb_ctx = ctx;

	if (NULL == dev->xfer) {
		dev->xfer = malloc(NUM_ISOCH_XFERS * sizeof(struct libusb_transfer *));

		for(i = 0; i < NUM_ISOCH_XFERS; i++) {
			dev->xfer[i] = libusb_alloc_transfer(NUM_ISOCH_PACKETS);
		}
	}

	if (NULL == dev->xfer_buf) {
		dev->xfer_buf = malloc(NUM_ISOCH_XFERS * sizeof(unsigned char *));

		for(i = 0; i < NUM_ISOCH_XFERS; i++) {
			dev->xfer_buf[i] = malloc(3*1024 * NUM_ISOCH_PACKETS);
		}
	}

	for(i = 0; i < NUM_ISOCH_XFERS; i++) {
		libusb_fill_iso_transfer(
				dev->xfer[i],
				dev->devh,
				0x81,
				dev->xfer_buf[i],
				3*1024 * NUM_ISOCH_PACKETS,
				NUM_ISOCH_PACKETS,
				tvwsdr_xfer_cb,
				(void *)dev,
				1000);
		libusb_set_iso_packet_lengths(dev->xfer[i], 3*1024);
		libusb_submit_transfer(dev->xfer[i]);
	}

	while (libusb_handle_events(dev->ctx) >= 0) {
		;
	}

	return 0;
}

int
tvwsdr_cancel_async(tvwsdr_dev_t *dev) {
	/* FIXME */
	return 0;
}
