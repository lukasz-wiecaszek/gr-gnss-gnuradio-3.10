/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_TYPE_CONVERTER_H
#define INCLUDED_GNSS_TYPE_CONVERTER_H

#include <gnuradio/sync_block.h>
#include <gnuradio/gnss/api.h>

namespace gr {
  namespace gnss {

    /*!
     * \brief <+description of block+>
     * \ingroup gnss
     */
    template<typename ITYPE, typename OTYPE>
    struct GNSS_API type_converter : virtual public gr::sync_block
    {
      using sptr = std::shared_ptr<type_converter<ITYPE, OTYPE>>;

      /*!
       * \brief Return a shared_ptr to a new instance of gnss::type_converter block.
       */
      static sptr make(size_t vlen);
    };

    using f32_to_f64 = type_converter<float, double>;
    using f64_to_f32 = type_converter<double, float>;
    using fc32_to_fc64 = type_converter<std::complex<float>, std::complex<double>>;
    using fc64_to_fc32 = type_converter<std::complex<double>, std::complex<float>>;

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_TYPE_CONVERTER_H */
