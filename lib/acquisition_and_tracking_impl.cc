/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include <gnuradio/nco.h>
#include <volk/volk.h>
#include <numeric>

#include "acquisition_and_tracking_impl.h"
#include "gnss_parameters.h"
#include "tags.h"
#include "ca_code.h"
#include "dll_discriminators.h"
#include "pll_discriminators.h"

namespace gr {
  namespace gnss {

    using gps_ca_code = ca_code<int, GPS_CA_CODE_LENGTH>;

    /*===========================================================================*\
    * public function definitions
    \*===========================================================================*/
    acquisition_and_tracking::sptr
    acquisition_and_tracking::make(double sampling_freq,
      double dll_bw_coarse, double pll_bw_coarse, double dll_bw_fine, double pll_bw_fine)
    {
      return gnuradio::make_block_sptr<acquisition_and_tracking_impl<gr_complex, gr_complexd>>(
        sampling_freq, dll_bw_coarse, pll_bw_coarse, dll_bw_fine, pll_bw_fine);
    }

    template<typename ITYPE, typename OTYPE>
    acquisition_and_tracking_impl<ITYPE, OTYPE>::acquisition_and_tracking_impl(double sampling_freq,
      double dll_bw_coarse, double pll_bw_coarse, double dll_bw_fine, double pll_bw_fine)
      : gr::block("acquisition_and_tracking",
                  gr::io_signature::make(1, 1, sizeof(ITYPE) * IVLEN),
                  gr::io_signature::make3(1, 3, sizeof(OTYPE) * OVLEN, sizeof(double) * OVLEN, sizeof(double) * OVLEN)),
        d_sampling_freq{sampling_freq},
        d_dll_bw_coarse{dll_bw_coarse},
        d_pll_bw_coarse{pll_bw_coarse},
        d_dll_bw_fine{dll_bw_fine},
        d_pll_bw_fine{pll_bw_fine},
        d_spreading_code_samples{static_cast<decltype(d_spreading_code_samples)>(ceil(d_sampling_freq / GPS_CA_CODE_RATE))},
        d_state{state_e::ACQUISITION},
        d_navigation_system{NAVIGATION_SYSTEM_UNDEFINED},
        d_id{-1},
        d_spreading_code(d_spreading_code_samples),
        d_fft_fwd{std::make_unique<gr::fft::fft_complex_fwd>(d_spreading_code_samples)},
        d_fft_rev{std::make_unique<gr::fft::fft_complex_rev>(d_spreading_code_samples)},
        d_doppler_shifts{sampling_freq, -doppler_df * doppler_ssb_bins, doppler_df, d_spreading_code_samples},
        d_magnitude(d_spreading_code_samples),
        d_max_magnitude_stats{},
        d_code_chip_rate{GPS_CA_CODE_CHIP_RATE},
        d_code_offset_chips{0.0},
        d_code_offset_samples{0.0},
        d_freq{0.0},
        d_freq_offset{0.0},
        d_dll_loop_filter{1.0 / GPS_CA_CODE_RATE},
        d_pll_loop_filter{1.0 / GPS_CA_CODE_RATE},
        d_tracking_history{}
    {
      set_relative_rate(1, d_spreading_code_samples);
    }

    template<typename ITYPE, typename OTYPE>
    acquisition_and_tracking_impl<ITYPE, OTYPE>::~acquisition_and_tracking_impl()
    {
    }

    template<typename ITYPE, typename OTYPE>
    void
    acquisition_and_tracking_impl<ITYPE, OTYPE>::set_acq_params(navigation_system_e system, int id)
    {
      gr::thread::scoped_lock lock(d_setlock);

      d_navigation_system = system;
      d_id = id;

      init_spreading_code();
      d_max_magnitude_stats.reset();
    }

    template<typename ITYPE, typename OTYPE>
    void
    acquisition_and_tracking_impl<ITYPE, OTYPE>::get_acq_params(navigation_system_e& system, int& id) const
    {
      system = d_navigation_system;
      id = d_id;
    }

    template<typename ITYPE, typename OTYPE>
    void
    acquisition_and_tracking_impl<ITYPE, OTYPE>::forecast(int noutput_items, gr_vector_int &ninput_items_required)
    {
      // 1 output item requires d_spreading_code_samples (integration period)
      int nrequired = noutput_items * d_spreading_code_samples;

      for (auto&& element : ninput_items_required)
        element = nrequired;
    }

