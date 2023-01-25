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

#ifndef INCLUDED_GNSS_WGS84_PARAMETERS_H
#define INCLUDED_GNSS_WGS84_PARAMETERS_H

#include <cstdint>

namespace gr {
  namespace gnss {

    // WGS 84 Nominal Mean Angular Velocity of the Earth [rad/s]
    constexpr double WGS84_OMEGA = 7.29211500000000000000e-05;

    // WGS 84 value of the Earth's rotation rate for GPS user [rad/s]
    constexpr double WGS84_dOMEGA_dt_EARTH = 7.29211514670000000000e-05;

    // Oblate Earth Gravity Coefficient
    constexpr double WGS84_J2 = 1.08262620000000000000e-03;

    // WGS 84 Geocentric Gravitational Constant Times the Mass of the Earth (Mass of Earth’s Atmosphere Included) [m^3/s^2]
    constexpr double WGS84_GM = 3.98600441800000000000e+14;

    // WGS 84 Geocentric Gravitational Constant Times the Mass of the Earth for GPS user [m^3/s^2]
    constexpr double WGS84_GM_FOR_GPS = 3.98600500000000000000e+14;

    // WGS 84 Semi-major Axis (Earth Equatorial Radius) [m]
    constexpr double WGS84_A = 6.37813700000000000000e+06;

    // WGS 84 Flattening Factor of the Earth (1/f)
    constexpr double WGS84_INVF = 2.98257223563000000000e+02;

    // WGS 84 Reciprocal of Flattening Factor of the Earth (f)
    constexpr double WGS84_F = 1.0 / WGS84_INVF;

    // WGS 84 Semi-minor Axis [m]
    constexpr double WGS84_B = WGS84_A * (1.0 - WGS84_F);

    // WGS 84 Linear Eccentricity (c = sqrt(a*a - b*b)) [m]
    constexpr double WGS84_C = 5.21854008423385332406e+05;

    // WGS 84 Eccentricity (e = sqrt(1 - (a/b)^2) = c/a)
    constexpr double WGS84_E = WGS84_C / WGS84_A;

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_WGS84_PARAMETERS_H */

