/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_PVT_SATELLITE_IMPL_H
#define INCLUDED_GNSS_PVT_SATELLITE_IMPL_H

#include <map>

#include "sv_clock_parameters.h"
#include "ephemeris.h"

#include <gnuradio/gnss/pvt_satellite.h>

#define IVLEN0 1 // input
#define OVLEN0 1 // output - position
#define OVLEN1 1 // output - velocity
#define OVLEN2 1 // output - time

namespace gr {
  namespace gnss {

    template<typename ITYPE0, typename OTYPE0, typename OTYPE1, typename OTYPE2>
    class pvt_satellite_impl : public pvt_satellite
    {
    public:
      pvt_satellite_impl();
      ~pvt_satellite_impl();

      void set_acq_params(navigation_system_e system, int id) override;
      void get_acq_params(navigation_system_e& system, int& id) const override;

      void forecast(int noutput_items, gr_vector_int& ninput_items_required);

      int general_work(int noutput_items,
                      gr_vector_int& ninput_items,
                      gr_vector_const_void_star& input_items,
                      gr_vector_void_star& output_items);

    private:
      navigation_system_e d_satellite_navigation_system;
      int d_satellite_id;
      std::shared_ptr<sv_clock_parameters> d_sv_clock_parameters;
      std::shared_ptr<ephemeris> d_ephemeris;
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_PVT_SATELLITE_IMPL_H */
