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

#ifndef INCLUDED_GNSS_VECTOR3D_H
#define INCLUDED_GNSS_VECTOR3D_H

#include <cstdio>
#include <cmath>
#include <string>
#include <tensor.hpp>

namespace gr {
  namespace gnss {

    using vector3d = lts::tensor<double, lts::dimensions<3>, lts::dimensions<>>;

    inline double norm(const vector3d& v)
    {
      const lts::tensor<double, lts::dimensions<3>, lts::dimensions<3>> g{{
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
      }};
      return lts::transpose(v) * g * v;
    }

    inline double abs(const vector3d& v)
    {
      return std::sqrt(norm(v));
    }

    inline std::string to_string(const vector3d& v)
    {
        char strbuf[1024];

        snprintf(strbuf, sizeof(strbuf),
          "(%+.15e, %+.15e, %+.15e) [%.15e]",
          v.get({0}, {}),
          v.get({1}, {}),
          v.get({2}, {}),
          abs(v)
        );

        return std::string(strbuf);
    }

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_VECTOR3D_H */

