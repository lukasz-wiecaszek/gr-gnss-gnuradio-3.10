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

#ifndef INCLUDED_GNSS_PRBS_H
#define INCLUDED_GNSS_PRBS_H

#include <array>
#include <memory>
#include <algorithm>
#include <type_traits>

namespace gr {
  namespace gnss {

    template<typename T, unsigned... POLYNOMIAL>
    class prbs
    {
      static_assert(std::is_integral<T>::value, "T shall be an integral type");

    private:
      static constexpr std::array<unsigned, sizeof...(POLYNOMIAL)> polynomial{POLYNOMIAL...};

      static constexpr unsigned polynomial_degree()
      {
        unsigned degree = 0;

        for(std::size_t i = 0; i < sizeof...(POLYNOMIAL); ++i)
          if(polynomial[i] > degree)
            degree = polynomial[i];

        return degree;
      }

      int d_length;
      std::array<T, polynomial_degree()> d_FSR; // feedback shift register
      std::unique_ptr<T[]> d_PRBS;

    public:
      prbs(int length) :
         d_length{length},
         d_FSR{},
         d_PRBS{new T[length]}
      {
        d_FSR.fill(1); // initial state

        for (int i = 0; i < length; ++i) {
          T x = XOR();
          std::rotate(d_FSR.rbegin(), d_FSR.rbegin() + 1, d_FSR.rend());
          d_PRBS[i] = d_FSR[0];
          d_FSR[0] = x;
        }
      }

      T operator[](int x) const
      {
        return d_PRBS[x];
      }

      int length() const
      {
        return d_length;
      }

    private:
      T XOR() const
      {
        T x = 0;
        for (unsigned i : polynomial)
          x ^= d_FSR[i - 1];
        return x;
      }
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_PRBS_H */

