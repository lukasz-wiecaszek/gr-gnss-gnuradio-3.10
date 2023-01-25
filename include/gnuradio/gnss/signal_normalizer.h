/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_SIGNAL_NORMALIZER_H
#define INCLUDED_GNSS_SIGNAL_NORMALIZER_H

#include <gnuradio/block.h>
#include <gnuradio/gnss/api.h>

namespace gr {
  namespace gnss {

    /*!
     * \brief Converts different input IQ formats to
     *        std::complex<float> or std::complex<double>.
     *        All samples/items are normalized to values
     *        in the range [-1.0 .. +1.0] for I and Q parts.
     * \ingroup gnss
     */
    template<typename ITYPE, typename OTYPE>
    struct GNSS_API signal_normalizer : virtual public gr::block
    {
      using sptr = std::shared_ptr<signal_normalizer<ITYPE, OTYPE>>;

      /*!
       * \brief Return a shared_ptr to a new instance of gnss::signal_normalizer block.
       */
      static sptr make(size_t vlen, size_t adc_resolution);
    };

    using signal_normalizer_s8_fc32 = signal_normalizer<std::int8_t, std::complex<float>>;
    using signal_normalizer_u8_fc32 = signal_normalizer<std::uint8_t, std::complex<float>>;
    using signal_normalizer_s16_fc32 = signal_normalizer<std::int16_t, std::complex<float>>;
    using signal_normalizer_u16_fc32 = signal_normalizer<std::uint16_t, std::complex<float>>;

    using signal_normalizer_s8_fc64 = signal_normalizer<std::int8_t, std::complex<double>>;
    using signal_normalizer_u8_fc64 = signal_normalizer<std::uint8_t, std::complex<double>>;
    using signal_normalizer_s16_fc64 = signal_normalizer<std::int16_t, std::complex<double>>;
    using signal_normalizer_u16_fc64 = signal_normalizer<std::uint16_t, std::complex<double>>;

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_SIGNAL_NORMALIZER_H */
