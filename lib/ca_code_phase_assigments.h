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

#ifndef INCLUDED_GNSS_CA_CODE_PHASE_ASSIGMENTS_H
#define INCLUDED_GNSS_CA_CODE_PHASE_ASSIGMENTS_H

#include <cstdint>

namespace gr {
  namespace gnss {

  struct ca_code_phase_ids
  {
    uint32_t svid;
    uint32_t prn;
    uint32_t delay;
  };

  // IS-GPS-200 - Table 3-Ia - Code Phase Assigments
  constexpr ca_code_phase_ids ca_code_phase_assigments[] =
  {
    { 0,  0,   0},
    { 1,  1,   5},
    { 2,  2,   6},
    { 3,  3,   7},
    { 4,  4,   8},
    { 5,  5,  17},
    { 6,  6,  18},
    { 7,  7, 139},
    { 8,  8, 140},
    { 9,  9, 141},
    {10, 10, 251},
    {11, 11, 252},
    {12, 12, 254},
    {13, 13, 255},
    {14, 14, 256},
    {15, 15, 257},
    {16, 16, 258},
    {17, 17, 469},
    {18, 18, 470},
    {19, 19, 471},
    {20, 20, 472},
    {21, 21, 473},
    {22, 22, 474},
    {23, 23, 509},
    {24, 24, 512},
    {25, 25, 513},
    {26, 26, 514},
    {27, 27, 515},
    {28, 28, 516},
    {29, 29, 859},
    {30, 30, 860},
    {31, 31, 861},
    {32, 32, 862},
  };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_CA_CODE_PHASE_ASSIGMENTS_H */

