/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_RESAMPLER_H
#define INCLUDED_GNSS_RESAMPLER_H

#include <gnuradio/block.h>
#include <gnuradio/gnss/api.h>

namespace gr {
  namespace gnss {

    /*!
     * \brief <+description of block+>
     * \ingroup gnss
     */
    struct GNSS_API resampler : virtual public gr::block
    {
      using sptr = std::shared_ptr<resampler>;

      /*!
       * \brief Return a shared_ptr to a new instance of gnss::resampler block.
       */
      static sptr make(double fs_in, double fs_out);
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_RESAMPLER_H */
