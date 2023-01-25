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

#ifndef INCLUDED_GNSS_GPS_NAV_MESSAGE_SUBFRAME_H
#define INCLUDED_GNSS_GPS_NAV_MESSAGE_SUBFRAME_H

#include "gps_nav_message_subframe_1.h"
#include "gps_nav_message_subframe_2.h"
#include "gps_nav_message_subframe_3.h"
#include "gps_nav_message_subframe_4.h"
#include "gps_nav_message_subframe_5.h"

namespace gr {
  namespace gnss {

    class gps_nav_message_subframe :
      public gps_nav_message_subframe_1,
      public gps_nav_message_subframe_2,
      public gps_nav_message_subframe_3,
      public gps_nav_message_subframe_4,
      public gps_nav_message_subframe_5
    {
    public:
      template<typename T>
      void init(const T** vptr)
      {
        T D30 = 0;
        for (std::size_t i = 0; i < words.size(); D30 = *(*vptr - 1), ++i)
          words[i].init(vptr, D30);

        unsigned id = subframe_id();
        if (id >= 1 && id <= 5)
          is_initialized = true; // of course proper error checking need to be implemened
        else
          is_initialized = false;

        //for (std::size_t i = 0; i < words.size(); ++i)
        //  printf("[%zu]: '%s'\n", i, words[i].to_string().c_str());
      }

      std::string to_string() const
      {
        std::string str;

#define CASE(x) case x: str = gps_nav_message_subframe_##x::to_string(); break
        switch (subframe_id())
        {
          CASE(1);
          CASE(2);
          CASE(3);
          CASE(4);
          CASE(5);
        }
#undef CASE

        return str;
      }

      operator std::string () const
      {
        return to_string();
      }
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_GPS_NAV_MESSAGE_SUBFRAME_H */

