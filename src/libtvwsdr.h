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

#ifndef LIBTVWSDR_H
#define LIBTVWSDR_H

enum tda18271_regs {
	TDA18271_ID = 0x00,
	TDA18271_TM,
	TDA18271_PL,
	TDA18271_EP1,
	TDA18271_EP2,
	TDA18271_EP3,
	TDA18271_EP4,
	TDA18271_EP5,
	TDA18271_CPD,
	TDA18271_CD1,
	TDA18271_CD2,
	TDA18271_CD3,
	TDA18271_MPD,
	TDA18271_MD1,
	TDA18271_MD2,
	TDA18271_MD3,
	TDA18271_EB1,
	TDA18271_EB2,
	TDA18271_EB3,
	TDA18271_EB4,
	TDA18271_EB5,
	TDA18271_EB6,
	TDA18271_EB7,
	TDA18271_EB8,
	TDA18271_EB9,
	TDA18271_EB10,
	TDA18271_EB11,
	TDA18271_EB12,
	TDA18271_EB13,
	TDA18271_EB14,
	TDA18271_EB15,
	TDA18271_EB16,
	TDA18271_EB17,
	TDA18271_EB18,
	TDA18271_EB19,
	TDA18271_EB20,
	TDA18271_EB21,
	TDA18271_EB22,
	TDA18271_EB23,

	TDA18271_NUM_REGS
};

#endif
