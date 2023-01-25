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

#ifndef INCLUDED_GNSS_DLL_DISCRIMINATORS_H
#define INCLUDED_GNSS_DLL_DISCRIMINATORS_H

#include <gnuradio/gr_complex.h>
#include <cmath>

namespace gr {
  namespace gnss {

    // The general idea for the discriminators is taken from
    // Understanding the GPS: Principles and Applications by
    // Elliott D. Kaplan and Christopher J. Hegarty.
    // Table 5.5 Common Delay Lock Loop Discriminators.

    inline double dll_discriminator_noncoherent_e_minus_l_envelope(gr_complexd e, gr_complexd l)
    {
      double abs_e = std::abs(e);
      double abs_l = std::abs(l);
      double abs_e_plus_l = abs_e + abs_l;

      return abs_e_plus_l != 0 ? (abs_e - abs_l) / abs_e_plus_l : 0;
    }

    inline double dll_discriminator_noncoherent_e_minus_l_power(gr_complexd e, gr_complexd l)
    {
      double norm_e = std::norm(e);
      double norm_l = std::norm(l);
      double norm_e_plus_l = norm_e + norm_l;

      return norm_e_plus_l != 0 ? (norm_e - norm_l) / norm_e_plus_l : 0;
    }

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_DLL_DISCRIMINATORS_H */

