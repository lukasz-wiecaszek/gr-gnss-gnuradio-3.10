/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "resampler_impl.h"

#define UINT32_MAX_AS_DOUBLE ((double)((uint32_t)-1))

namespace gr {
  namespace gnss {

    resampler::sptr
    resampler::make(double fs_in, double fs_out)
    {
      return gnuradio::make_block_sptr<resampler_impl<gr_complex, gr_complex>>(fs_in, fs_out);
    }

    template<typename ITYPE, typename OTYPE>
    resampler_impl<ITYPE, OTYPE>::resampler_impl(double fs_in, double fs_out)
      : gr::block("resampler",
                  gr::io_signature::make(1, 1, sizeof(ITYPE) * IVLEN),
                  gr::io_signature::make(1, 1, sizeof(OTYPE) * OVLEN)),
        d_fs_in{fs_in},
        d_fs_out{fs_out},
        d_phase_step{d_fs_out < d_fs_in ?
          static_cast<uint32_t>(UINT32_MAX_AS_DOUBLE * d_fs_out / d_fs_in) :
          static_cast<uint32_t>(UINT32_MAX_AS_DOUBLE * d_fs_in / d_fs_out)},
        d_phase{0},
        d_lphase{0}
    {
      set_relative_rate(d_fs_out / d_fs_in);
    }

    template<typename ITYPE, typename OTYPE>
    resampler_impl<ITYPE, OTYPE>::~resampler_impl()
    {
    }

    template<typename ITYPE, typename OTYPE>
    void
    resampler_impl<ITYPE, OTYPE>::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      int nrequired = noutput_items * d_fs_in / d_fs_out;

      for (auto&& element : ninput_items_required)
        element = nrequired;
    }

    template<typename ITYPE, typename OTYPE>
    int
    resampler_impl<ITYPE, OTYPE>::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const ITYPE* iptr0 = (const ITYPE*) input_items[0];
      OTYPE* optr0 = (OTYPE*) output_items[0];
      int nproduced = 0;
      int nconsumed = 0;

      if (d_fs_out < d_fs_in) { // decimation
        while ((nproduced < noutput_items) && (nconsumed < ninput_items[0])) {
          if (d_phase <= d_lphase)
            optr0[nproduced++] = *iptr0;
          d_lphase = d_phase;
          d_phase += d_phase_step;
          iptr0++;
          nconsumed++;
        }
      }
      else { // interpolation
        while ((nproduced < noutput_items) && (nconsumed < ninput_items[0])) {
          d_lphase = d_phase;
          d_phase += d_phase_step;
          if (d_phase <= d_lphase) {
            iptr0++;
            nconsumed++;
          }
          optr0[nproduced++] = *iptr0;
        }
      }

      // Tell runtime system how many input items we consumed on
      // each input stream.
      this->consume_each(nconsumed);

      // Tell runtime system how many output items we produced.
      return nproduced;
    }

  } /* namespace gnss */
} /* namespace gr */
