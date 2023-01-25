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

#ifndef INCLUDED_GNSS_CA_CODE_H
#define INCLUDED_GNSS_CA_CODE_H

#include <array>
#include <memory>
#include <map>

#include "gnss_parameters.h"
#include "ca_code_phase_assigments.h"
#include "prbs.h"

namespace gr {
  namespace gnss {

    template<typename T, std::size_t N>
    class ca_code
    {
      static_assert(std::is_integral<T>::value, "T shall be an integral type");

    private:
      static inline const prbs<T, 10, 3> G1{N}; // C++17 allows for that
      static inline const prbs<T, 10, 9, 8, 6, 3, 2> G2{N};
      static inline std::map<int, std::shared_ptr<ca_code>> map;

      std::array<T, N> d_code;

    private:
      ca_code(uint32_t svid)
      {
        int delay = N - ca_code_phase_assigments[svid].delay;

        for (std::size_t i = 0; i < N; ++i)
          d_code[i] = G1[i] ^ G2[(i + delay) % N];
      }

    public:
      static std::shared_ptr<ca_code> get(uint32_t svid)
      {
        struct from_this : public ca_code {
          from_this(uint32_t svid) : ca_code(svid) {}
        }; // please, have mercy

        if (svid < std::size(ca_code_phase_assigments)) {
          auto it = map.find(svid);
          if (it != map.end())
            return it->second;
          else {
            std::shared_ptr<ca_code> code = std::make_shared<from_this>(svid);
            //return map.insert(std::make_pair(svid, code)).first->second;
            return map.insert(std::make_pair(svid, code)), code; // I believe this is more descriptive than the line above.
          }
        }
        else
          return nullptr;
      }

      T operator[](int x) const
      {
        return d_code[x % N];
      }

    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_CA_CODE_H */

