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

#ifndef INCLUDED_GNSS_PLL_FLL_LOOP_FILTER_H
#define INCLUDED_GNSS_PLL_FLL_LOOP_FILTER_H

#include <cmath>

namespace gr {
  namespace gnss {

    // The general idea for the filter is taken from
    // Understanding the GPS: Principles and Applications by
    // Elliott D. Kaplan and Christopher J. Hegarty.
    // Table 5.6 gives values for the coefficients.
    // Please see also attached pdfs.
    template<std::size_t ORDER>
    class pll_fll_loop_filter
    {
      static_assert(ORDER == 1 || ORDER == 2,
        "Only 1st and 2nd ORDER loop filters are implemented");
    public:
      pll_fll_loop_filter(double sampling_interval) :
        d_sampling_interval{sampling_interval},
        d_pll_accumulators{},
        d_pll_g{},
        d_fll_g{}
      {
      }

      void reset(double initial_output, double pll_bandwidth, double fll_bandwidth)
      {
        if constexpr (ORDER == 1) {
          d_pll_accumulators[0] = initial_output; // velocity accumulator
        }
        else
        if constexpr (ORDER == 2) {
          d_pll_accumulators[0] = 2.0 * initial_output; // velocity accumulator
          d_pll_accumulators[1] = 0.0; // acceleration accumulator
        }
        else {
          /* do nothing */
        }

        update_coefficients(pll_bandwidth, fll_bandwidth);
      }

      void update_coefficients(double pll_bandwidth, double fll_bandwidth)
      {
        if constexpr (ORDER == 1) {
          /* PLL */
          const double pll_zeta = 1.0 / std::sqrt(2.0); /* 0.707 */
          const double pll_lambda = (8.0 * pll_zeta) / (1.0 + 4 * pll_zeta * pll_zeta); /* 1.886 */
          const double pll_wn = pll_bandwidth * pll_lambda;
          d_pll_g[0] = 2 * pll_zeta * pll_wn;
          d_pll_g[1] = pll_wn * pll_wn;

          /* FLL */
          const double fll_wn = fll_bandwidth * 4;
          d_fll_g[0] = fll_wn;
        }
        else
        if constexpr (ORDER == 2) {
          /* PLL */
          const double pll_xi = 2.4;
          const double pll_zeta = 1.1;
          const double pll_lambda =
            (4.0 * (pll_zeta * pll_xi - 1.0)) /
            (pll_zeta * pll_xi * pll_xi + pll_zeta * pll_zeta - pll_xi); /* 1.274 */
          const double pll_wn = pll_bandwidth * pll_lambda;
          d_pll_g[0] = pll_xi * pll_wn;
          d_pll_g[1] = pll_zeta * pll_wn * pll_wn;
          d_pll_g[2] = pll_wn * pll_wn * pll_wn;

          /* FLL */
          const double fll_zeta = 1.0 / std::sqrt(2.0); /* 0.707 */
          const double fll_lambda = (8.0 * pll_zeta) / (1.0 + 4 * pll_zeta * pll_zeta); /* 1.886 */
          const double fll_wn = fll_bandwidth * fll_lambda;
          d_fll_g[0] = 2 * fll_zeta * fll_wn;
          d_fll_g[1] = fll_wn * fll_wn;
        }
        else {
          /* do nothing */
        }
      }

      double operator () (double pll_discriminator, double fll_discriminator)
      {
        double result;
        const double T = d_sampling_interval;

        if constexpr (ORDER == 1) {
          d_pll_accumulators[0] += (d_pll_g[1] * T) * pll_discriminator + (d_fll_g[0] * T) * fll_discriminator;
          result = d_pll_accumulators[0] + (d_pll_g[0] - d_pll_g[1] * T / 2.0) * pll_discriminator;
        }
        else
        if constexpr (ORDER == 2) {
          d_pll_accumulators[1] += (d_pll_g[2] * T) * pll_discriminator + (d_fll_g[1] * T) * fll_discriminator;
          d_pll_accumulators[0] += d_pll_accumulators[1] * T / 2.0 + (d_pll_g[1] * T) * pll_discriminator + (d_fll_g[0] * T) * fll_discriminator;;
          result = d_pll_accumulators[0] / 2.0 + d_pll_g[0] * pll_discriminator;
        }
        else {
          /* do nothing */
        }

        return result;
      }

    private:
      double d_sampling_interval;
      double d_pll_accumulators[ORDER];
      double d_pll_g[ORDER + 1];
      double d_fll_g[ORDER];
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_PLL_FLL_LOOP_FILTER_H */

