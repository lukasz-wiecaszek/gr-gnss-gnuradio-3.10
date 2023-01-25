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

#ifndef INCLUDED_GNSS_GPS_NAV_MESSAGE_SUBFRAME_HEADER_H
#define INCLUDED_GNSS_GPS_NAV_MESSAGE_SUBFRAME_HEADER_H

#include <array>
#include <string>
#include "gps_nav_message_word.h"

namespace gr {
  namespace gnss {

    class gps_nav_message_subframe_header
    {
    public:
      gps_nav_message_subframe_header() :
        is_initialized{false}
      {
      }

      unsigned tlm_message()       const { return field1unsigned<9, 14>();  }
      bool integrity_status_flag() const { return flag<23>();               }

      unsigned tow_count_message() const { return field1unsigned<31, 17>(); }
      bool alert_flag()            const { return flag<48>();               }
      bool anti_spoof_flag()       const { return flag<49>();               }
      unsigned subframe_id()       const { return field1unsigned<50, 3>();  }

      operator bool () const
      {
        return is_initialized;
      }

      std::string to_string() const
      {
        char strbuf[256];

        snprintf(strbuf, sizeof(strbuf),
          "subframe id: %d, tow: %d",
            subframe_id(), tow_count_message());

        return std::string(strbuf);
      }

      operator std::string () const
      {
        return to_string();
      }

    protected:
      template<std::size_t BIT>
      bool flag() const
      {
        const gps_nav_message_word& word = words[(BIT - 1) / GPS_NAV_MESSAGE_BITS_PER_WORD];
        return word.flag<((BIT - 1) % GPS_NAV_MESSAGE_BITS_PER_WORD) + 1>();
      }

      template<std::size_t BIT1, std::size_t BITS1>
      unsigned long field1unsigned() const
      {
        const gps_nav_message_word& word1 = words[(BIT1 - 1) / GPS_NAV_MESSAGE_BITS_PER_WORD];
        unsigned long field1 = word1.field<((BIT1 - 1) % GPS_NAV_MESSAGE_BITS_PER_WORD) + 1, BITS1>();
        unsigned long retval = field1;

        return retval;
      }

      template<std::size_t BIT1, std::size_t BITS1>
      long field1signed() const
      {
        const gps_nav_message_word& word1 = words[(BIT1 - 1) / GPS_NAV_MESSAGE_BITS_PER_WORD];
        unsigned long field1 = word1.field<((BIT1 - 1) % GPS_NAV_MESSAGE_BITS_PER_WORD) + 1, BITS1>();
        long retval = field1;

        if (retval & (1UL << (BITS1 - 1))) // MSB is set, thus it is a negative value
          retval = -(1UL << (BITS1 - 1)) + (retval & ~(1UL << (BITS1 - 1)));

        return retval;
      }

      template<std::size_t BIT1, std::size_t BITS1, std::size_t BIT2, std::size_t BITS2>
      unsigned long field2unsigned() const
      {
        const gps_nav_message_word& word1 = words[(BIT1 - 1) / GPS_NAV_MESSAGE_BITS_PER_WORD];
        const gps_nav_message_word& word2 = words[(BIT2 - 1) / GPS_NAV_MESSAGE_BITS_PER_WORD];

        unsigned long field1 = word1.field<((BIT1 - 1) % GPS_NAV_MESSAGE_BITS_PER_WORD) + 1, BITS1>();
        unsigned long field2 = word2.field<((BIT2 - 1) % GPS_NAV_MESSAGE_BITS_PER_WORD) + 1, BITS2>();
        unsigned long retval = field1 << BITS2 | field2;

        return retval;
      }

      template<std::size_t BIT1, std::size_t BITS1, std::size_t BIT2, std::size_t BITS2>
      unsigned long field2signed() const
      {
        const gps_nav_message_word& word1 = words[(BIT1 - 1) / GPS_NAV_MESSAGE_BITS_PER_WORD];
        const gps_nav_message_word& word2 = words[(BIT2 - 1) / GPS_NAV_MESSAGE_BITS_PER_WORD];

        unsigned long field1 = word1.field<((BIT1 - 1) % GPS_NAV_MESSAGE_BITS_PER_WORD) + 1, BITS1>();
        unsigned long field2 = word2.field<((BIT2 - 1) % GPS_NAV_MESSAGE_BITS_PER_WORD) + 1, BITS2>();
        long retval = field1 << BITS2 | field2;

        if (retval & (1UL << (BITS1 + BITS2 - 1))) // MSB is set, thus it is a negative value
          retval = -(1UL << (BITS1 + BITS2 - 1)) + (retval & ~(1UL << (BITS1 + BITS2 - 1)));

        return retval;
      }

      std::array<gps_nav_message_word, GPS_NAV_MESSAGE_WORDS_PER_SUBFRAME> words;
      bool is_initialized;
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_GPS_NAV_MESSAGE_SUBFRAME_HEADER_H */

