/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_DOPPLER_SHIFT_IMPL_H
#define INCLUDED_GNSS_DOPPLER_SHIFT_IMPL_H

#include <gnuradio/gnss/doppler_shift.h>

#include "vector3d.h"

#define IVLEN 1
#define OVLEN 1

namespace gr {
  namespace gnss {

    template<typename ITYPE, typename OTYPE>
    class doppler_shift_impl : public doppler_shift
    {
    public:
      doppler_shift_impl(double x, double y, double z);
      ~doppler_shift_impl();

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

      vector3d d_receiver_position;
      double d_t;
      vector3d d_r;
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_DOPPLER_SHIFT_IMPL_H */
