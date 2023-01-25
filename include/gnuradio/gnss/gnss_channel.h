/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_GNSS_CHANNEL_H
#define INCLUDED_GNSS_GNSS_CHANNEL_H

#include <gnuradio/hier_block2.h>
#include <gnuradio/gnss/api.h>
#include <gnuradio/gnss/navigation_system.h>

namespace gr {
  namespace gnss {

    /*!
     * \brief <+description of block+>
     * \ingroup gnss
     */
    struct GNSS_API gnss_channel : virtual public gr::hier_block2
    {
      using sptr = std::shared_ptr<gnss_channel>;

      /*!
       * \brief Return a shared_ptr to a new instance of gnss::gnss_channel block.
       */
      static sptr make(double sampling_freq,
                       double dll_bw_coarse,
                       double pll_bw_coarse,
                       double dll_bw_fine,
                       double pll_bw_fine);

      virtual void set_acq_params(navigation_system_e system, int id) = 0;
      virtual void get_acq_params(navigation_system_e& system, int& id) const = 0;
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_GNSS_CHANNEL_H */
