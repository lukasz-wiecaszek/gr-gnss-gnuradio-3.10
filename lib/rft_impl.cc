/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "rft_impl.h"
#include <gnuradio/io_signature.h>
#include "rft_impl.h"
#include "vector3d.h"
#include "reference_frames_transformations.h"

namespace gr {
  namespace gnss {

    /*===========================================================================*\
    * public function definitions
    \*===========================================================================*/
    rft::sptr
    rft::make()
    {
      return gnuradio::make_block_sptr<rft_impl<vector3d, vector3d>>();
    }

    template<typename ITYPE, typename OTYPE>
    rft_impl<ITYPE, OTYPE>::rft_impl()
      : gr::sync_block("rft",
                       gr::io_signature::make(1, 1, sizeof(ITYPE) * IVLEN),
                       gr::io_signature::make(1, 1, sizeof(OTYPE) * OVLEN)),
        d_rft_type{RFT_UNDEFINED}
    {
    }

    template<typename ITYPE, typename OTYPE>
    rft_impl<ITYPE, OTYPE>::~rft_impl()
    {
    }

    template<typename ITYPE, typename OTYPE>
    void
    rft_impl<ITYPE, OTYPE>::set_transformation(rft_type_e type)
    {
      gr::thread::scoped_lock lock(d_setlock);
      d_rft_type = type;
    }

    template<typename ITYPE, typename OTYPE>
    void
    rft_impl<ITYPE, OTYPE>::get_transformation(rft_type_e& type) const
    {
      type = d_rft_type;
    }

    template<typename ITYPE, typename OTYPE>
    int
    rft_impl<ITYPE, OTYPE>::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const ITYPE* iptr0 = (const ITYPE*) input_items[0];
      OTYPE* optr0 = (OTYPE*) output_items[0];

      gr::thread::scoped_lock lock(d_setlock);

      switch (d_rft_type)
      {
        case RFT_GCS_TO_ECEF:
          for (int n = 0; n < noutput_items; ++n)
            rftns::gcs_to_ecef(iptr0[n], optr0[n]);
          break;

        case RFT_ECEF_TO_GCS:
          for (int n = 0; n < noutput_items; ++n)
            rftns::ecef_to_gcs(iptr0[n], optr0[n]);
          break;

        default:
          /* do nothing */
          break;
      }

#if 0
      for (int n = 0; n < noutput_items; ++n) {
        printf("in: %s\n", to_string(iptr0[n]).c_str());
        printf("out: %s\n", to_string(optr0[n]).c_str());
      }
#endif

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace gnss */
} /* namespace gr */
