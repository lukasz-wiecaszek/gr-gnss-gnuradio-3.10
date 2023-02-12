/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#include <gnuradio/io_signature.h>

#include "tags.h"
#include "ca_symbols_to_nav_bits_impl.h"

namespace gr {
  namespace gnss {

    /*===========================================================================*\
    * public function definitions
    \*===========================================================================*/
    ca_symbols_to_nav_bits::sptr
    ca_symbols_to_nav_bits::make()
    {
      return gnuradio::make_block_sptr<ca_symbols_to_nav_bits_impl<gr_complexd, uint8_t>>();
    }

    template<typename ITYPE, typename OTYPE>
    ca_symbols_to_nav_bits_impl<ITYPE, OTYPE>::ca_symbols_to_nav_bits_impl()
      : gr::block("ca_symbols_to_nav_bits",
                  gr::io_signature::make(1, 1, sizeof(ITYPE) * IVLEN),
                  gr::io_signature::make(1, 1, sizeof(OTYPE) * OVLEN)),
        d_state{state_e::unlocked},
        d_polarity{0},
        d_subframe_bit{-1}
    {
      set_tag_propagation_policy(TPP_DONT);

      set_relative_rate(1, GPS_CA_CODES_PER_NAV_MESSAGE_BIT);
      set_output_multiple(GPS_NAV_MESSAGE_BITS_PER_WORD);
    }

    template<typename ITYPE, typename OTYPE>
    ca_symbols_to_nav_bits_impl<ITYPE, OTYPE>::~ca_symbols_to_nav_bits_impl()
    {
    }

    template<typename ITYPE, typename OTYPE>
    void
    ca_symbols_to_nav_bits_impl<ITYPE, OTYPE>::forecast(int noutput_items, gr_vector_int &ninput_items_required)
    {
      int nrequired = noutput_items * GPS_CA_CODES_PER_NAV_MESSAGE_BIT;

      for (auto&& element : ninput_items_required)
        element = nrequired;
    }

    template<typename ITYPE, typename OTYPE>
    int
    ca_symbols_to_nav_bits_impl<ITYPE, OTYPE>::general_work(
      int noutput_items,
      gr_vector_int &ninput_items,
      gr_vector_const_void_star &input_items,
      gr_vector_void_star &output_items)
    {
      int nproduced;

      switch (d_state)
      {
        case state_e::unlocked:
          nproduced = state_handler_unlocked(noutput_items, ninput_items, input_items, output_items);
          break;

        case state_e::locked:
          nproduced = state_handler_locked(noutput_items, ninput_items, input_items, output_items);
          break;

        default:
          consume(0, noutput_items * GPS_CA_CODES_PER_NAV_MESSAGE_BIT);
          nproduced = 0;
          break;
      }

      return nproduced;
    }

    /*===========================================================================*\
    * protected function definitions
    \*===========================================================================*/

    /*===========================================================================*\
    * private function definitions
    \*===========================================================================*/
    template<typename ITYPE, typename OTYPE>
    int
    ca_symbols_to_nav_bits_impl<ITYPE, OTYPE>::state_handler_unlocked(
      int noutput_items,
      gr_vector_int &ninput_items,
      gr_vector_const_void_star &input_items,
      gr_vector_void_star &output_items)
    {
      const ITYPE* iptr0 = (const ITYPE*) input_items[0];
      int limit = ninput_items[0] - GPS_NAV_MESSAGE_BITS_PER_WORD * GPS_CA_CODES_PER_NAV_MESSAGE_BIT;
      int i = 0;

      for (i = 0; i <= limit; i++) {
        if (is_preamble_detected(iptr0 + i)) {
          d_state = state_e::locked;
          d_subframe_bit = 0;
          break;
        }
      }

      // Tell runtime system how many input items we consumed on input 0
      consume(0, i);

      // Tell runtime system how many output items we produced.
      return 0;
    }

    template<typename ITYPE, typename OTYPE>
    int
    ca_symbols_to_nav_bits_impl<ITYPE, OTYPE>::state_handler_locked(
      int noutput_items,
      gr_vector_int &ninput_items,
      gr_vector_const_void_star &input_items,
      gr_vector_void_star &output_items)
    {
      const ITYPE* iptr0 = (const ITYPE*) input_items[0];
      OTYPE* optr0 = (OTYPE*) output_items[0];
      int nproduced = 0;
      int nconsumed = 0;

      std::vector<tag_t> tags;
      get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0) + ninput_items[0], pmt::mp(TAG_RX_TIME));

      while (nproduced < noutput_items) {
        if (d_subframe_bit == GPS_NAV_MESSAGE_BITS_PER_SUBFRAME) {
          if (!is_preamble_detected(iptr0)) {
            d_state = state_e::unlocked;
            break;
          }
          d_subframe_bit = 0;
        }

        optr0[nproduced] = get_bit(iptr0 + nconsumed);
        add_item_tag(0, nitems_written(0) + nproduced, tags[nconsumed].key, tags[nconsumed].value, alias_pmt());
        add_item_tag(0, nitems_written(0) + nproduced, pmt::mp(TAG_SUBFRAME_BIT), pmt::mp(d_subframe_bit), alias_pmt());

        nproduced++;
        nconsumed += GPS_CA_CODES_PER_NAV_MESSAGE_BIT;
        if (++d_subframe_bit == GPS_NAV_MESSAGE_BITS_PER_SUBFRAME)
          break;
      }

      // Tell runtime system how many input items we consumed on input 0
      consume(0, nconsumed);

      // Tell runtime system how many output items we produced.
      return nproduced;
    }

  } /* namespace gnss */
} /* namespace gr */
