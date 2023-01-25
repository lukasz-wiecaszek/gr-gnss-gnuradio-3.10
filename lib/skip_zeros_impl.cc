/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#include <gnuradio/io_signature.h>
#include "skip_zeros_impl.h"
#include "vector3d.h"

namespace gr {
  namespace gnss {

    /*===========================================================================*\
    * public function definitions
    \*===========================================================================*/
    skip_zeros::sptr
    skip_zeros::make()
    {
      return gnuradio::make_block_sptr<skip_zeros_impl<vector3d, vector3d>>();
    }

    template<typename ITYPE, typename OTYPE>
    skip_zeros_impl<ITYPE, OTYPE>::skip_zeros_impl()
      : gr::sync_block("skip_zeros",
                       gr::io_signature::make(1, 1, sizeof(ITYPE) * IVLEN),
                       gr::io_signature::make(1, 1, sizeof(OTYPE) * OVLEN))
    {
    }

    template<typename ITYPE, typename OTYPE>
    skip_zeros_impl<ITYPE, OTYPE>::~skip_zeros_impl()
    {
    }

    template<typename ITYPE, typename OTYPE>
    int
    skip_zeros_impl<ITYPE, OTYPE>::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const ITYPE *iptr0 = (const ITYPE *) input_items[0];
      OTYPE* optr0 = (OTYPE*) output_items[0];
      int nproduced = 0;

      for (int n = 0; n < noutput_items; ++n) {
        if (is_null(iptr0[n])) {
          consume(0, 1);
          continue;
        }

        optr0[nproduced++] = iptr0[n];
      }

      // Tell runtime system how many output items we produced.
      return nproduced;
    }

  } /* namespace gnss */
} /* namespace gr */

