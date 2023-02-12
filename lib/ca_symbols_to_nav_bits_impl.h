/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GNSS_CA_SYMBOLS_TO_NAV_BITS_IMPL_H
#define INCLUDED_GNSS_CA_SYMBOLS_TO_NAV_BITS_IMPL_H

#include <gnuradio/gnss/ca_symbols_to_nav_bits.h>
#include <bitset>
#include <cmath>

#include "gnss_parameters.h"

#define IVLEN 1
#define OVLEN 1

#define STATES    \
  STATE(unlocked) \
  STATE(locked)   \

namespace gr {
  namespace gnss {

    template<typename ITYPE, typename OTYPE>
    class ca_symbols_to_nav_bits_impl : public ca_symbols_to_nav_bits
    {
    public:
      ca_symbols_to_nav_bits_impl();
      ~ca_symbols_to_nav_bits_impl();

      void forecast(int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items);

    private:
      enum class state_e {
#define STATE(x) x,
        STATES
#undef STATE
      };

#define STATE(x) int state_handler_##x(int noutput_items,                      \
                                       gr_vector_int &ninput_items,            \
                                       gr_vector_const_void_star &input_items, \
                                       gr_vector_void_star &output_items);
        STATES
#undef STATE

      int to_bit(ITYPE symbol)
      {
        return (symbol.real() > 0);
      }

      bool is_preamble_detected(const ITYPE* symbols)
      {
        std::size_t bit_idx;
        int bit;
        int xorarg;
        int n = 0;

        for (bit_idx = 0; bit_idx < GPS_CA_TLM_PREAMBLE_BITS.size(); ++bit_idx) {
          bit = to_bit(symbols[bit_idx * GPS_CA_CODES_PER_NAV_MESSAGE_BIT]);
          for (int i = 1; i < GPS_CA_CODES_PER_NAV_MESSAGE_BIT; ++i)
            if (bit != to_bit(symbols[bit_idx * GPS_CA_CODES_PER_NAV_MESSAGE_BIT + i]))
              return false;

          if (GPS_CA_TLM_PREAMBLE_BITS[bit_idx] == bit)
            n++;
          else
            n--;

          if (std::abs(n) != (bit_idx + 1))
            return false;
        }

        if (n > 0) {
          d_polarity = +1;
          xorarg = 0;
        } else {
          d_polarity = -1;
          xorarg = 1;
        }

        // At this point we know that the first 8 bits correspond to preamble pattern '0b10001011'.
        // But this may be some arbitrary sequence of bits which apparently looks exactly the same as preamble.
        // Some additional check needs to be done, and this will be checking the parity bits of that word.

        std::bitset<GPS_NAV_MESSAGE_BITS_PER_WORD> word = 0b10001011;

        for (; bit_idx < 24; ++bit_idx) {
          bit = to_bit(symbols[bit_idx * GPS_CA_CODES_PER_NAV_MESSAGE_BIT]) ^ xorarg;
          word <<= 1;
          word |= bit;
        }

        word <<= 6;

        // IS-GPS-200L - Chapter 20.3.5.2 "User Parity Algorithm"
        bit = to_bit(symbols[bit_idx++ * GPS_CA_CODES_PER_NAV_MESSAGE_BIT]) ^ xorarg;
        if ((bit + (word & GPS_PARITY_ALGORITHM_MASK_D25).count()) % 2)
          return false;

        bit = to_bit(symbols[bit_idx++ * GPS_CA_CODES_PER_NAV_MESSAGE_BIT]) ^ xorarg;
        if ((bit + (word & GPS_PARITY_ALGORITHM_MASK_D26).count()) % 2)
          return false;

        bit = to_bit(symbols[bit_idx++ * GPS_CA_CODES_PER_NAV_MESSAGE_BIT]) ^ xorarg;
        if ((bit + (word & GPS_PARITY_ALGORITHM_MASK_D27).count()) % 2)
          return false;

        bit = to_bit(symbols[bit_idx++ * GPS_CA_CODES_PER_NAV_MESSAGE_BIT]) ^ xorarg;
        if ((bit + (word & GPS_PARITY_ALGORITHM_MASK_D28).count()) % 2)
          return false;

        bit = to_bit(symbols[bit_idx++ * GPS_CA_CODES_PER_NAV_MESSAGE_BIT]) ^ xorarg;
        if ((bit + (word & GPS_PARITY_ALGORITHM_MASK_D29).count()) % 2)
          return false;

        bit = to_bit(symbols[bit_idx++ * GPS_CA_CODES_PER_NAV_MESSAGE_BIT]) ^ xorarg;
        if ((bit + (word & GPS_PARITY_ALGORITHM_MASK_D30).count()) % 2)
          return false;

        return true;
      }

      int get_bit(const ITYPE* symbols)
      {
        double sum = 0;

        for (int i = 0; i < GPS_CA_CODES_PER_NAV_MESSAGE_BIT; ++i)
          sum += symbols[i].real();

        return d_polarity * sum > 0 ? 1 : 0;
      }

      state_e d_state;
      int d_polarity;
      int d_subframe_bit;
    };

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_CA_SYMBOLS_TO_NAV_BITS_IMPL_H */
