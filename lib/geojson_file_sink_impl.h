/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_GEOJSON_FILE_SINK_IMPL_H
#define INCLUDED_GNSS_GEOJSON_FILE_SINK_IMPL_H

#include <stdio.h>

#include <gnuradio/gnss/geojson_file_sink.h>

#define IVLEN 1

namespace gr {
  namespace gnss {

    template<typename ITYPE>
    class geojson_file_sink_impl : public geojson_file_sink
    {
    public:
      geojson_file_sink_impl(const char *filename);
      ~geojson_file_sink_impl();

      int work(int noutput_items,
               gr_vector_const_void_star &input_items,
               gr_vector_void_star &output_items);

    private:
      FILE *d_fp;
      std::size_t d_line_cnt;
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_GEOJSON_FILE_SINK_IMPL_H */
