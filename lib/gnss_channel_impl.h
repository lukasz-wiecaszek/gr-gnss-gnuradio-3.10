/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_GNSS_CHANNEL_IMPL_H
#define INCLUDED_GNSS_GNSS_CHANNEL_IMPL_H

#include <gnuradio/gnss/gnss_channel.h>

#include <gnuradio/gnss/acquisition_and_tracking.h>
#include <gnuradio/gnss/ca_symbols_to_nav_bits.h>
#include <gnuradio/gnss/nav_message_decoder.h>

#define IVLEN0 1
#define OVLEN0 1
#define OVLEN1 1

namespace gr {
  namespace gnss {

    template<typename ITYPE0, typename OTYPE0, typename OTYPE1>
    class gnss_channel_impl : public gnss_channel
    {
    public:
      gnss_channel_impl(double sampling_freq,
                        double dll_bw_coarse,
                        double pll_bw_coarse,
                        double dll_bw_fine,
                        double pll_bw_fine);
      ~gnss_channel_impl();

      void set_acq_params(navigation_system_e system, int id) override;
      void get_acq_params(navigation_system_e& system, int& id) const override;

    private:
      gr::gnss::acquisition_and_tracking::sptr d_acquisition_and_tracking;
      gr::gnss::ca_symbols_to_nav_bits::sptr d_ca_symbols_to_nav_bits;
      gr::gnss::nav_message_decoder::sptr d_nav_message_decoder;
      navigation_system_e d_navigation_system;
      int d_id;
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_GNSS_CHANNEL_IMPL_H */
