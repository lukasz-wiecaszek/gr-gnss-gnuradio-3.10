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

#ifndef INCLUDED_GNSS_GPS_NAV_MESSAGE_SUBFRAME_2_H
#define INCLUDED_GNSS_GPS_NAV_MESSAGE_SUBFRAME_2_H

#include <string>
#include "gps_nav_message_subframe_header.h"

namespace gr {
  namespace gnss {

    struct gps_nav_message_subframe_2 : public virtual gps_nav_message_subframe_header
    {
      uint8_t IODE()           const { return field1unsigned<61, 8>();           }
      int16_t C_RS()           const { return field1signed<69, 16>();            }
      int16_t DELTA_N()        const { return field1signed<91, 16>();            }
      int32_t M_0()            const { return field2signed<107, 8, 121, 24>();   }
      int16_t C_UC()           const { return field1signed<151, 16>();           }
      uint32_t e()             const { return field2unsigned<167, 8, 181, 24>(); }
      int16_t C_US()           const { return field1signed<211, 16>();           }
      uint32_t SQRT_A()        const { return field2unsigned<227, 8, 241, 24>(); }
      uint16_t t_oe()          const { return field1unsigned<271, 16>();         }
      bool fit_interval_flag() const { return flag<287>();                       }
      uint8_t AODO()           const { return field1unsigned<288, 5>();          }

      std::string to_string() const
      {
        char strbuf[256];

        snprintf(strbuf, sizeof(strbuf),
          "%s | IODE: %u, C_RS: %d, DELTA_N: %d, M_0: %d, C_UC: %d, e: %u, C_US: %d, SQRT_A: %u, t_oe: %u, AODO: %u",
            gps_nav_message_subframe_header::to_string().c_str(),
              IODE(), C_RS(), DELTA_N(), M_0(), C_UC(), e(), C_US(), SQRT_A(), t_oe(), AODO());

        return std::string(strbuf);
      }

      operator std::string () const
      {
        return to_string();
      }
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_GPS_NAV_MESSAGE_SUBFRAME_2_H */

