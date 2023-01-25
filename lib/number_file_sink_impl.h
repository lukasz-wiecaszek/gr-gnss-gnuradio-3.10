/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_NUMBER_FILE_SINK_IMPL_H
#define INCLUDED_GNSS_NUMBER_FILE_SINK_IMPL_H

#include <stdio.h>
#include <string>

#include <gnuradio/gnss/number_file_sink.h>

namespace gr {
  namespace gnss {

    template<typename ITYPE>
    class number_file_sink_impl : public number_file_sink<ITYPE>
    {
    public:
      number_file_sink_impl(std::size_t vlen, const char *filename, const char *delimiter);
      ~number_file_sink_impl();

      int work(int noutput_items,
               gr_vector_const_void_star &input_items,
               gr_vector_void_star &output_items);

    private:
      void print(const ITYPE *values[], std::size_t N);

      std::size_t d_vlen;
      FILE *d_fp;
      std::string d_delimiter;
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_NUMBER_FILE_SINK_IMPL_H */
