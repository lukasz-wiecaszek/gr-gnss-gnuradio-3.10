/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_CA_CODE_GENERATOR_H
#define INCLUDED_GNSS_CA_CODE_GENERATOR_H

#include <cstdint>
#include <complex>

#include <gnuradio/sync_block.h>
#include <gnuradio/gnss/api.h>
#include <gnuradio/gnss/ca_code_domain.h>

namespace gr {
  namespace gnss {

    /*!
     * \brief <+description of block+>
     * \ingroup gnss
     */
    template<typename T>
    struct GNSS_API ca_code_generator : virtual public gr::sync_block
    {
      using sptr = std::shared_ptr<ca_code_generator<T>>;

      /*!
       * \brief Return a shared_ptr to a new instance of gnss::ca_code_generator block.
       */
      static sptr make(std::size_t vlen, double sampling_freq, unsigned svid, ca_code_domain_e domain = CA_CODE_DOMAIN_TIME);
    };

    using ca_code_generator_s8   = ca_code_generator<std::int8_t>;
    using ca_code_generator_s16  = ca_code_generator<std::int16_t>;
    using ca_code_generator_s32  = ca_code_generator<std::int32_t>;
    using ca_code_generator_f32  = ca_code_generator<float>;
    using ca_code_generator_f64  = ca_code_generator<double>;
    using ca_code_generator_fc32 = ca_code_generator<std::complex<float>>;

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_CA_CODE_GENERATOR_H */
