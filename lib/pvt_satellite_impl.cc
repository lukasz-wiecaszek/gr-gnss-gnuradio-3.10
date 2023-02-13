/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>

#include "pvt_satellite_impl.h"
#include "tags.h"
#include "ports.h"
#include "vector3d.h"
#include "pvt_utils.h"

#define DECIMATION_FACTOR 5

namespace gr {
  namespace gnss {

    /*===========================================================================*\
    * public function definitions
    \*===========================================================================*/
    pvt_satellite::sptr
    pvt_satellite::make()
    {
      return gnuradio::make_block_sptr<pvt_satellite_impl<
        vector3d, // input
        vector3d, // output - position
        vector3d, // output - velocity
        double    // output - time
      >>();
    }


    template<typename ITYPE0, typename OTYPE0, typename OTYPE1, typename OTYPE2>
    pvt_satellite_impl<ITYPE0, OTYPE0, OTYPE1, OTYPE2>::pvt_satellite_impl()
      : gr::block("pvt_satellite",
                  gr::io_signature::make(1, 1, sizeof(ITYPE0) * IVLEN0),
                  gr::io_signature::make3(3, 3, sizeof(OTYPE0) * OVLEN0, sizeof(OTYPE1) * OVLEN1, sizeof(OTYPE2) * OVLEN2)),
        d_satellite_navigation_system{NAVIGATION_SYSTEM_UNDEFINED},
        d_satellite_id{-1},
        d_sv_clock_parameters{},
        d_ephemeris{}
    {
      set_relative_rate(1, DECIMATION_FACTOR);
      set_tag_propagation_policy(TPP_DONT);

      message_port_register_in(pmt::mp(PORT_CLOCK));
      set_msg_handler(pmt::mp(PORT_CLOCK), [this](const pmt::pmt_t& msg) {
        try {
          const std::shared_ptr<sv_clock_parameters> c = boost::any_cast<std::shared_ptr<sv_clock_parameters>>(pmt::any_ref(msg));
          printf("%s\n", c->to_string().c_str());
          if (c->svid >= 0) {
            gr::thread::scoped_lock lock(d_setlock);
            d_sv_clock_parameters = c;
          }
        }
        catch (boost::bad_any_cast &e) {
          printf("boost::bad_any_cast (%s)\n", e.what());
        }
      });

      message_port_register_in(pmt::mp(PORT_EPHEMERIS));
      set_msg_handler(pmt::mp(PORT_EPHEMERIS), [this](const pmt::pmt_t& msg) {
        try {
          const std::shared_ptr<ephemeris> e = boost::any_cast<std::shared_ptr<ephemeris>>(pmt::any_ref(msg));
          printf("%s\n", e->to_string().c_str());
          if (e->svid >= 0) {
            gr::thread::scoped_lock lock(d_setlock);
            d_ephemeris = e;
          }
        }
        catch (boost::bad_any_cast &e) {
          printf("boost::bad_any_cast (%s)\n", e.what());
        }
      });
    }

    template<typename ITYPE0, typename OTYPE0, typename OTYPE1, typename OTYPE2>
    pvt_satellite_impl<ITYPE0, OTYPE0, OTYPE1, OTYPE2>::~pvt_satellite_impl()
    {
    }

    template<typename ITYPE0, typename OTYPE0, typename OTYPE1, typename OTYPE2>
    void
    pvt_satellite_impl<ITYPE0, OTYPE0, OTYPE1, OTYPE2>::set_acq_params(navigation_system_e system, int id)
    {
      gr::thread::scoped_lock lock(d_setlock);
      d_satellite_navigation_system = system;
      d_satellite_id = id;
    }

    template<typename ITYPE0, typename OTYPE0, typename OTYPE1, typename OTYPE2>
    void
    pvt_satellite_impl<ITYPE0, OTYPE0, OTYPE1, OTYPE2>::get_acq_params(navigation_system_e& system, int& id) const
    {
      system = d_satellite_navigation_system;
      id = d_satellite_id;
    }

    template<typename ITYPE0, typename OTYPE0, typename OTYPE1, typename OTYPE2>
    void
    pvt_satellite_impl<ITYPE0, OTYPE0, OTYPE1, OTYPE2>::forecast(int noutput_items, gr_vector_int& ninput_items_required)
    {
      // 1 output item requires DECIMATION_FACTOR items at input
      int nrequired = noutput_items * DECIMATION_FACTOR;

      for (auto&& element : ninput_items_required)
        element = nrequired;
    }

    template<typename ITYPE0, typename OTYPE0, typename OTYPE1, typename OTYPE2>
    int
    pvt_satellite_impl<ITYPE0, OTYPE0, OTYPE1, OTYPE2>::general_work(
        int noutput_items,
        gr_vector_int& ninput_items,
        gr_vector_const_void_star& input_items,
        gr_vector_void_star& output_items)
    {
      int nproduced = 0;
      const ITYPE0* iptr0 = (const ITYPE0*) input_items[0];
      OTYPE0* optr0 = (OTYPE0*) output_items[0];
      OTYPE1* optr1 = (OTYPE1*) output_items[1];
      OTYPE2* optr2 = (OTYPE2*) output_items[2];

      gr::thread::scoped_lock lock(d_setlock);

      if (d_sv_clock_parameters == nullptr) {
        // we do not have clock parameters (subframe1) for this satelite
        consume_each(noutput_items * DECIMATION_FACTOR);
        return 0;
      }

      if (d_ephemeris == nullptr) {
        // we do not have ephemeris data (subframe2 and subframe3) for this satelite
        consume_each(noutput_items * DECIMATION_FACTOR);
        return 0;
      }

      pvt_utils::satelite satelite;
      while (nproduced < noutput_items) {
        double tx_time = iptr0[nproduced * DECIMATION_FACTOR].get({1}, {});
        d_ephemeris->get_vectors(tx_time, &satelite.position, &satelite.velocity, NULL);

        optr0[nproduced] = satelite.position;
        optr1[nproduced] = satelite.velocity;
        optr2[nproduced] = tx_time;
        add_item_tag(0, nitems_written(0), pmt::mp(TAG_TX_TIME), pmt::mp(tx_time), alias_pmt());

        nproduced++;
      }

      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each(nproduced * DECIMATION_FACTOR);

      // Tell runtime system how many output items we produced.
      return nproduced;
    }

  } /* namespace gnss */
} /* namespace gr */
