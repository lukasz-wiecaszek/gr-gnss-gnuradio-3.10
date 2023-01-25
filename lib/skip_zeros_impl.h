/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_SKIP_ZEROS_IMPL_H
#define INCLUDED_GNSS_SKIP_ZEROS_IMPL_H

#include <gnuradio/gnss/skip_zeros.h>
#include "vector3d.h"

#define IVLEN 1
#define OVLEN 1

namespace gr {
  namespace gnss {

    template<typename ITYPE, typename OTYPE>
    class skip_zeros_impl : public skip_zeros
    {
    public:
      skip_zeros_impl();
      ~skip_zeros_impl();

      int work(int noutput_items,
               gr_vector_const_void_star &input_items,
               gr_vector_void_star &output_items);

    private:
      bool is_null(const vector3d& v) {
        return
          (v.get({0}, {0}) == 0) &&
          (v.get({1}, {0}) == 0) &&
          (v.get({2}, {0}) == 0);
      }
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_SKIP_ZEROS_IMPL_H */
