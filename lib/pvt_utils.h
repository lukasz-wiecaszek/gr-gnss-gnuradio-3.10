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

#ifndef INCLUDED_GNSS_PVT_UTILS_H
#define INCLUDED_GNSS_PVT_UTILS_H

#include <cstdio>
#include <string>

#include "vector3d.h"
#include "gnss_parameters.h"

namespace gr {
  namespace gnss {
    namespace pvt_utils {

      struct satellite
      {
        vector3d position; // satellite position
        vector3d velocity; // satellite velocity
        double pseudorange; // associated pseudorange measurement

        satellite() :
          position{{0.0, 0.0, 0.0}},
          velocity{{0.0, 0.0, 0.0}},
          pseudorange{0.0}
        {
        }

        std::string to_string() const
        {
          char strbuf[1024];

          snprintf(strbuf, sizeof(strbuf),
            "r: %s, v: %s, pseudorange: %+.15e",
            gr::gnss::to_string(position).c_str(),
            gr::gnss::to_string(velocity).c_str(),
            pseudorange);

          return std::string(strbuf);
        }

        operator std::string() const
        {
          return to_string();
        }
      };

      struct user
      {
        vector3d position; // user/receiver position
        double dt; // advance of the user/reveiver clock with respect to system time

        user() :
          position{{0.0, 0.0, 0.0}},
          dt{0.0}
        {
        }

        std::string to_string() const
        {
          char strbuf[1024];

          snprintf(strbuf, sizeof(strbuf),
            "%s dt: %+.15e", gr::gnss::to_string(position).c_str(), dt);

          return std::string(strbuf);
        }

        operator std::string() const
        {
          return to_string();
        }
      };

      template<int N>
      inline
      int get(const satellite* s, user& hint, vector3d* position, vector3d* velocity, double* t)
      {
        constexpr int M = 4;
        lts::tensor<double, lts::dimensions<N>, lts::dimensions<M>> H1;
        lts::tensor<double, lts::dimensions<M>, lts::dimensions<N>> H2; // this is a transpose of H1
        lts::tensor<double, lts::dimensions<M>, lts::dimensions<M>> H; // this is a H2 * H1

        lts::tensor<double, lts::dimensions<N>, lts::dimensions<>> dq;
        lts::tensor<double, lts::dimensions<M>, lts::dimensions<>> ds;

        do {
          for (int n = 0; n < N; ++n) {
            vector3d r = s[n].position - hint.position;
            double r_magnitude = abs(r);
            double u_pseudorange = r_magnitude + C * hint.dt; // user/approximated pseudorange
            dq.set({n}, {}, s[n].pseudorange - u_pseudorange);
            int m = 0;
            for (; m < (M - 1); ++m)
              H1.set({n}, {m}, (hint.position.get({m}, {}) - s[n].position.get({m}, {})) / r_magnitude);
            H1.set({n}, {m}, 1.0);
          }

          H2 = lts::transpose(H1);
          H = H2 * H1;

          double det = H.det();
          if (det != 0) {
            ds = inverse(H, det) * (H2 * dq);
          }
          else
            break;

          hint.position.set({0}, {}, hint.position.get({0}, {}) + ds.get({0}, {}));
          hint.position.set({1}, {}, hint.position.get({1}, {}) + ds.get({1}, {}));
          hint.position.set({2}, {}, hint.position.get({2}, {}) + ds.get({2}, {}));
          hint.dt = hint.dt + ds.get({3}, {}) / C;
        } while (std::abs(ds.get({0}, {})) > 1.0 ||
                 std::abs(ds.get({1}, {})) > 1.0 ||
                 std::abs(ds.get({2}, {})) > 1.0 ||
                 std::abs(ds.get({3}, {})) > 1.0);

        if (position)
          *position = hint.position;

        if (velocity)
          ;// not supported yet

        if (t)
          *t = hint.dt;

        return 0;
      }

