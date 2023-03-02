/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "pvt_user_impl.h"
#include "tags.h"

namespace gr {
  namespace gnss {

    /*===========================================================================*\
    * public function definitions
    \*===========================================================================*/
    pvt_user::sptr
    pvt_user::make(bool skip_zeros)
    {
      return gnuradio::make_block_sptr<pvt_user_impl<vector3d, vector3d>>(skip_zeros);
    }

    template<typename ITYPE, typename OTYPE>
    pvt_user_impl<ITYPE, OTYPE>::pvt_user_impl(bool skip_zeros)
      : gr::block("pvt_user",
                  gr::io_signature::make(1, MAX_STREAMS, sizeof(ITYPE) * IVLEN),
                  gr::io_signature::make(1, 1, sizeof(OTYPE) * OVLEN)),
      d_hint{},
      d_skip_zeros{skip_zeros}
    {
      set_tag_propagation_policy(TPP_DONT);
    }

    template<typename ITYPE, typename OTYPE>
    pvt_user_impl<ITYPE, OTYPE>::~pvt_user_impl()
    {
    }

    template<typename ITYPE, typename OTYPE>
    void
    pvt_user_impl<ITYPE, OTYPE>::forecast(int noutput_items, gr_vector_int &ninput_items_required)
    {
     // 1 output item requires 1 item at input
      int nrequired = noutput_items;

      for (auto&& element : ninput_items_required)
        element = nrequired;
    }

    template<typename ITYPE, typename OTYPE>
    int
    pvt_user_impl<ITYPE, OTYPE>::general_work(int noutput_items,
                 gr_vector_int &ninput_items,
                 gr_vector_const_void_star &input_items,
                 gr_vector_void_star &output_items)
    {
      int n;
      const int N = ninput_items.size();
      OTYPE* optr0 = (OTYPE*) output_items[0];
      int nproduced = 0;

      if (N < 4)
        throw std::out_of_range("invalid number of input pads (at least 4 required)");

      std::vector<tag_t> pseudorange_tags[N];
      for (n = 0; n < N; ++n)
        get_tags_in_range(pseudorange_tags[n], n, nitems_read(n), nitems_read(n) + noutput_items, pmt::mp(TAG_PSEUDORANGE));

      while (nproduced < noutput_items) {
        pvt_utils::satelite satelites[N];
        for (n = 0; n < N; ++n) {
          const ITYPE* iptr = static_cast<const ITYPE*>(input_items[n]);
          const std::vector<tag_t>& tags = pseudorange_tags[n];

          double pseudorange = pmt::to_double(tags[nproduced].value);
          if (pseudorange == 0)
            break; // we do not have valid pseudorange for this input port yet

          satelites[n].pseudorange = pmt::to_double(pseudorange_tags[n][nproduced].value);
          satelites[n].position = iptr[nproduced];
        }

        consume_each(1);

        if (n < N) {
          if (d_skip_zeros) {
            break;
          } else {
            optr0[nproduced] = {{0.0, 0.0, 0.0}};
          }
        } else {
          vector3d efec_user_position;

#define CASE(x) case x: pvt_utils::get<x>(satelites, d_hint, &efec_user_position, NULL, NULL)
          switch (N) {
            CASE(4); break;
            CASE(5); break;
            CASE(6); break;
            CASE(7); break;
            CASE(8); break;
            default: break;
          }
#undef CASE

          optr0[nproduced] = efec_user_position;
        }

        nproduced++;
      }

      // Tell runtime system how many output items we produced.
      return nproduced;
    }

  } /* namespace gnss */
} /* namespace gr */
