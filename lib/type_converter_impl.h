/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_TYPE_CONVERTER_IMPL_H
#define INCLUDED_GNSS_TYPE_CONVERTER_IMPL_H

#include <gnuradio/gnss/type_converter.h>

namespace gr {
  namespace gnss {

    template<typename ITYPE, typename OTYPE>
    class type_converter_impl : public type_converter<ITYPE, OTYPE>
    {
    public:
      type_converter_impl(size_t vlen);
      ~type_converter_impl();

      int work(int noutput_items,
               gr_vector_const_void_star &input_items,
               gr_vector_void_star &output_items);

    private:
      const size_t d_vlen;
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_TYPE_CONVERTER_IMPL_H */
