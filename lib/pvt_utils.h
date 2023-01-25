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

      struct satelite
      {
        vector3d position; // satelite position
        vector3d velocity; // satelite velocity
        double pseudorange; // associated pseudorange measurement

        satelite() :
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
      int get(const satelite* s, user& hint, vector3d* position, vector3d* velocity, double* t)
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
      int get<4>(const satelite* s, user& hint, vector3d* position, vector3d* velocity, double* t)
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

        if (position)
          *position = hint.position;

        if (velocity)
          ;// not supported yet

        if (t)
          *t = hint.dt;

        return 0;
      }

      template<std::size_t N>
      inline
      int get(const satelite (&s)[N], user& hint, vector3d* position, vector3d* velocity, double* t)
      {
        return get<N>(s, hint, position, velocity, t);
      }

      template<std::size_t N>
      inline
      int get(const std::array<satelite, N>& s, user& hint, vector3d* position, vector3d* velocity, double* t)
      {
        return get<N>(s, hint, position, velocity, t);
      }

    } // namespace pvt_utils
  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_PVT_UTILS_H */

