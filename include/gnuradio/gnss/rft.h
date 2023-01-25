/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_RFT_H
#define INCLUDED_GNSS_RFT_H

#include <gnuradio/sync_block.h>
#include <gnuradio/gnss/api.h>
#include <gnuradio/gnss/rft_type.h>

namespace gr {
  namespace gnss {

    /*!
     * \brief <+description of block+>
     * \ingroup gnss
     */
    struct GNSS_API rft : virtual public gr::sync_block
    {
      using sptr = std::shared_ptr<rft>;

      /*!
       * \brief Return a shared_ptr to a new instance of gnss::rft block.
       */
      static sptr make();

      virtual void set_transformation(rft_type_e type) = 0;
      virtual void get_transformation(rft_type_e& type) const = 0;
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_RFT_H */
