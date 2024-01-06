/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#include <gnuradio/io_signature.h>

#include "pseudoranges_decoder_impl.h"
#include "tags.h"
#include "ports.h"
#include "vector3d.h"
#include "gnss_parameters.h"
#include "sv_clock_parameters.h"
#include "ephemeris.h"
#include "pvt_utils.h"

#define DECIMATION_FACTOR 5

namespace gr {
  namespace gnss {

    /*===========================================================================*\
    * public function definitions
    \*===========================================================================*/
    pseudoranges_decoder::sptr
    pseudoranges_decoder::make(bool add_velocity_outputs)
    {
      return gnuradio::make_block_sptr<pseudoranges_decoder_impl<vector3d, vector3d>>(add_velocity_outputs);
    }

    template<typename ITYPE, typename OTYPE>
    pseudoranges_decoder_impl<ITYPE, OTYPE>::pseudoranges_decoder_impl(bool add_velocity_outputs)
      : gr::block("pseudoranges_decoder",
                  gr::io_signature::make(1, MAX_STREAMS, sizeof(ITYPE) * IVLEN),
                  gr::io_signature::make(1, MAX_STREAMS * (add_velocity_outputs ? 2 : 1), sizeof(OTYPE) * OVLEN)),
        d_add_velocity_outputs{add_velocity_outputs},
        d_satellite_ids(),
        d_flatbuffers(),
        d_sv_clock_parameters{},
        d_ephemerides{}
    {
      set_relative_rate(1, DECIMATION_FACTOR);
      set_tag_propagation_policy(TPP_DONT);

      for (int i = 0; i < MAX_STREAMS; ++i) {
        d_satellite_ids[i] = {NAVIGATION_SYSTEM_UNDEFINED, -1};
        d_flatbuffers[i] = std::move(lts::flatbuffer<vector3d>{2000});
      }

      message_port_register_in(pmt::mp(PORT_CLOCK));
      set_msg_handler(pmt::mp(PORT_CLOCK), [this](const pmt::pmt_t& msg) {
        try {
          const std::shared_ptr<sv_clock_parameters> c = boost::any_cast<std::shared_ptr<sv_clock_parameters>>(pmt::any_ref(msg));
          printf("%s\n", c->to_string().c_str());
          if (c->svid >= 0) {
            gr::thread::scoped_lock lock(d_setlock);
            d_sv_clock_parameters[c->svid] = c;
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
            d_ephemerides[e->svid] = e;
          }
        }
        catch (boost::bad_any_cast &e) {
          printf("boost::bad_any_cast (%s)\n", e.what());
        }
      });
    }

    template<typename ITYPE, typename OTYPE>
    pseudoranges_decoder_impl<ITYPE, OTYPE>::~pseudoranges_decoder_impl()
    {
    }

    template<typename ITYPE, typename OTYPE>
    void
    pseudoranges_decoder_impl<ITYPE, OTYPE>::set_acq_params(int port, navigation_system_e system, int id)
    {
      gr::thread::scoped_lock lock(d_setlock);
      d_satellite_ids[port] = {system, id};
    }

    template<typename ITYPE, typename OTYPE>
    void
    pseudoranges_decoder_impl<ITYPE, OTYPE>::get_acq_params(int port, navigation_system_e& system, int& id) const
    {
      system = std::get<0>(d_satellite_ids[port]);
      id = std::get<1>(d_satellite_ids[port]);
    }

    template<typename ITYPE, typename OTYPE>
    void
    pseudoranges_decoder_impl<ITYPE, OTYPE>::forecast(int noutput_items, gr_vector_int &ninput_items_required)
    {
      // 1 output item requires DECIMATION_FACTOR items at input
      int nrequired = noutput_items * DECIMATION_FACTOR;

      for (auto&& element : ninput_items_required)
        element = nrequired;
    }

