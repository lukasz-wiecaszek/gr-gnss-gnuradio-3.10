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

#ifndef INCLUDED_GNSS_VERLET_INTEGRATION_H
#define INCLUDED_GNSS_VERLET_INTEGRATION_H

#include "gnss_parameters.h"
#include "vector3d.h"

namespace gr {
  namespace gnss {

    struct verlet_integration
    {
      static void get_vectors(double dt, vector3d& r, vector3d &v, vector3d& a)
      {
        vector3d a0 = a;

        // new position at t + dt
        r = r + v * dt + 0.5 * a * dt * dt;

        double r_magnitude = abs(r);
        double x = r.get({0}, {});
        double y = r.get({1}, {});
        double z = r.get({2}, {});
        double x_r = x / r_magnitude;
        double y_r = y / r_magnitude;
        double z_r = z / r_magnitude;
        double vx = v.get({0}, {});
        double vy = v.get({1}, {});
        double theta = 5.0 * z_r * z_r;

        double F = -(3.0 / 2.0) * GPS_J2 * (GPS_MI / (r_magnitude * r_magnitude)) * (GPS_RE / r_magnitude) * (GPS_RE / r_magnitude); // Oblate Earth acceleration Factor
        vector3d f = vector3d{F * (1.0 - theta) * x_r, F * (1.0 - theta) * y_r, F * (3.0 - theta) * z_r}; // Oblate Earth acceleration correction
        vector3d o = vector3d{+2.0 * vy * GPS_dOMEGA_dt_EARTH + x * GPS_dOMEGA_dt_EARTH * GPS_dOMEGA_dt_EARTH,
                              -2.0 * vx * GPS_dOMEGA_dt_EARTH + y * GPS_dOMEGA_dt_EARTH * GPS_dOMEGA_dt_EARTH,
                               0.0};

        // new acceleration at t + dt
        a = -GPS_MI * r / (r_magnitude * r_magnitude * r_magnitude) + f + o;

        // new velocity at t + dt
        v = v + 0.5 * (a0 + a) * dt;
      }
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_VERLET_INTEGRATION_H */