      template<> // this is specialization when N == 4
      inline
      int get<4>(const satellite* s, user& hint, vector3d* position, vector3d* velocity, double* t)
      {
        constexpr int N = 4;
        constexpr int M = 4;
        lts::tensor<double, lts::dimensions<N>, lts::dimensions<M>> H;
        lts::tensor<double, lts::dimensions<N>, lts::dimensions<>> dq;
        lts::tensor<double, lts::dimensions<N>, lts::dimensions<>> ds;

        do {
          for (int n = 0; n < N; ++n) {
            vector3d r = s[n].position - hint.position;
            double r_magnitude = abs(r);
            double u_pseudorange = r_magnitude + C * hint.dt; // user/approximated pseudorange
            dq.set({n}, {}, s[n].pseudorange - u_pseudorange);
            int m = 0;
            for (; m < (M - 1); ++m)
              H.set({n}, {m}, (hint.position.get({m}, {}) - s[n].position.get({m}, {})) / r_magnitude);
            H.set({n}, {m}, 1.0);
          }

          double det = H.det();
          if (det != 0)
            ds = inverse(H, det) * dq;
          else
            break;

          hint.position.set({0}, {}, hint.position.get({0}, {}) + ds.get({0}, {}));
          hint.position.set({1}, {}, hint.position.get({1}, {}) + ds.get({1}, {}));
          hint.position.set({2}, {}, hint.position.get({2}, {}) + ds.get({2}, {}));
          hint.dt = hint.dt + ds.get({3}, {}) / C;
        } while (std::abs(ds.get({0}, {})) > 1.0 ||
                 std::abs(ds.get({1}, {})) > 1.0 ||
                 std::abs(ds.get({2}, {})) > 1.0 ||
                 std::abs(ds.get({3}, {})) > 1.0);

        /*
        There is too big simplification (or I shall call it error, without beating around the bush)
        in the above computation of user position.
        The correct algorithm shall be.
        1. Once satellite's positions are calculated, we shall select an arbitrary inertial frame for
           computation of path delays. The reason for that is, that satellite's positions
           are available in the EFEC frame which is slightly different for each of the satellites
           as each of them has slightly different transmission time
           (we are assuming "time tagging at the receiver" approach).
        2. We shall transform the EFEC coordinates of each of the satellites into the chosen inertial frame.
           If we freeze the EFEC frame at the transmission time of one of the satellites and this
           will be our inertial frame, then we are saved with one transformation.
           This is the approach I am going to apply later on in this implementation.
           I may skip this step for a while as the error introduced by not doing this procedure
           is not so big as error introduced by not doing procedure (4).
           The time difference between the earliest and the latest transmission
           in time tagging at the receiver approach is about 6 ms.
            +3.686154041043728e+05
            +3.686154000000000e+05
            +3.686154057984319e+05
            +3.686154021735514e+05
        3. Now we may solve the path delay equations for the receiver's position and time.
           The result will be given in the previously selected inertial frame.
        4. Finally we shall rotate the user's position again to EFEC frame.

        In the current implementataion I am still solving the equations in the EFEC frame
        (or I shall write in so many EFEC frames as used satellites) at time
        which "approximately" corresponds to transmission time.
        And the easiest way to correct this implementation a little is to rotate
        the user position by the "time" it takes for signal to travel from satellite to the Earth.
        Let's do it all approximately (which is still better than not doing it at all).
        So, the avarage distance from GPS satellite to the Earth is about 20200 km. It gives ~67 ms
        for an EM wave to travel that distance. And the rotation matrix is
        [ cos(fi) -sin(fi) 0 ]
        [ sin(fi)  cos(fi) 0 ]
        [ 0        0       1 ]
        where fi = -WGS84_OMEGA * delta t = 7.292115e-05 rad/s * 0.067 s = -4.88571e-06 rad
        */

        /* As mentioned above this is temporary solution */
        static const double dt = 0.067;
        static const double fi = -WGS84_OMEGA * dt;

        static const lts::tensor<double, lts::dimensions<3>, lts::dimensions<3>> R{{
          std::cos(fi), -std::sin(fi), 0.0,
          std::sin(fi),  std::cos(fi), 0.0,
          0.0,           0.0,          1.0
        }};

        if (position)
          *position = R * hint.position;

        if (velocity)
          ;// not supported yet

        if (t)
          *t = hint.dt;

        return 0;
      }

      template<std::size_t N>
      inline
      int get(const satellite (&s)[N], user& hint, vector3d* position, vector3d* velocity, double* t)
      {
        return get<N>(s, hint, position, velocity, t);
      }

      template<std::size_t N>
      inline
      int get(const std::array<satellite, N>& s, user& hint, vector3d* position, vector3d* velocity, double* t)
      {
        return get<N>(s, hint, position, velocity, t);
      }

    } // namespace pvt_utils
  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_PVT_UTILS_H */

