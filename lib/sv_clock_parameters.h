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

#ifndef INCLUDED_GNSS_SV_CLOCK_PARAMETERS_H
#define INCLUDED_GNSS_SV_CLOCK_PARAMETERS_H

#include <cstdio>
#include <cmath>
#include <string>

#include "gnss_parameters.h"

namespace gr {
  namespace gnss {

    struct sv_clock_parameters
    {
      sv_clock_parameters() noexcept :
        svid{-1},
        IODC{-1}
      {
      }

      sv_clock_parameters(const sv_clock_parameters& other) noexcept
      {
        *this = other;
      };

      sv_clock_parameters& operator = (const sv_clock_parameters& other) noexcept
      {
        if (this != &other) {
          svid        = other.svid;
          IODC        = other.IODC;
          week_no     = other.week_no;
          SV_accuracy = other.SV_accuracy;
          SV_health   = other.SV_health;
          T_GD        = other.T_GD;
          t_oc        = other.t_oc;
          a_f0        = other.a_f0;
          a_f1        = other.a_f1;
          a_f2        = other.a_f2;
        }

        return *this;
      };

      int svid;
      int IODC;         // Issue of Data - Clock
      int week_no;      // Week Number
      int SV_accuracy;  // Space Vehicle Accuracy
      int SV_health;    // Space Vehicle Health
      double T_GD;      // Estimated Group Delay Differential
      int t_oc;         // Clock data reference time of week [s] (IS-GPS-200L 20.3.4.5 "Reference Times")
      double a_f0;      // SV Clock Bias Correction Coefficient
      double a_f1;      // SV Clock Drift Correction Coefficient
      double a_f2;      // SV Clock Drift Rate Correction Coefficient

      // t is GPS system time at time of transmission, i.e., GPS time corrected for transit time (range/speed of light).
      // Furthermore, tk shall be the actual total time difference between the time t and the epoch time t_oe, and must
      // account for beginning or end of week crossovers.
      // That is, if tk is greater than 302400 seconds, subtract 604800 seconds from tk.
      // If tk is less than -302400 seconds, add 604800 seconds to tk.
      double get_tk(double t) const
      {
        double tk = t - t_oc;

        if (tk > (GPS_SECONDS_PER_WEEK / 2))
        {
          tk -= GPS_SECONDS_PER_WEEK;
        }
        else
        if (tk < -(GPS_SECONDS_PER_WEEK / 2))
        {
          tk += GPS_SECONDS_PER_WEEK;
        }

        return tk;
      }

      double code_phase_offset(double t) const
      {
        double tk = get_tk(t); // time difference relative to clock reference epoch [s]
        return a_f0 + a_f1 * tk + a_f2 * tk * tk - T_GD;
      }

      std::string to_string() const
      {
        char strbuf[1024];

        snprintf(strbuf, sizeof(strbuf),
          "sv_clock_parameters:\n"
          "\tsvid:        %d\n"
          "\tIODC:        %d\n"
          "\tt_oc:        %d\n"
          "\tweek_no:     %d\n"
          "\tSV_accuracy: %d\n"
          "\tSV_health:   %d\n"
          "\tT_GD:        %+e\n"
          "\ta_f0:        %+e\n"
          "\ta_f1:        %+e\n"
          "\ta_f2:        %+e\n",
          svid, IODC, t_oc, week_no, SV_accuracy, SV_health, T_GD, a_f0, a_f1, a_f2);

        return std::string(strbuf);
      }

      operator std::string () const
      {
        return to_string();
      }
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_SV_CLOCK_PARAMETERS_H */

