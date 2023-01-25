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

#ifndef INCLUDED_GNSS_DOPPLER_SHIFTS_H
#define INCLUDED_GNSS_DOPPLER_SHIFTS_H

#include <gnuradio/fxpt_nco.h>
#include <gnuradio/math.h>

#include <array>
#include <vector>

namespace gr {
  namespace gnss {

    template<int N>
    class doppler_shifts
    {
    public:
      doppler_shifts(double sampling_freq, double f0, double df, int n_samples) :
        d_sampling_freq{sampling_freq},
        d_f0{f0},
        d_df{df},
        d_shifts{}
      {
        for (std::size_t n = 0; n < d_shifts.size(); ++n) {
          d_shifts[n] = std::vector<gr_complex>(n_samples);
          init(d_shifts[n], n_to_freq(n));
        }
      }

      int bins() const
      {
        return N;
      }

      double n_to_freq(int n)
      {
        return d_f0 + n * d_df;
      }

      const std::vector<gr_complex>& operator[](int n) const
      {
        return d_shifts[n];
      }

    private:
      void init(std::vector<gr_complex>& v, double f)
      {
        gr::fxpt_nco nco;

        nco.set_freq(-2.0 * GR_M_PI * f / d_sampling_freq);
        nco.sincos(v.data(), v.size(), 1);
      }

      double d_sampling_freq;
      double d_f0;
      double d_df;
      std::array<std::vector<gr_complex>, N> d_shifts;
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_DOPPLER_SHIFTS_H */

