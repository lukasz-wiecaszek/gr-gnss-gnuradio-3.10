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

#ifndef INCLUDED_GNSS_GPS_NAV_MESSAGE_SUBFRAME_3_H
#define INCLUDED_GNSS_GPS_NAV_MESSAGE_SUBFRAME_3_H

#include <string>
#include "gps_nav_message_subframe_header.h"

namespace gr {
  namespace gnss {

    struct gps_nav_message_subframe_3 : public virtual gps_nav_message_subframe_header
    {
      int16_t C_IC()       const { return field1signed<61, 16>();          }
      int32_t OMEGA_0()    const { return field2signed<77, 8, 91, 24>();   }
      int16_t C_IS()       const { return field1signed<121, 16>();         }
      int32_t i_0()        const { return field2signed<137, 8, 151, 24>(); }
      int16_t C_RC()       const { return field1signed<181, 16>();         }
      int32_t omega()      const { return field2signed<197, 8, 211, 24>(); }
      int32_t dOMEGA_dt()  const { return field1signed<241, 24>();         }
      uint8_t IODE()       const { return field1unsigned<271, 8>();        }
      int16_t dI_dt()      const { return field1signed<279, 14>();         }

      std::string to_string() const
      {
        char strbuf[256];

        snprintf(strbuf, sizeof(strbuf),
          "%s | C_IC: %d, OMEGA_0: %d, C_IS: %d, i_0: %d, C_RC: %d, omega: %d, dOMEGA_dt: %d, IODE: %u, dI_dt: %d",
            gps_nav_message_subframe_header::to_string().c_str(),
              C_IC(), OMEGA_0(), C_IS(), i_0(), C_RC(), omega(), dOMEGA_dt(), IODE(), dI_dt());

        return std::string(strbuf);
      }

      operator std::string () const
      {
        return to_string();
      }
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_GPS_NAV_MESSAGE_SUBFRAME_3_H */

