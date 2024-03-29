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

#ifndef INCLUDED_GNSS_LOOP_FILTER_H
#define INCLUDED_GNSS_LOOP_FILTER_H

#include <cmath>
#include "iir_filter.h"

namespace gr {
  namespace gnss {

    // The general idea for the filter is taken from
    // Understanding the GPS: Principles and Applications by
    // Elliott D. Kaplan and Christopher J. Hegarty.
    // Table 5.6 gives values for the coefficients.
    // Please see also attached pdfs.
    template<std::size_t ORDER>
    class loop_filter : public iir_filter<double, ORDER, ORDER + 1>
    {
      static_assert(ORDER == 1 || ORDER == 2,
        "Only 1st and 2nd ORDER loop filters are implemented");

    public:
      loop_filter(double sampling_interval) :
        iir_filter<double, ORDER, ORDER + 1>{},
        d_sampling_interval{sampling_interval}
      {
      }

      void reset(double initial_output, double noise_bandwidth)
      {
        iir_filter<double, ORDER, ORDER + 1>::reset(initial_output);
        update_coefficients(noise_bandwidth);
      }

      void update_coefficients(double noise_bandwidth)
      {
        const double T = d_sampling_interval;

        if constexpr (ORDER == 1) {
          const double zeta = 1.0 / std::sqrt(2.0); /* 0.707 */
          const double lambda = (8.0 * zeta) / (1.0 + 4 * zeta * zeta);
          const double wn = noise_bandwidth * lambda;
          const double g[] = {2 * zeta * wn, wn * wn};

          iir_filter<double, ORDER, ORDER + 1>::d_b[0] = +g[0] + g[1] * T / 2.0;
          iir_filter<double, ORDER, ORDER + 1>::d_b[1] = -g[0] + g[1] * T / 2.0;

          iir_filter<double, ORDER, ORDER + 1>::d_a[0] = -1.0;
        }
        else
        if constexpr (ORDER == 2) {
          const double zeta = 1.1;
          const double xi = 2.4;
          const double lambda = (4.0 * (zeta * xi - 1.0)) / (zeta * xi * xi + zeta * zeta - xi);
          const double wn = noise_bandwidth * lambda;
          const double g[] = {xi * wn, zeta * wn * wn, wn * wn * wn};

          iir_filter<double, ORDER, ORDER + 1>::d_b[0] = +1.0 * g[0] + T / 2.0 * (g[1] + g[2] * T / 2.0);
          iir_filter<double, ORDER, ORDER + 1>::d_b[1] = -2.0 * g[0] + g[2] * T * T / 2.0;
          iir_filter<double, ORDER, ORDER + 1>::d_b[2] = +1.0 * g[0] + T / 2.0 * (-g[1] + g[2] * T / 2.0);

          iir_filter<double, ORDER, ORDER + 1>::d_a[0] = -2.0;
          iir_filter<double, ORDER, ORDER + 1>::d_a[1] = 1.0;
        }
        else {
          /* do nothing */
        }
      }

      double d_sampling_interval;
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_LOOP_FILTER_H */

