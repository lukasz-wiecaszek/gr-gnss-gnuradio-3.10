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

#ifndef INCLUDED_GNSS_GPS_NAV_MESSAGE_SUBFRAME_5_H
#define INCLUDED_GNSS_GPS_NAV_MESSAGE_SUBFRAME_5_H

#include <string>
#include "gps_nav_message_subframe_header.h"

namespace gr {
  namespace gnss {

    struct gps_nav_message_subframe_5 : public virtual gps_nav_message_subframe_header
    {
      std::string to_string() const
      {
        std::ostringstream stream;

        stream << gps_nav_message_subframe_header::to_string();

        return stream.str();
      }

      operator std::string () const
      {
        return to_string();
      }
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_GPS_NAV_MESSAGE_SUBFRAME_5_H */

