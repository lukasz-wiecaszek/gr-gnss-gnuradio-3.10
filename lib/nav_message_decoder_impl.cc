/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "nav_message_decoder_impl.h"
#include <gnuradio/io_signature.h>
#include <time.h>
#include <chrono>

#include "nav_message_decoder_impl.h"
#include "tags.h"
#include "ports.h"

namespace gr {
  namespace gnss {

    /*===========================================================================*\
    * public function definitions
    \*===========================================================================*/
    nav_message_decoder::sptr
    nav_message_decoder::make()
    {
      return gnuradio::make_block_sptr<nav_message_decoder_impl<uint8_t, vector3d>>();
    }

    template<typename ITYPE, typename OTYPE>
    nav_message_decoder_impl<ITYPE, OTYPE>::nav_message_decoder_impl()
      : gr::block("nav_message_decoder",
                  gr::io_signature::make(1, 1, sizeof(ITYPE) * IVLEN),
                  gr::io_signature::make(1, 1, sizeof(OTYPE) * OVLEN)),
        d_navigation_system{NAVIGATION_SYSTEM_UNDEFINED},
        d_id{-1},
        d_fp{NULL},
        d_subframe_data{},
        d_subframe_data_cnt{0},
        d_subframe{},
        d_tx_time{std::numeric_limits<double>::infinity()},
        d_sv_clock_parameters{},
        d_ephemeris{},
        d_current_ephemeris_idx{-1}
    {
      set_tag_propagation_policy(TPP_DONT);

      message_port_register_out(pmt::mp(PORT_CLOCK));
      message_port_register_out(pmt::mp(PORT_EPHEMERIS));
    }

    template<typename ITYPE, typename OTYPE>
    nav_message_decoder_impl<ITYPE, OTYPE>::~nav_message_decoder_impl()
    {
      if (d_fp != NULL)
        fclose(d_fp);
    }

    template<typename ITYPE, typename OTYPE>
    void
    nav_message_decoder_impl<ITYPE, OTYPE>::set_acq_params(navigation_system_e system, int id)
    {
      gr::thread::scoped_lock lock(d_setlock);

      d_navigation_system = system;
      d_id = id;

      if (d_id > 0) {
        struct tm tm;
        char date_time_buf[128] = {};
        char date_time_filename_buf[256];

        auto current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        if (::gmtime_r(&current_time, &tm))
          ::strftime(date_time_buf, sizeof(date_time_buf), "%F_%H%M%S", &tm);

        snprintf(date_time_filename_buf, sizeof(date_time_filename_buf), "%s_subframes_svid_%d.dat", date_time_buf, d_id);

        if (d_fp != NULL)
          fclose(d_fp);

        d_fp = fopen(date_time_filename_buf, "w");
      }
    }

    template<typename ITYPE, typename OTYPE>
    void
    nav_message_decoder_impl<ITYPE, OTYPE>::get_acq_params(navigation_system_e& system, int& id) const
    {
      system = d_navigation_system;
      id = d_id;
    }

    template<typename ITYPE, typename OTYPE>
    void
    nav_message_decoder_impl<ITYPE, OTYPE>::forecast(int noutput_items, gr_vector_int &ninput_items_required)
    {
      // 1 output item requires 1 input item
      int nrequired = noutput_items * 1;

      for (auto&& element : ninput_items_required)
        element = nrequired;
    }

