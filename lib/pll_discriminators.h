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

#ifndef INCLUDED_GNSS_PLL_DISCRIMINATORS_H
#define INCLUDED_GNSS_PLL_DISCRIMINATORS_H

#include <gnuradio/gr_complex.h>
#include <cmath>

namespace gr {
  namespace gnss {

    // The general idea for the discriminators is taken from
    // Understanding the GPS: Principles and Applications by
    // Elliott D. Kaplan and Christopher J. Hegarty.
    // Table 5.2 PLL discriminators and table 5.3 Common Costas Loop Discriminators.

    constexpr double pll_discriminator_classic_costas_analog(gr_complexd c)
    {
      return c.imag() * c.real();
    }

    constexpr double pll_discriminator_four_quadrant_arctangent(gr_complexd c)
    {
      return std::atan2(c.imag(), c.real());
    }

    constexpr double pll_discriminator_two_quadrant_arctangent(gr_complexd c)
    {
      return c.real() != 0 ? std::atan(c.imag() / c.real()) : 0;
    }

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_PLL_DISCRIMINATORS_H */

