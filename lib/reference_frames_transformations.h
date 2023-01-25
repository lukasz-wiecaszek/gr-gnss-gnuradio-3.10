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

#ifndef INCLUDED_GNSS_REFERENCE_FRAMES_TRANSFORMATIONS_H
#define INCLUDED_GNSS_REFERENCE_FRAMES_TRANSFORMATIONS_H

#include <cmath>

#include "gnss_parameters.h"
#include "vector3d.h"

namespace gr {
  namespace gnss {
    namespace rftns {

      constexpr double Hmin = /* e^12 / 4 */
          (WGS84_E * WGS84_E * WGS84_E * WGS84_E *
          WGS84_E * WGS84_E * WGS84_E * WGS84_E *
          WGS84_E * WGS84_E * WGS84_E * WGS84_E) / 4.0;

      constexpr double INVCBRT2 = 7.93700525984099737380e-01; /* 1/(2^(1/3)) */
      constexpr double INV3     = 3.33333333333333333333e-01; /* 1/3 */
      constexpr double INV6     = 1.66666666666666666667e-01; /* 1/6 */

      /**
       * @brief Geographic/Geospatial Coordinate System (GCS) to Earth Centered Earth Fixed (ECEF)
       *
       * @param[in] gcs  Geographical coordinates (latitude, longitude, altitude)
       * @param[out] ecef Earth-Centered Earth-Fixed coordinates (x, y, z)
       *
       * @return 0 on success, other value on failure
       * @note Implementation taken from https://hal.archives-ouvertes.fr/hal-01704943v2/document
       */
      inline int gcs_to_ecef(const vector3d& gcs, vector3d& ecef)
      {
        double lat = gcs.get({0}, {}) * M_PI / 180.0; // convert degrees to radians
        double lon = gcs.get({1}, {}) * M_PI / 180.0; // convert degrees to radians
        double alt = gcs.get({2}, {});
        double cos_lat = std::cos(lat);
        double sin_lat = std::sin(lat);
        double cos_lon = std::cos(lon);
        double sin_lon = std::sin(lon);
        double N = (WGS84_A * WGS84_A / WGS84_C) / std::sqrt(cos_lat * cos_lat + (WGS84_B * WGS84_B) / (WGS84_C * WGS84_C));
        double d = (N + alt) * cos_lat;
        double x = d * cos_lon;
        double y = d * sin_lon;
        double z = ((1.0 - WGS84_E * WGS84_E) * N + alt) * sin_lat;

        ecef.set({0}, {}, x);
        ecef.set({1}, {}, y);
        ecef.set({2}, {}, z);

        return 0;
      }

      /**
       * @brief Earth Centered Earth Fixed (ECEF) to Geographic/Geospatial Coordinate System (GCS)
       *
       * @param[in] ecef Earth-Centered Earth-Fixed coordinates (x, y, z)
       * @param[out] gcs Geographical coordinates (latitude, longitude, altitude)
       *
       * @return 0 on success, other value on failure
       * @note Implementation taken from https://hal.archives-ouvertes.fr/hal-01704943v2/document
       */
      inline int ecef_to_gcs(const vector3d& ecef, vector3d& gcs)
      {
        double x, y, z;
        double lat, lon, alt;

        // The variables below correspond to symbols used in the paper
        // "Accurate Conversion of Earth-Centered, Earth-Fixed Coordinates
        // to Geodetic Coordinates"
        // https://hal.archives-ouvertes.fr/hal-01704943v2/document
        double beta;
        double C;
        double dFdt;
        double dt;
        double dw;
        double dz;
        double F;
        double G;
        double H;
        double i;
        double k;
        double m;
        double n;
        double p;
        double P;
        double t;
        double u;
        double v;
        double w;

        // Intermediate variables
        double j;
        double ww;      // w^2
        double mpn;     // m+n
        double g;
        double tt;      // t^2
        double ttt;     // t^3
        double tttt;    // t^4
        double zu;      // z * u
        double wv;      // w * v
        double invuv;   // 1 / (u * v)
        double da;
        double t1, t2, t3, t4, t5, t6, t7;

        x = ecef.get({0}, {});
        y = ecef.get({1}, {});
        z = ecef.get({2}, {});

        ww = x * x + y * y;
        m = ww / (WGS84_A * WGS84_A);
        n = z * z * ((1.0 - WGS84_E * WGS84_E) / (WGS84_A * WGS84_A));
        mpn = m + n;
        p = INV6 * (mpn - WGS84_E * WGS84_E * WGS84_E * WGS84_E);
        G = m * n * WGS84_E * WGS84_E * WGS84_E * WGS84_E / 4.0;
        H = 2.0 * p * p * p + G;

        if (H < Hmin)
        {
            return -1;
        }

        C = INVCBRT2 * std::pow(H + G + 2.0 * std::sqrt(H * G), INV3);
        i = -(WGS84_E * WGS84_E * WGS84_E * WGS84_E / 4.0 + 0.5 * mpn);
        P = p * p;
        beta = INV3 * i - C - P / C;
        k = (WGS84_E * WGS84_E * WGS84_E * WGS84_E / 4.0) * (WGS84_E * WGS84_E * WGS84_E * WGS84_E / 4.0 - mpn);

        // Compute left part of t
        t1 = beta * beta - k;
        t2 = std::sqrt(t1);
        t3 = t2 - 0.5 * (beta + i);
        t4 = std::sqrt(t3);

        // Compute right part of t
        // t5 may accidentally drop just below zero due to numeric turbulence
        // This only occurs at latitudes close to +- 45.3 degrees
        t5 = std::fabs(0.5 * (beta - i));
        t6 = std::sqrt(t5);
        t7 = (m < n) ? t6 : -t6;

        // Add left and right parts
        t = t4 + t7;

        // Use Newton-Raphson's method to compute t correction
        j = (WGS84_E * WGS84_E / 2.0) * (m - n);
        g = 2.0 * j;
        tt = t * t;
        ttt = tt * t;
        tttt = tt * tt;
        F = tttt + 2.0 * i * tt + g * t + k;
        dFdt = 4.0 * ttt + 4.0 * i * t + g;
        dt = -F / dFdt;

        // compute latitude (range -PI/2..PI/2)
        u = t + dt + (WGS84_E * WGS84_E / 2.0);
        v = t + dt - (WGS84_E * WGS84_E / 2.0);
        w = std::sqrt(ww);
        zu = z * u;
        wv = w * v;
        lat = std::atan2(zu, wv);

        // compute longitude (range -PI..PI)
        lon = std::atan2(y, x);

        // compute altitude
        invuv = 1.0 / (u * v);
        dw = w - wv * invuv;
        dz = z - zu * (1.0 - WGS84_E * WGS84_E) * invuv;
        da = std::sqrt(dw * dw + dz * dz);
        alt = (u < 1.0) ? -da : da;

        gcs.set({0}, {}, lat * 180.0 / M_PI); // convert degrees to radians
        gcs.set({1}, {}, lon * 180.0 / M_PI); // convert degrees to radians
        gcs.set({2}, {}, alt);

        return 0;
      }

    } // namespace rft
  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_REFERENCE_FRAMES_TRANSFORMATIONS_H */

