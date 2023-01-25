/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_SKIP_ZEROS_H
#define INCLUDED_GNSS_SKIP_ZEROS_H

#include <gnuradio/sync_block.h>
#include <gnuradio/gnss/api.h>

namespace gr {
  namespace gnss {

    /*!
     * \brief <+description of block+>
     * \ingroup gnss
     */
    struct GNSS_API skip_zeros : virtual public gr::sync_block
    {
      using sptr = std::shared_ptr<skip_zeros>;

      /*!
       * \brief Return a shared_ptr to a new instance of gnss::skip_zeros block.
       */
      static sptr make();
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_SKIP_ZEROS_H */