    template<typename ITYPE, typename OTYPE>
    int
    acquisition_and_tracking_impl<ITYPE, OTYPE>::general_work(int noutput_items,
        gr_vector_int &ninput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      int nproduced;

      gr::thread::scoped_lock lock(d_setlock);

      switch (d_state)
      {
        case state_e::ACQUISITION:
          nproduced = work_acquisition(noutput_items, ninput_items, input_items, output_items);
          break;

        case state_e::TRACKING_COARSE:
          nproduced = work_tracking(noutput_items, ninput_items, input_items, output_items, correlation_shifts_coarse);
          break;

        case state_e::TRACKING_FINE:
          nproduced = work_tracking(noutput_items, ninput_items, input_items, output_items, correlation_shifts_fine);
          break;

        case state_e::UNLOCKED: /* fall through */
        default:
          consume_each(ninput_items[0]);
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
    void
    acquisition_and_tracking_impl<ITYPE, OTYPE>::init_spreading_code()
    {
      const std::shared_ptr<gps_ca_code> code = gps_ca_code::get(d_id);
      if (code == nullptr)
        throw std::out_of_range("invalid space vehicle id");

      for (int i = 0; i < d_spreading_code_samples; ++i)
        d_spreading_code[i] = (*code)[(i * GPS_CA_CODE_LENGTH) / d_spreading_code_samples] ?
          gr_complex{+1.0f, 0.0f} : gr_complex{-1.0f, 0.0f};

      memcpy(d_fft_fwd->get_inbuf(), d_spreading_code.data(), d_spreading_code_samples * sizeof(gr_complex));
      d_fft_fwd->execute();

      volk_32fc_conjugate_32fc(d_spreading_code.data(), d_fft_fwd->get_outbuf(), d_spreading_code_samples);
    }

    template<typename ITYPE, typename OTYPE>
    int
    acquisition_and_tracking_impl<ITYPE, OTYPE>::work_acquisition(int noutput_items,
        gr_vector_int &ninput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const ITYPE* iptr0 = (const ITYPE*) input_items[0];

      for (int n = 0; n < d_doppler_shifts.bins(); ++n) {
        // Doppler shift
        volk_32fc_x2_multiply_32fc(d_fft_fwd->get_inbuf(), iptr0, d_doppler_shifts[n].data(), d_doppler_shifts[n].size());

        // Compute the forward fft of the doppler shifted incoming signal
        d_fft_fwd->execute();

        // Multiply doppler shifted incoming signal with the spreading code
        volk_32fc_x2_multiply_32fc(d_fft_rev->get_inbuf(), d_fft_fwd->get_outbuf(), d_spreading_code.data(), d_spreading_code.size());

        // Compute the inverse fft
        d_fft_rev->execute();

        // Compute the magnitude^2
        volk_32fc_magnitude_squared_32f(d_magnitude.data(), d_fft_rev->get_outbuf(), d_magnitude.size());

        // Get statistics
        magnitude_stats<float> stats = magnitude_stats<float>::get(d_magnitude, n);
        if (n > 0) {
          if (stats.d_max > d_max_magnitude_stats.d_max)
            d_max_magnitude_stats = stats;
        } else
          d_max_magnitude_stats = stats;
      }

      if (d_max_magnitude_stats.d_max > (d_max_magnitude_stats.d_avg * 2)) { // this needs to be reworked
        printf("svid %02d: (positive acquisition) - doppler shift f: %.1f Hz [%s]\n",
          d_id, d_doppler_shifts.n_to_freq(d_max_magnitude_stats.d_freq_index), d_max_magnitude_stats.to_string().c_str());

        d_code_chip_rate = GPS_CA_CODE_CHIP_RATE;
        d_code_offset_chips = 0.0;
        d_code_offset_samples = 0.0;
        d_freq = d_doppler_shifts.n_to_freq(d_max_magnitude_stats.d_freq_index);
        d_freq_offset = 0.0;
        d_tracking_history.reset();
        d_dll_loop_filter.reset(0.0, d_dll_bw_coarse);
        d_pll_loop_filter.reset(d_freq, d_pll_bw_coarse);

        printf("svid %02d: switching to 'coarse tracking' state\n", d_id);
        d_state = state_e::TRACKING_COARSE;

        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each(d_max_magnitude_stats.d_max_index);
      }
      else {
        printf("svid %02d: (negative acquisition) - switching to unlocked state\n", d_id);

        d_state = state_e::UNLOCKED;

        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each(d_spreading_code_samples);
      }

      // Tell runtime system how many output items we produced.
      return 0;
    }

    template<typename ITYPE, typename OTYPE>
    template<std::size_t CTAPS>
    int
    acquisition_and_tracking_impl<ITYPE, OTYPE>::work_tracking(int noutput_items,
        gr_vector_int &ninput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items,
        const double (&correlation_shifts)[CTAPS])
    {
      const ITYPE* iptr0 = (const ITYPE*) input_items[0];
      OTYPE* optr0 = (OTYPE*) output_items[0];
      double* optr1 = nullptr;
      double* optr2 = nullptr;

      if (output_items.size() > 1)
        optr1 = (double*) output_items[1];

      if (output_items.size() > 2)
        optr2 = (double*) output_items[2];

      const std::shared_ptr<gps_ca_code> code = gps_ca_code::get(d_id);
      if (code == nullptr)
        throw std::out_of_range("invalid space vehicle id");

      // Doppler shift
      gr::fxpt_nco nco;
      std::vector<gr_complex> doppler_shift(d_spreading_code_samples);
      std::vector<gr_complex> input_stream(d_spreading_code_samples);

      nco.set_freq(-2.0 * GR_M_PI * d_freq / d_sampling_freq);
      nco.set_phase(d_freq_offset);
      nco.sincos(doppler_shift.data(), doppler_shift.size(), 1);
      d_freq_offset = nco.get_phase();

      volk_32fc_x2_multiply_32fc(input_stream.data(), iptr0, doppler_shift.data(), d_spreading_code_samples);

      std::array<std::vector<gr_complexd>, CTAPS> spreading_codes;
      std::array<gr_complexd, CTAPS> correlations;
      std::vector<gr_complexd> output_stream(input_stream.begin(), input_stream.end());

      double code_phase_increment = d_code_chip_rate / d_sampling_freq; // [samples]

      for (std::size_t tap = 0; tap < CTAPS; ++tap) {
        std::vector<gr_complexd>& spreading_code = spreading_codes[tap];
        spreading_code.resize(d_spreading_code_samples);
        for (int i = 0; i < d_spreading_code_samples; ++i) {
          int idx = static_cast<int>(i * code_phase_increment + d_code_offset_chips + correlation_shifts[tap]);
          idx += GPS_CA_CODE_LENGTH;
          idx %= GPS_CA_CODE_LENGTH;
          spreading_code[i] = (*code)[idx] ? gr_complexd{+1.0, 0.0} : gr_complexd{-1.0, 0.0};
        }

        // Multiply and then integrate doppler shifted incoming signal with the spreading code
        correlations[tap] = std::inner_product(output_stream.begin(), output_stream.end(), spreading_code.begin(), gr_complexd{0.0, 0.0});
      }

      gr_complexd E = correlations[0];
      gr_complexd P = correlations[1]; *optr0 = P;
      gr_complexd L = correlations[2];

      double rx_time = (nitems_read(0) - d_code_offset_samples) / d_sampling_freq;
      add_item_tag(0, nitems_written(0), pmt::mp(TAG_RX_TIME), pmt::mp(rx_time), alias_pmt());

      double dll_discriminator = dll_discriminator_noncoherent_e_minus_l_power(E, L);
      double dll_discriminator_filtered = d_dll_loop_filter[dll_discriminator * (1.0 + correlation_shifts[0])];

      double pll_discriminator = pll_discriminator_two_quadrant_arctangent(P);
      double pll_discriminator_filtered = d_pll_loop_filter[pll_discriminator / (2.0 * GR_M_PI)];
      d_freq = pll_discriminator_filtered;

      d_tracking_history.add(std::abs(pll_discriminator));

      d_code_chip_rate = GPS_CA_CODE_CHIP_RATE - dll_discriminator_filtered;
      d_code_chip_rate += d_freq * GPS_CA_CODE_CHIP_RATE / GPS_L1_FREQ_HZ;

      if (optr1)
        *optr1 = d_freq;

      if (optr2)
        *optr2 = d_code_chip_rate;

      d_code_offset_chips += GPS_CA_CODE_LENGTH * d_code_chip_rate / GPS_CA_CODE_CHIP_RATE - GPS_CA_CODE_LENGTH;
      d_code_offset_samples += GPS_CA_CODE_LENGTH * d_sampling_freq * (1.0 / GPS_CA_CODE_CHIP_RATE - 1.0 / d_code_chip_rate);

#if 0
      {
        static int cnt = 0;
        if ((cnt++ % 500) == 0) {
          printf("svid %02d: E: %e P: %e L: %e pll [%e %e] dll [%e %e] %e %e %e\n",
            d_id,
            std::norm(E), std::norm(P), std::norm(L),
            pll_discriminator, pll_discriminator_filtered,
            dll_discriminator, dll_discriminator_filtered,
            d_code_chip_rate, d_code_offset_chips, d_code_offset_samples);
        }
      }
#endif

      if (d_tracking_history.n > GPS_CA_CODE_RATE /* 1 second */) {
        double pll_discriminator_avg = d_tracking_history.avg();
        if (d_state == state_e::TRACKING_COARSE) {
          if (pll_discriminator_avg < 0.1) {
            printf("svid %02d: switching to 'fine tracking' state\n", d_id);
            d_tracking_history.reset();
            d_dll_loop_filter.update_coefficients(d_dll_bw_fine);
            d_pll_loop_filter.update_coefficients(d_pll_bw_fine);
            d_state = state_e::TRACKING_FINE;
          }
          else
          if (pll_discriminator_avg > 0.5) {
            printf("svid %02d: switching to 'acquisition' state\n", d_id);
            d_state = state_e::ACQUISITION;
          }
          else {
            /* stay in coarse tracking state */
          }
        }
        else
        if (d_state == state_e::TRACKING_FINE) {
          if (pll_discriminator_avg > 0.25) {
            printf("svid %02d: switching to 'coarse tracking' state\n", d_id);
            d_tracking_history.reset();
            d_dll_loop_filter.update_coefficients(d_dll_bw_coarse);
            d_pll_loop_filter.update_coefficients(d_pll_bw_coarse);
            d_state = state_e::TRACKING_COARSE;
          }
          else {
            /* stay in fine tracking state */
          }
        } else {
          /* do nothing */
        }
      }

      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each(d_spreading_code_samples);

      // Tell runtime system how many output items we produced.
      return 1;
    }

  } /* namespace gnss */
} /* namespace gr */
