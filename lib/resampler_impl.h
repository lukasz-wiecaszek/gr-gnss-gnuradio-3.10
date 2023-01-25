/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_RESAMPLER_IMPL_H
#define INCLUDED_GNSS_RESAMPLER_IMPL_H

#include <gnuradio/gnss/resampler.h>

#define IVLEN 1
#define OVLEN 1

namespace gr {
  namespace gnss {

    template<typename ITYPE, typename OTYPE>
    class resampler_impl : public resampler
    {
    private:
      const double d_fs_in;
      const double d_fs_out;
      const uint32_t d_phase_step;
      uint32_t d_phase;
      uint32_t d_lphase;

    public:
      resampler_impl(double fs_in, double fs_out);
      ~resampler_impl();

      void forecast(int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_RESAMPLER_IMPL_H */
