/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_NAV_MESSAGE_DECODER_H
#define INCLUDED_GNSS_NAV_MESSAGE_DECODER_H

#include <gnuradio/block.h>
#include <gnuradio/gnss/api.h>
#include <gnuradio/gnss/navigation_system.h>

namespace gr {
  namespace gnss {

    /*!
     * \brief <+description of block+>
     * \ingroup gnss
     */
    struct GNSS_API nav_message_decoder : virtual public gr::block
    {
      using sptr = std::shared_ptr<nav_message_decoder>;

      /*!
       * \brief Return a shared_ptr to a new instance of gnss::nav_message_decoder block.
       */
      static sptr make();

      /*!
       * \brief Sets acquisition parameters.
       */
      virtual void set_acq_params(navigation_system_e system, int id) = 0;

      /*!
       * \brief Gets acquisition parameters.
       */
      virtual void get_acq_params(navigation_system_e& system, int& id) const = 0;
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_NAV_MESSAGE_DECODER_H */
