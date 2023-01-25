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

#ifndef INCLUDED_GNSS_PLL_LOOP_FILTER_H
#define INCLUDED_GNSS_PLL_LOOP_FILTER_H

#include <cmath>

namespace gr {
  namespace gnss {

    // The general idea for the filter is taken from
    // Understanding the GPS: Principles and Applications by
    // Elliott D. Kaplan and Christopher J. Hegarty.
    // Table 5.6 gives values for the coefficients.
    // Please see also attached pdfs.
    template<std::size_t ORDER>
    class pll_loop_filter
    {
      static_assert(ORDER == 1 || ORDER == 2,
        "Only 1st and 2nd ORDER loop filters are implemented");
    public:
      pll_loop_filter(double sampling_interval) :
        d_sampling_interval{sampling_interval},
        d_accumulators{},
        d_g{}
      {
      }

      void reset(double initial_output, double pll_bandwidth)
      {
        if constexpr (ORDER == 1) {
          d_accumulators[0] = initial_output; // velocity accumulator
        }
        else
        if constexpr (ORDER == 2) {
          d_accumulators[0] = 2.0 * initial_output; // velocity accumulator
          d_accumulators[1] = 0.0; // acceleration accumulator
        }
        else {
          /* do nothing */
        }

        update_coefficients(pll_bandwidth);
      }

      void update_coefficients(double pll_bandwidth)
      {
        if constexpr (ORDER == 1) {
          const double alpha = std::sqrt(2.0);
          const double lambda = (4.0 * alpha) / (1.0 + alpha * alpha);
          const double w0 = pll_bandwidth * lambda;

          d_g[0] = alpha * w0;
          d_g[1] = w0 * w0;
        }
        else
        if constexpr (ORDER == 2) {
          const double alpha = 2.4;
          const double beta = 1.1;
          const double lambda = (4.0 * (alpha * beta - 1)) / (alpha * alpha * beta + beta * beta - alpha);
          const double w0 = pll_bandwidth * lambda;

          d_g[0] = alpha * w0;
          d_g[1] = beta * w0 * w0;
          d_g[2] = w0 * w0 * w0;
        }
        else {
          /* do nothing */
        }
      }

      double operator[](double x)
      {
        double result;
        const double T = d_sampling_interval;

        if constexpr (ORDER == 1) {
          d_accumulators[0] += (d_g[1] * T) * x;
          result = d_accumulators[0] + (d_g[0] - d_g[1] * T / 2.0) * x;
        }
        else
        if constexpr (ORDER == 2) {
          d_accumulators[1] += (d_g[2] * T) * x;
          d_accumulators[0] += d_accumulators[1] * T / 2.0 + (d_g[1] * T) * x;
          result = d_accumulators[0] / 2.0 + d_g[0] * x;
        }
        else {
          /* do nothing */
        }

        return result;
      }

    private:
      double d_sampling_interval;
      double d_accumulators[ORDER];
      double d_g[ORDER + 1];
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_DLL_LOOP_FILTER_H */

