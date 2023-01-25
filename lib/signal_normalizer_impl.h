/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_SIGNAL_NORMALIZER_IMPL_H
#define INCLUDED_GNSS_SIGNAL_NORMALIZER_IMPL_H

#include <gnuradio/gnss/signal_normalizer.h>

namespace gr {
  namespace gnss {

    template<typename ITYPE, typename OTYPE>
    class signal_normalizer_impl : public signal_normalizer<ITYPE, OTYPE>
    {
    public:
      signal_normalizer_impl(size_t vlen, size_t adc_resolution);
      ~signal_normalizer_impl();

      void forecast(int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items);

    private:
      const size_t d_vlen;
      const size_t d_adc_resolution;
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_SIGNAL_NORMALIZER_IMPL_H */
