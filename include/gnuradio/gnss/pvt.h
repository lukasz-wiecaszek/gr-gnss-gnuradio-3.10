/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_PVT_H
#define INCLUDED_GNSS_PVT_H

#include <gnuradio/block.h>
#include <gnuradio/gnss/api.h>

namespace gr {
  namespace gnss {

    /*!
     * \brief <+description of block+>
     * \ingroup gnss
     */
    struct GNSS_API pvt : virtual public gr::block
    {
      using sptr = std::shared_ptr<pvt>;

      /*!
       * \brief Return a shared_ptr to a new instance of gnss::pvt block.
       */
      static sptr make(bool skip_zeros = true);
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_PVT_H */
