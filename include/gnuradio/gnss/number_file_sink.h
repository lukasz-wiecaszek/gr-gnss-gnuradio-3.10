/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_NUMBER_FILE_SINK_H
#define INCLUDED_GNSS_NUMBER_FILE_SINK_H

#include <cstdint>

#include <gnuradio/sync_block.h>
#include <gnuradio/gnss/api.h>

namespace gr {
  namespace gnss {

    /*!
     * \brief <+description of block+>
     * \ingroup gnss
     */
    template<typename ITYPE>
    struct GNSS_API number_file_sink : virtual public gr::sync_block
    {
      using sptr = std::shared_ptr<number_file_sink<ITYPE>>;

      /*!
       * \brief Return a shared_ptr to a new instance of gnss::number_file_sink block.
       */
      static sptr make(std::size_t vlen, const char *filename, const char *delimiter);
    };

    using number_file_sink_s8 = number_file_sink<std::int8_t>;
    using number_file_sink_u8 = number_file_sink<std::uint8_t>;
    using number_file_sink_s16 = number_file_sink<std::int16_t>;
    using number_file_sink_u16 = number_file_sink<std::uint16_t>;
    using number_file_sink_s32 = number_file_sink<std::int32_t>;
    using number_file_sink_u32 = number_file_sink<std::uint32_t>;
    using number_file_sink_s64 = number_file_sink<std::int64_t>;
    using number_file_sink_u64 = number_file_sink<std::uint64_t>;
    using number_file_sink_f32 = number_file_sink<float>;
    using number_file_sink_f64 = number_file_sink<double>;
    using number_file_sink_fc32 = number_file_sink<std::complex<float>>;
    using number_file_sink_fc64 = number_file_sink<std::complex<double>>;

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_NUMBER_FILE_SINK_H */
