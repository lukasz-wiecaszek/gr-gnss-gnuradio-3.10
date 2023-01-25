/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_DOPPLER_SHIFT_H
#define INCLUDED_GNSS_DOPPLER_SHIFT_H

#include <gnuradio/sync_block.h>
#include <gnuradio/gnss/api.h>

namespace gr {
  namespace gnss {

    /*!
     * \brief <+description of block+>
     * \ingroup gnss
     */
    struct GNSS_API doppler_shift : virtual public gr::sync_block
    {
      using sptr = std::shared_ptr<doppler_shift>;

      /*!
       * \brief Return a shared_ptr to a new instance of gnss::doppler_shift block.
       */
      static sptr make(double x = 0.0, double y = 0.0, double z = 0.0);
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_DOPPLER_SHIFT_H */
