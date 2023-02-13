/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>

#include "doppler_shift_impl.h"
#include "gnss_parameters.h"
#include "vector3d.h"
#include "tags.h"

namespace gr {
  namespace gnss {

    /*===========================================================================*\
    * public function definitions
    \*===========================================================================*/
    doppler_shift::sptr
    doppler_shift::make(double x, double y, double z)
    {
      return gnuradio::make_block_sptr<doppler_shift_impl<vector3d, double>>(x, y, z);
    }

    template<typename ITYPE, typename OTYPE>
    doppler_shift_impl<ITYPE, OTYPE>::doppler_shift_impl(double x, double y, double z)
      : gr::sync_block("doppler_shift",
                       gr::io_signature::make(1, 2, sizeof(ITYPE) * IVLEN),
                       gr::io_signature::make(1, 1, sizeof(OTYPE) * OVLEN)),
        d_receiver_position{{x, y, z}},
        d_t{0.0},
        d_r{{0.0, 0.0, 0.0}}
    {
      printf("doppler_shift: receiver_position: %s\n", to_string(d_receiver_position).c_str());
    }

    template<typename ITYPE, typename OTYPE>
    doppler_shift_impl<ITYPE, OTYPE>::~doppler_shift_impl()
    {
    }

    template<typename ITYPE, typename OTYPE>
    int
    doppler_shift_impl<ITYPE, OTYPE>::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const ITYPE *iptr0 = (const ITYPE *) input_items[0];
      const ITYPE *iptr1 = nullptr;
      OTYPE* optr0 = (OTYPE*) output_items[0];
      int nproduced = 0;
      bool skip;

      if (input_items.size() > 1)
        iptr1 = (const ITYPE *) input_items[1];

      std::vector<tag_t> tags;
      get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0) + noutput_items, pmt::mp(TAG_TX_TIME));

      for (int n = 0; n < noutput_items; ++n) {
        skip = false;
        if (is_null(iptr0[n])) {
          consume(0, 1);
          skip = true; // we do not have valid satellite position yet
        }
        if (iptr1 && is_null(iptr1[n])) {
          consume(1, 1);
          skip = true; // we do not have valid receiver position yet
        }
        if (skip)
          continue;

        double t = pmt::to_double(tags[n].value);
        ITYPE r;
        if (iptr1)
          r = iptr0[n] - iptr1[n];
        else
          r = iptr0[n] - d_receiver_position;

        if ((d_t != 0.0) && (!is_null(d_r))) {
          double dt = t - d_t;
          ITYPE dr = r - d_r;
          ITYPE v = (dr / dt);
          ITYPE er = r / abs(r);

          double v_radial = lts::transpose(v) * er;
          double df = GPS_L1_FREQ_HZ * (std::sqrt((C - v_radial) / (C + v_radial)) - 1.0);

          //printf("r: %s\n", to_string(r).c_str());
          //printf("dr: %s, dt: %.15e\n", to_string(dr).c_str(), dt);
          //printf("v: %s, v_radial: %.15e, df: %.15e\n", to_string(v).c_str(), v_radial, df);

          optr0[nproduced++] = df;
        }
        else
          consume_each(1);

        d_t = t;
        d_r = r;
      }

      // Tell runtime system how many output items we produced.
      return nproduced;
    }

  } /* namespace gnss */
} /* namespace gr */
