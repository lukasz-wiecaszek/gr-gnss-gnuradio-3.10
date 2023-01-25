/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_GEOJSON_FILE_SINK_H
#define INCLUDED_GNSS_GEOJSON_FILE_SINK_H

#include <gnuradio/sync_block.h>
#include <gnuradio/gnss/api.h>

namespace gr {
  namespace gnss {

    /*!
     * \brief <+description of block+>
     * \ingroup gnss
     */
    struct GNSS_API geojson_file_sink : virtual public gr::sync_block
    {
      using sptr = std::shared_ptr<geojson_file_sink>;

      /*!
       * \brief Return a shared_ptr to a new instance of gnss::geojson_file_sink block.
       */
      static sptr make(const char* filename);
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_GEOJSON_FILE_SINK_H */
