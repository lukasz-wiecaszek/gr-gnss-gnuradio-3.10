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

#ifndef INCLUDED_GNSS_IIR_FILTER_H
#define INCLUDED_GNSS_IIR_FILTER_H

#include <algorithm>
#include <array>
#include "ilog2.hpp"

namespace gr {
  namespace gnss {

    template<typename T, std::size_t N, std::size_t M>
    class iir_filter
    {
      static constexpr std::size_t H = 1UL << lts::ilog2_roundup(std::max(N, M));

    public:
      iir_filter() :
        d_a{},
        d_b{},
        d_i{},
        d_o{},
        d_current_index{}
      {
        reset(0.0);
      }

      void reset(T initial_output)
      {
        d_i.fill(0);
        d_o.fill(initial_output);
        d_current_index = 0;
      }

      T operator[](T x)
      {
        T result = 0.0;

        // add output items first: o(n) = -[a(0)*o(n-1) + a(1)*o(n-2) + ... + a(N-1)*o(n-N)]
        for (std::size_t i = 0; i < d_a.size(); ++i)
          result -= d_a[i] * d_o[(d_current_index + i) & (H - 1)];

        // decrement (or wrap around) the current index
        // that way the old samples are available at possitive offsets in regads to d_current_index
        // for example:
        // i(n-1) = d_i[(d_current_index + 1) % H]
        // i(n-2) = d_i[(d_current_index + 2) % H]
        d_current_index = (d_current_index > 0) ? d_current_index - 1 : H - 1;

        d_i[d_current_index] = x;

        // now add input items: o(n) = b(0)*i(n) + b(1)*i(n-1) + b(2)*i(n-2) + ... + b(M-1)*i(n-M)
        for (std::size_t i = 0; i < d_b.size(); ++i)
          result += d_b[i] * d_i[(d_current_index + i) & (H - 1)];

        d_o[d_current_index] = result;

        return result;
      }

    protected:
      std::array<T, N> d_a; // output coefficients
      std::array<T, M> d_b; // input coefficients

    private:
      std::array<T, H> d_i; // previous input values
      std::array<T, H> d_o; // previous output values
      int d_current_index;  // current index in the d_i and d_o arrays
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_IIR_FILTER_H */

