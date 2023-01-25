/* -*- c++ -*- */
/*
 * Copyright 2021 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_GNSS_GPS_PARAMETERS_H
#define INCLUDED_GNSS_GPS_PARAMETERS_H

#include <cstdint>
#include <bitset>

namespace gr {
  namespace gnss {

    constexpr uint32_t GPS_F0 = 10230000;
    constexpr uint32_t GPS_L1_MULTIPLIER = 154;
    constexpr uint32_t GPS_L2_MULTIPLIER = 120;

    // IS-GPS-200L - Chapter 3.3.1.1 Frequency Plan
    // The nominal carrier frequencies shall be 1575.42 MHz,
    // and 1227.6 MHz for L1 and L2, respectively.
    constexpr uint32_t GPS_L1_FREQ_HZ = GPS_L1_MULTIPLIER * GPS_F0;
    constexpr uint32_t GPS_L2_FREQ_HZ = GPS_L2_MULTIPLIER * GPS_F0;

    constexpr int GPS_CA_CODE_LENGTH = 1023;
    constexpr int GPS_CA_CODE_RATE = 1000; // codes per second
    constexpr int GPS_CA_CODE_CHIP_RATE = GPS_CA_CODE_LENGTH * GPS_CA_CODE_RATE; // chips per second

    constexpr int GPS_CA_CODES_PER_NAV_MESSAGE_BIT = 20;

    // IS-GPS-200L - Chapter 20.3.2 Message Structure.
    // IS-GPS-200L - Chapter 20.3.3 Message Content.
    constexpr int GPS_NAV_MESSAGE_BITS_PER_WORD = 30;
    constexpr int GPS_NAV_MESSAGE_WORDS_PER_SUBFRAME = 10;
    constexpr int GPS_NAV_MESSAGE_BITS_PER_SUBFRAME =
      GPS_NAV_MESSAGE_BITS_PER_WORD * GPS_NAV_MESSAGE_WORDS_PER_SUBFRAME;

    constexpr std::bitset<8> GPS_CA_TLM_PREAMBLE_BITS = 0b11010001;

    constexpr double GPS_SCALE_FACTOR_T_GD = (1.0 / (1LL << 31));
    constexpr double GPS_SCALE_FACTOR_A_F0 = (1.0 / (1LL << 31));
    constexpr double GPS_SCALE_FACTOR_A_F1 = (1.0 / (1LL << 43));
    constexpr double GPS_SCALE_FACTOR_A_F2 = (1.0 / (1LL << 55));

    constexpr double GPS_SCALE_FACTOR_C_U = (1.0 / (1LL << 29));
    constexpr double GPS_SCALE_FACTOR_C_I = (1.0 / (1LL << 29));
    constexpr double GPS_SCALE_FACTOR_C_R = (1.0 / (1LL << 5));

    constexpr int GPS_SCALE_FACTOR_T_OC = (1 << 4);
    constexpr int GPS_SCALE_FACTOR_T_OE = (1 << 4);

    constexpr double GPS_SCALE_FACTOR_E          = (1.0 / (1LL << 33));
    constexpr double GPS_SCALE_FACTOR_SQRT_A     = (1.0 / (1LL << 19));
    constexpr double GPS_SCALE_FACTOR_M_0        = (1.0 / (1LL << 31));
    constexpr double GPS_SCALE_FACTOR_DELTA_N    = (1.0 / (1LL << 43));
    constexpr double GPS_SCALE_FACTOR_OMEGA_0    = (1.0 / (1LL << 31));
    constexpr double GPS_SCALE_FACTOR_I_0        = (1.0 / (1LL << 31));
    constexpr double GPS_SCALE_FACTOR_OMEGA      = (1.0 / (1LL << 31));
    constexpr double GPS_SCALE_FACTOR_D_OMEGA_DT = (1.0 / (1LL << 43));
    constexpr double GPS_SCALE_FACTOR_IDOT       = (1.0 / (1LL << 43));

    // Number of seconds in GPS week
    constexpr int GPS_SECONDS_PER_WEEK = 60 * 60 * 24 * 7;

    // Relativistic correction term constant, [s/(m)^(1/2)],
    // IS-GPS-200L, chapter 20.3.3.3.3.1 "User Algorithm fo SV Clock Correction"
    constexpr double GPS_F = -4.442807633e-10;

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_GPS_PARAMETERS_H */

