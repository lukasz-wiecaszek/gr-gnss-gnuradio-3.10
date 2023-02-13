/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_PVT_SATELLITE_H
#define INCLUDED_GNSS_PVT_SATELLITE_H

#include <gnuradio/block.h>
#include <gnuradio/gnss/api.h>
#include <gnuradio/gnss/navigation_system.h>

namespace gr {
  namespace gnss {

    /*!
     * \brief This block produces ECEF positions and velocities of a satellite at times
     * as received in that satellite navigation messages.
     * Clock and Ephemeris data from 'Nav Message Decoder' block need to be connected
     * to respective 'Clock' and 'Ephemeris' message ports of this block.
     *
     * \ingroup gnss
     */
    struct GNSS_API pvt_satellite : virtual public gr::block
    {
      using sptr = std::shared_ptr<pvt_satellite>;

      /*!
       * \brief Return a shared_ptr to a new instance of gnss::pvt_satellite block.
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

#endif /* INCLUDED_GNSS_PVT_SATELLITE_H */
