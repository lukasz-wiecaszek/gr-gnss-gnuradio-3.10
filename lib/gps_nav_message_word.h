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

    class gps_nav_message_word
    {
    public:
      template<typename T>
      void init(const T** vptr, T D30)
      {
        const T* values = *vptr;

        if (D30 == 0)
          for (std::size_t i = 0; i < word.size(); ++i)
            word.set(word.size() - 1 - i, !!values[i]);
        else
          for (std::size_t i = 0; i < word.size(); ++i)
            word.set(word.size() - 1 - i, !values[i]);

        *vptr = values + word.size();
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

