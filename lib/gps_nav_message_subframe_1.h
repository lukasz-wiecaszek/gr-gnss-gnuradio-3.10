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

#ifndef INCLUDED_GNSS_GPS_NAV_MESSAGE_SUBFRAME_1_H
#define INCLUDED_GNSS_GPS_NAV_MESSAGE_SUBFRAME_1_H

#include <string>
#include "gps_nav_message_subframe_header.h"

namespace gr {
  namespace gnss {

    struct gps_nav_message_subframe_1 : public virtual gps_nav_message_subframe_header
    {
      uint16_t WN()           const { return field1unsigned<61, 10>();        }
      uint8_t CA_OR_P_ON_L2() const { return field1unsigned<71, 2>();         }
      uint8_t URA_INDEX()     const { return field1unsigned<73, 4>();         }
      uint8_t SV_HEALTH()     const { return field1unsigned<77, 6>();         }
      uint16_t IODC()         const { return field2unsigned<83, 2, 211, 8>(); }
      bool L2_P_DATA_FLAG()   const { return flag<91>();                      }
      int8_t T_GD()           const { return field1signed<197, 8>();          }
      uint16_t t_oc()         const { return field1unsigned<219, 16>();       }
      int32_t a_f2()          const { return field1signed<241, 8>();          }
      int32_t a_f1()          const { return field1signed<249, 16>();         }
      int32_t a_f0()          const { return field1signed<271, 22>();         }

      std::string to_string() const
      {
        char strbuf[256];

        snprintf(strbuf, sizeof(strbuf),
          "%s | WN: %u, CA_OR_P_ON_L2: %u, URA_INDEX: %u, SV_HEALTH: %u, IODC: %u, T_GD: %d, t_oc: %u, a_f0: %d, a_f1: %d, a_f2: %d",
            gps_nav_message_subframe_header::to_string().c_str(),
              WN(), CA_OR_P_ON_L2(), URA_INDEX(), SV_HEALTH(), IODC(), T_GD(), t_oc(), a_f0(), a_f1(), a_f2());

        return std::string(strbuf);
      }

      operator std::string () const
      {
        return to_string();
      }
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_GPS_NAV_MESSAGE_SUBFRAME_1_H */

