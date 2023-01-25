/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_CA_SYMBOLS_TO_NAV_BITS_H
#define INCLUDED_GNSS_CA_SYMBOLS_TO_NAV_BITS_H

#include <gnuradio/block.h>
#include <gnuradio/gnss/api.h>

namespace gr {
  namespace gnss {

    /*!
     * \brief <+description of block+>
     * \ingroup gnss
     */
    struct GNSS_API ca_symbols_to_nav_bits : virtual public gr::block
    {
      using sptr = std::shared_ptr<ca_symbols_to_nav_bits>;

      /*!
       * \brief Return a shared_ptr to a new instance of gnss::ca_symbols_to_nav_bits block.
       */
      static sptr make();
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_CA_SYMBOLS_TO_NAV_BITS_H */
