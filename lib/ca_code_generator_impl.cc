/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <memory>
#include <cmath>

#include <gnuradio/io_signature.h>
#include <gnuradio/fft/fft.h>
#include <volk/volk.h>

#include "ca_code_generator_impl.h"
#include "gnss_parameters.h"
#include "ca_code.h"

namespace gr {
  namespace gnss {

    using gps_ca_code = ca_code<int, GPS_CA_CODE_LENGTH>;

    /*===========================================================================*\
    * public function definitions
    \*===========================================================================*/
    template<typename T>
    typename ca_code_generator<T>::sptr
    ca_code_generator<T>::make(std::size_t vlen, double sampling_freq, unsigned svid, ca_code_domain_e domain)
    {
      return gnuradio::make_block_sptr<ca_code_generator_impl<T>>(vlen, sampling_freq, svid, domain);
    }

    /*===========================================================================*\
    * constructor for non-complex types
    \*===========================================================================*/
    template<typename T>
    ca_code_generator_impl<T>::ca_code_generator_impl(std::size_t vlen, double sampling_freq, unsigned svid, ca_code_domain_e domain)
      : gr::sync_block("ca_code_generator",
                       gr::io_signature::make(0, 0, 0),
                       gr::io_signature::make(1, 1, sizeof(T) * vlen)),
        d_vlen{vlen},
        d_n_samples{static_cast<decltype(d_n_samples)>(ceil(sampling_freq / GPS_CA_CODE_RATE))},
        d_code_sampled(d_n_samples),
        d_n{0}
    {
      if (domain != CA_CODE_DOMAIN_TIME)
        throw std::out_of_range("only time domain is supported for non-complex types");

      const std::shared_ptr<gps_ca_code> code = gps_ca_code::get(svid);
      if (code == nullptr)
        throw std::out_of_range("invalid space vehicle id");

      printf("samples per ca code: %d\n", d_n_samples);

      for (int i = 0; i < d_n_samples; ++i)
        d_code_sampled[i] = (*code)[(i * GPS_CA_CODE_LENGTH) / d_n_samples] ? +1 : -1;
    }

    /*===========================================================================*\
    * constructor for gr_complex (template specialization)
    \*===========================================================================*/
    template<>
    ca_code_generator_impl<gr_complex>::ca_code_generator_impl(std::size_t vlen, double sampling_freq, unsigned svid, ca_code_domain_e domain)
      : gr::sync_block("ca_code_generator",
                       gr::io_signature::make(0, 0, 0),
                       gr::io_signature::make(1, 1, sizeof(gr_complex) * vlen)),
        d_vlen{vlen},
        d_n_samples{static_cast<decltype(d_n_samples)>(ceil(sampling_freq / GPS_CA_CODE_RATE))},
        d_code_sampled(d_n_samples),
        d_n{0}
    {
      const std::shared_ptr<gps_ca_code> code = gps_ca_code::get(svid);
      if (code == nullptr)
        throw std::out_of_range("invalid space vehicle id");

      printf("samples per ca code: %d\n", d_n_samples);

      for (int i = 0; i < d_n_samples; ++i)
        d_code_sampled[i] = (*code)[(i * GPS_CA_CODE_LENGTH) / d_n_samples] ?
          gr_complex{+1.0f, 0.0f} : gr_complex{-1.0f, 0.0f};

      if ((domain == CA_CODE_DOMAIN_FREQUENCY) || (domain == CA_CODE_DOMAIN_FREQUENCY_CONJUGATE)) {
        auto fft = std::make_unique<gr::fft::fft_complex_fwd>(d_code_sampled.size());

        memcpy(fft->get_inbuf(), d_code_sampled.data(), d_code_sampled.size() * sizeof(gr_complex));
        fft->execute();

        if (domain == CA_CODE_DOMAIN_FREQUENCY_CONJUGATE)
          volk_32fc_conjugate_32fc(d_code_sampled.data(), fft->get_outbuf(), d_code_sampled.size());
        else
          memcpy(d_code_sampled.data(), fft->get_outbuf(), d_code_sampled.size() * sizeof(gr_complex));
      }
    }

    template<typename T>
    ca_code_generator_impl<T>::~ca_code_generator_impl()
    {
    }

    template<typename T>
    int
    ca_code_generator_impl<T>::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      T* optr0 = (T*) output_items[0];

      for (std::size_t i = 0; i < (d_vlen * noutput_items); ++i, ++d_n)
        optr0[i] = d_code_sampled[d_n % d_n_samples];

      d_n %= d_n_samples;

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

    /*===========================================================================*\
    * protected function definitions
    \*===========================================================================*/

    /*===========================================================================*\
    * private function definitions
    \*===========================================================================*/

    template class ca_code_generator<std::int8_t>;
    template class ca_code_generator<std::int16_t>;
    template class ca_code_generator<std::int32_t>;
    template class ca_code_generator<float>;
    template class ca_code_generator<double>;
    template class ca_code_generator<std::complex<float>>;

  } /* namespace gnss */
} /* namespace gr */