    template<typename ITYPE, typename OTYPE>
    int
    nav_message_decoder_impl<ITYPE, OTYPE>::general_work(int noutput_items,
                                                         gr_vector_int &ninput_items,
                                                         gr_vector_const_void_star &input_items,
                                                         gr_vector_void_star &output_items)
    {
      int nproduced = 0;
      const ITYPE* iptr0 = (const ITYPE*) input_items[0];
      OTYPE* optr0 = (OTYPE*) output_items[0];

      gr::thread::scoped_lock lock(d_setlock);

      std::vector<tag_t> subframe_bit_tags;
      get_tags_in_range(subframe_bit_tags, 0, nitems_read(0), nitems_read(0) + noutput_items, pmt::mp(TAG_SUBFRAME_BIT));

      std::vector<tag_t> rx_time_tags;
      get_tags_in_range(rx_time_tags, 0, nitems_read(0), nitems_read(0) + noutput_items, pmt::mp(TAG_RX_TIME));

      for (int i = 0; i < noutput_items; ++i) {
        int n = pmt::to_long(subframe_bit_tags[i].value);
        double rx_time = pmt::to_double(rx_time_tags[i].value);

        if (d_subframe) {
          double tx_time = (d_subframe.tow_count_message() * 6) + n * 0.02;
          optr0[nproduced++] = vector3d{{rx_time, tx_time, 0.0}};
        } else {
          if (d_tx_time != std::numeric_limits<double>::infinity()) {
            double tx_time = d_tx_time;
            optr0[nproduced++] = vector3d{{rx_time, tx_time, 0.0}};
            d_tx_time += 0.02;
          }
        }

        if (d_subframe_data_cnt == n) {
          d_subframe_data[n] = iptr0[i];
          d_subframe_data_cnt++;
        }
        else
          d_subframe_data_cnt = 0;

        if (d_subframe_data_cnt == GPS_NAV_MESSAGE_BITS_PER_SUBFRAME) {
          const ITYPE *p = &d_subframe_data[0];
          d_subframe.init(&p);
          if (d_subframe) {
            d_tx_time = d_subframe.tow_count_message() * 6;
            process_subframe(d_subframe);
            if (d_fp)
              fprintf(d_fp, "%s\n", d_subframe.to_string().c_str());
            printf("svid %d: new (subframe %u) received\n", d_id, d_subframe.subframe_id());
          }
          else {
            printf("svid %d: new (corrupted) subframe received\n", d_id);
          }

          d_subframe_data_cnt = 0;
        }
      }

      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each(noutput_items);

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
    void
    nav_message_decoder_impl<ITYPE, OTYPE>::init_sv_clock_parameters(sv_clock_parameters& c, const gps_nav_message_subframe_1& subframe_1)
    {
      c.IODC        = subframe_1.IODC();
      c.week_no     = subframe_1.WN();
      c.SV_accuracy = subframe_1.URA_INDEX();
      c.SV_health   = subframe_1.SV_HEALTH();
      c.T_GD        = subframe_1.T_GD() * GPS_SCALE_FACTOR_T_GD;
      c.t_oc        = subframe_1.t_oc() * GPS_SCALE_FACTOR_T_OC;
      c.a_f0        = subframe_1.a_f0() * GPS_SCALE_FACTOR_A_F0;
      c.a_f1        = subframe_1.a_f1() * GPS_SCALE_FACTOR_A_F1;
      c.a_f2        = subframe_1.a_f2() * GPS_SCALE_FACTOR_A_F2;
    }

    template<typename ITYPE, typename OTYPE>
    void
    nav_message_decoder_impl<ITYPE, OTYPE>::init_ephemeris(ephemeris& e, const gps_nav_message_subframe_2& subframe_2)
    {
      e.IODE[0]               = subframe_2.IODE();
      e.correction_terms.C_RS = subframe_2.C_RS()       * GPS_SCALE_FACTOR_C_R;
      e.delta_n               = subframe_2.DELTA_N()    * GPS_SCALE_FACTOR_DELTA_N * M_PI;
      e.M_0                   = subframe_2.M_0()        * GPS_SCALE_FACTOR_M_0 * M_PI;
      e.correction_terms.C_UC = subframe_2.C_UC()       * GPS_SCALE_FACTOR_C_U;
      e.e                     = subframe_2.e()          * GPS_SCALE_FACTOR_E;
      e.correction_terms.C_US = subframe_2.C_US()       * GPS_SCALE_FACTOR_C_U;
      e.sqrt_a                = subframe_2.SQRT_A()     * GPS_SCALE_FACTOR_SQRT_A;
      e.t_oe                  = subframe_2.t_oe()       * GPS_SCALE_FACTOR_T_OE;
    }

    template<typename ITYPE, typename OTYPE>
    void
    nav_message_decoder_impl<ITYPE, OTYPE>::init_ephemeris(ephemeris& e, const gps_nav_message_subframe_3& subframe_3)
    {
      e.IODE[1]               = subframe_3.IODE();
      e.correction_terms.C_IC = subframe_3.C_IC()       * GPS_SCALE_FACTOR_C_I;
      e.OMEGA_0               = subframe_3.OMEGA_0()    * GPS_SCALE_FACTOR_OMEGA_0 * M_PI;
      e.correction_terms.C_IS = subframe_3.C_IS()       * GPS_SCALE_FACTOR_C_I;
      e.i_0                   = subframe_3.i_0()        * GPS_SCALE_FACTOR_I_0 * M_PI;
      e.correction_terms.C_RC = subframe_3.C_RC()       * GPS_SCALE_FACTOR_C_R;
      e.omega                 = subframe_3.omega()      * GPS_SCALE_FACTOR_OMEGA * M_PI;
      e.dOMEGA_dt             = subframe_3.dOMEGA_dt()  * GPS_SCALE_FACTOR_D_OMEGA_DT * M_PI;
      e.dI_dt                 = subframe_3.dI_dt()      * GPS_SCALE_FACTOR_IDOT * M_PI;
    }

    template<typename ITYPE, typename OTYPE>
    void
    nav_message_decoder_impl<ITYPE, OTYPE>::process_subframe(const gps_nav_message_subframe& subframe)
    {
      unsigned int subframe_id = subframe.subframe_id();

      if (subframe_id == 1) {
        gps_nav_message_subframe_1 subframe_1 = subframe;
        if ((d_sv_clock_parameters.IODC == -1) || (d_sv_clock_parameters.IODC != subframe_1.IODC())) {
          init_sv_clock_parameters(d_sv_clock_parameters, subframe_1);
          d_sv_clock_parameters.svid = d_id;
          message_port_pub(pmt::mp(PORT_CLOCK),
            pmt::make_any(std::make_shared<sv_clock_parameters>(d_sv_clock_parameters)));
          if (d_fp)
            fprintf(d_fp, "%s\n", d_sv_clock_parameters.to_string().c_str());
        }
      } else
      if ((subframe_id == 2) || (subframe_id == 3)) {
        if (d_current_ephemeris_idx < 0) {
          ephemeris& e = d_ephemeris[0];

          if (subframe_id == 2) {
            gps_nav_message_subframe_2 subframe_2 = subframe;
            init_ephemeris(e, subframe_2);
          }

          if (subframe_id == 3) {
            gps_nav_message_subframe_3 subframe_3 = subframe;
            init_ephemeris(e, subframe_3);
          }

          if ((e.IODE[0] != -1) && (e.IODE[0] == e.IODE[1])) {
            d_current_ephemeris_idx = 0;
            d_ephemeris[d_current_ephemeris_idx].svid = d_id;
            message_port_pub(pmt::mp(PORT_EPHEMERIS),
              pmt::make_any(std::make_shared<ephemeris>(d_ephemeris[d_current_ephemeris_idx])));
            if (d_fp)
              fprintf(d_fp, "%s\n", e.to_string().c_str());
          }
        }
        else {
          int complementary_ephemeris_idx = (d_current_ephemeris_idx == 0) ? 1 : 0;
          ephemeris e1 = d_ephemeris[d_current_ephemeris_idx];
          ephemeris e2 = d_ephemeris[complementary_ephemeris_idx];

          if (subframe_id == 2) {
            gps_nav_message_subframe_2 subframe_2 = subframe;
            if (e1.IODE[0] != subframe_2.IODE()) {
              init_ephemeris(e2, subframe_2);
            }
          }

          if (subframe_id == 3) {
            gps_nav_message_subframe_3 subframe_3 = subframe;
            if (e1.IODE[1] != subframe_3.IODE()) {
              init_ephemeris(e2, subframe_3);
            }
          }

          if ((e2.IODE[0] != -1) && (e2.IODE[0] == e2.IODE[1])) {
            e1.IODE[0] = e1.IODE[1] = -1;
            d_current_ephemeris_idx = complementary_ephemeris_idx;
            d_ephemeris[d_current_ephemeris_idx].svid = d_id;
            message_port_pub(pmt::mp(PORT_EPHEMERIS),
              pmt::make_any(std::make_shared<ephemeris>(d_ephemeris[d_current_ephemeris_idx])));
            if (d_fp)
              fprintf(d_fp, "%s\n", e2.to_string().c_str());
          }
        }
      } else {
        /* currently not handled */
      }
    }

  } /* namespace gnss */
} /* namespace gr */
