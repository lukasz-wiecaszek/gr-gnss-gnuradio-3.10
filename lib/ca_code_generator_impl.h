/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_CA_CODE_GENERATOR_IMPL_H
#define INCLUDED_GNSS_CA_CODE_GENERATOR_IMPL_H

#include <vector>

#include <gnuradio/gnss/ca_code_generator.h>

namespace gr {
  namespace gnss {

    template<typename T>
    class ca_code_generator_impl : public ca_code_generator<T>
    {
    public:
      ca_code_generator_impl(std::size_t vlen, double sampling_freq, unsigned svid, ca_code_domain_e domain);
      ~ca_code_generator_impl();

      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );

    private:
      const std::size_t d_vlen;
      const int d_n_samples;
      std::vector<T> d_code_sampled;
      int d_n;
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_CA_CODE_GENERATOR_IMPL_H */
