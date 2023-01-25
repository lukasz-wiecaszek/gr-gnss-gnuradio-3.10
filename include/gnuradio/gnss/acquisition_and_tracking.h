/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_ACQUISITION_AND_TRACKING_H
#define INCLUDED_GNSS_ACQUISITION_AND_TRACKING_H

#include <gnuradio/block.h>
#include <gnuradio/gnss/api.h>
#include <gnuradio/gnss/navigation_system.h>

namespace gr {
  namespace gnss {

    /*!
     * \brief <+description of block+>
     * \ingroup gnss
     */
    struct GNSS_API acquisition_and_tracking : virtual public gr::block
    {
      using sptr = std::shared_ptr<acquisition_and_tracking>;

      /*!
       * \brief Return a shared_ptr to a new instance of gnss::acquisition_and_tracking block.
       */
      static sptr make(double sampling_freq,
                       double dll_bw_coarse,
                       double pll_bw_coarse,
                       double dll_bw_fine,
                       double pll_bw_fine);

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

#endif /* INCLUDED_GNSS_ACQUISITION_AND_TRACKING_H */
