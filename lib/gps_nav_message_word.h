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

#ifndef INCLUDED_GNSS_GPS_NAV_MESSAGE_WORD_H
#define INCLUDED_GNSS_GPS_NAV_MESSAGE_WORD_H

#include <bitset>
#include "gnss_parameters.h"

namespace gr {
  namespace gnss {

    constexpr std::bitset<30> MASK_PIRITY_BITS = 0b111111;

    class gps_nav_message_word
    {
    public:
      template<typename T>
      bool init(const T** vptr, T D29, T D30)
      {
        const T* values = *vptr;

#define D(i) word.size() - i

        if (D30 == 0)
          for (std::size_t i = 1; i <= 24; ++i)
            word.set(D(i), !!values[i - 1]);
        else
          for (std::size_t i = 1; i <= 24; ++i)
            word.set(D(i), !values[i - 1]);

        // IS-GPS-200L - Chapter 20.3.5.2 "User Parity Algorithm"
        word.set(D(25), (!!values[25 - 1] + D29 + (word & GPS_PARITY_ALGORITHM_MASK_D25).count()) % 2);
        word.set(D(26), (!!values[26 - 1] + D30 + (word & GPS_PARITY_ALGORITHM_MASK_D26).count()) % 2);
        word.set(D(27), (!!values[27 - 1] + D29 + (word & GPS_PARITY_ALGORITHM_MASK_D27).count()) % 2);
        word.set(D(28), (!!values[28 - 1] + D30 + (word & GPS_PARITY_ALGORITHM_MASK_D28).count()) % 2);
        word.set(D(29), (!!values[29 - 1] + D30 + (word & GPS_PARITY_ALGORITHM_MASK_D29).count()) % 2);
        word.set(D(30), (!!values[30 - 1] + D29 + (word & GPS_PARITY_ALGORITHM_MASK_D30).count()) % 2);

#undef D

        *vptr = values + word.size();

        return ((word & MASK_PIRITY_BITS) == 0);
      }

      template<std::size_t BIT>
      bool flag() const
      {
        return word[word.size() - BIT];
      }

      template<std::size_t BIT, std::size_t BITS>
      unsigned long field() const
      {
        return (word.to_ulong() >> (word.size() - (BIT + BITS - 1))) & ((1UL << BITS) - 1);
      }

      std::string to_string() const
      {
        return word.to_string();
      }

    private:
      std::bitset<GPS_NAV_MESSAGE_BITS_PER_WORD> word;
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_GPS_NAV_MESSAGE_WORD_H */