    template<typename ITYPE, typename OTYPE>
    int
    pseudoranges_decoder_impl<ITYPE, OTYPE>::general_work(int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      int n;
      const int N = ninput_items.size();
      int nproduced = 0;

      if (N < 4)
        throw std::out_of_range("invalid number of input pads (at least 4 required)");

      if (d_add_velocity_outputs) {
        if (2 * input_items.size() != output_items.size())
          throw std::length_error("block shall have two times more output pads than input pads");
      }
      else {
        if (input_items.size() != output_items.size())
          throw std::length_error("block shall have the same number of input and output pads");
      }

      // Tell runtime system how many input items we consumed on
      // each input stream.
      for (n = 0; n < N; ++n) {
        if (static_cast<std::size_t>(ninput_items[n]) > d_flatbuffers[n].write_available())
          d_flatbuffers[n].move();
        consume(n, d_flatbuffers[n].write(static_cast<const ITYPE*>(input_items[n]), ninput_items[n]));
      }

      gr::thread::scoped_lock lock(d_setlock);

      while (nproduced < noutput_items) {

        double rx_time = get_rx_time(N);
        if (rx_time == std::numeric_limits<double>::infinity())
          return 0;

        double tx_times[N];
        for (n = 0; n < N; ++n) {
          tx_times[n] = get_tx_time(n, rx_time);
          if (tx_times[n] == std::numeric_limits<double>::infinity())
            return 0;
        }

        for (n = 0; n < N; ++n)
          d_flatbuffers[n].consume(std::min(d_flatbuffers[n].read_available(), static_cast<std::size_t>(DECIMATION_FACTOR)));

        pvt_utils::satellite satellites[N];
        for (n = 0; n < N; ++n) {
          int svid = std::get<1>(d_satellite_ids[n]);
          if (svid >= 0) {
            const std::shared_ptr<sv_clock_parameters> c = d_sv_clock_parameters[svid];
            if (c == nullptr) // we do not have clock parameters (subframe1) for this satellite
              continue;

            const std::shared_ptr<ephemeris> e = d_ephemerides[svid];
            if (e == nullptr) // we do not have ephemeris data (subframe2 and subframe3) for this satellite
              continue;

            double tx_time = tx_times[n];
            double dt1 = c->code_phase_offset(tx_time);
            double dt2 = e->relativistic_correction_term(tx_time);
            double dt = dt1 + dt2;
            tx_time -= dt;

            satellites[n].pseudorange = (rx_time - tx_time) * C;
            if (d_add_velocity_outputs)
              e->get_vectors(tx_time, &satellites[n].position, &satellites[n].velocity, NULL);
            else
              e->get_vectors(tx_time, &satellites[n].position, NULL, NULL);
          }
        }

        OTYPE* optr;
        for (n = 0; n < N; ++n) {
          optr = static_cast<OTYPE*>(output_items[n]);
          optr[nproduced] = satellites[n].position;
          if (d_add_velocity_outputs) {
            optr = static_cast<OTYPE*>(output_items[N + n]);
            optr[nproduced] = satellites[n].velocity;
          }
          add_item_tag(n, nitems_written(n) + nproduced, pmt::mp(TAG_PSEUDORANGE), pmt::mp(satellites[n].pseudorange), alias_pmt());
          add_item_tag(n, nitems_written(n) + nproduced, pmt::mp(TAG_RX_TIME), pmt::mp(rx_time), alias_pmt());
        }

        nproduced++;
      }

      // Tell runtime system how many output items we produced.
      return nproduced;
    }

    /*===========================================================================*\
    * protected function definitions
    \*===========================================================================*/

    /*===========================================================================*\
    * private function definitions
    \*===========================================================================*/
    template<typename ITYPE, typename OTYPE>
    double
    pseudoranges_decoder_impl<ITYPE, OTYPE>::get_rx_time(const int N)
    {
      int n;
      double rx_times[N];

      for (n = 0; (n < N) && (d_flatbuffers[n].read_available() > 0); ++n)
        rx_times[n] = d_flatbuffers[n].read_ptr()->get({0}, {});

      if (n < N)
        return std::numeric_limits<double>::infinity();

      auto it = std::max_element(rx_times, rx_times + N);
      int max_element_idx = std::distance(rx_times, it);
      double max_element = *it;

      for (n = 0; n < N; ++n) {
        if (n != max_element_idx) {
          int count = 0;
          const vector3d* v = d_flatbuffers[n].read_ptr();
          for (std::size_t m = 0; m < d_flatbuffers[n].read_available(); ++m)
            if (v[m].get({0}, {}) <= max_element)
              count++;
          if (count > 0)
            d_flatbuffers[n].consume(count - 1);
          else
            return printf("This should not happen\n"),
              std::numeric_limits<double>::infinity();
        }
      }

      return max_element;
    }

    template<typename ITYPE, typename OTYPE>
    double
    pseudoranges_decoder_impl<ITYPE, OTYPE>::get_tx_time(const int n, double rx_time)
    {
      double tx_time;

      if (d_flatbuffers[n].read_available() > 1) {
        const vector3d* v = d_flatbuffers[n].read_ptr();
        double rx_time_0 = v[0].get({0}, {});
        double rx_time_1 = v[1].get({0}, {});

        //printf("%d: %+.15e -> %+.15e -> %+.15e (%+.15e : %+.15e)\n",
        //  n, rx_time_0, rx_time, rx_time_1, rx_time - rx_time_0, rx_time_1 - rx_time);

        if ((rx_time - rx_time_0) < 0)
          return std::numeric_limits<double>::infinity();

        if ((rx_time_1 - rx_time) < 0)
          return std::numeric_limits<double>::infinity();

        double dt = (rx_time - rx_time_0) / (rx_time_1 - rx_time_0);
        double tx_time_0 = v[0].get({1}, {});
        double tx_time_1 = v[1].get({1}, {});

        // This is simple linear interpolation
        if (tx_time_1 > tx_time_0)
          tx_time = tx_time_0 + (tx_time_1 - tx_time_0) * dt;
        else
          // This is a situation when we cross gsp week boundary
          tx_time = tx_time_0 + (tx_time_1 + GPS_SECONDS_PER_WEEK - tx_time_0) * dt;

        // printf("%d: %+.15e -> %+.15e\n", n, rx_time, tx_time);

      } else {
        tx_time = std::numeric_limits<double>::infinity();
      }

      return tx_time;
    }

  } /* namespace gnss */
} /* namespace gr */
