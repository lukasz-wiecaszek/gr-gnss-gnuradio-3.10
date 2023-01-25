/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_RFT_IMPL_H
#define INCLUDED_GNSS_RFT_IMPL_H

#include <gnuradio/gnss/rft.h>

#define IVLEN 1
#define OVLEN 1

namespace gr {
  namespace gnss {

    template<typename ITYPE, typename OTYPE>
    class rft_impl : public rft
    {
    public:
      rft_impl();
      ~rft_impl();

      void set_transformation(rft_type_e type) override;
      void get_transformation(rft_type_e& type) const override;

      int work(int noutput_items,
               gr_vector_const_void_star &input_items,
               gr_vector_void_star &output_items);

    private:
      rft_type_e d_rft_type;
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_RFT_IMPL_H */
