/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_PVT_IMPL_H
#define INCLUDED_GNSS_PVT_IMPL_H

#include <gnuradio/gnss/pvt.h>
#include "pvt_utils.h"

#define IVLEN 1
#define OVLEN 1
#define MAX_STREAMS 8

namespace gr {
  namespace gnss {

    template<typename ITYPE, typename OTYPE>
    class pvt_impl : public pvt
    {
    public:
      pvt_impl(bool skip_zeros);
      ~pvt_impl();

      void forecast(int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items);

    private:
      pvt_utils::user d_hint;
      bool d_skip_zeros;
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_PVT_IMPL_H */
