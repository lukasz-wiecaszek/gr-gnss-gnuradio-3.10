/* -*- c++ -*- */
/*
 * Copyright 2023 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <cmath>

#include <gnuradio/io_signature.h>
#include "number_file_sink_impl.h"

namespace gr {
  namespace gnss {

    template<typename ITYPE>
    typename number_file_sink<ITYPE>::sptr
             number_file_sink<ITYPE>::make(std::size_t vlen, const char *filename, const char *delimiter)
    {
      return gnuradio::make_block_sptr<number_file_sink_impl<ITYPE>>(vlen, filename, delimiter);
    }

    template<typename ITYPE>
    number_file_sink_impl<ITYPE>::number_file_sink_impl(std::size_t vlen, const char *filename, const char *delimiter)
      : gr::sync_block("number_file_sink",
              gr::io_signature::make(1, -1, sizeof(ITYPE) * vlen),
              gr::io_signature::make(0, 0, 0)),
      d_vlen{vlen},
      d_fp{NULL},
      d_delimiter{delimiter}
    {
      if (filename)
        d_fp = fopen(filename, "w");
    }

    template<typename ITYPE>
    number_file_sink_impl<ITYPE>::~number_file_sink_impl()
    {
      if (d_fp) {
        fprintf(stderr, "number_file_sink_impl::~number_file_sink_impl: closing file descriptor\n");
        fclose(d_fp);
      }
    }

    template<typename ITYPE>
    void
    number_file_sink_impl<ITYPE>::print(const ITYPE *values[], std::size_t N)
    {
      for (std::size_t n = 0; n < N; ++n) {
        const ITYPE *v = values[n];
        for (std::size_t i = 0; i < d_vlen; ++i) {
          fprintf(d_fp, "%s", std::to_string(v[i]).c_str());
          if (i < (d_vlen - 1))
            fprintf(d_fp, " ");
        }
        if (d_vlen == 3) { // special case for vector3d
          fprintf(d_fp, " [%.15e]", std::sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]));
        }
        if (n < (N - 1))
          fprintf(d_fp, "\t");
      }
      fprintf(d_fp, "%s", d_delimiter.c_str());
    }

    template<>
    void
    number_file_sink_impl<float>::print(const float *values[], std::size_t N)
    {
      for (std::size_t n = 0; n < N; ++n) {
        const float *v = values[n];
        for (std::size_t i = 0; i < d_vlen; ++i) {
          fprintf(d_fp, "%.15e", v[i]);
          if (i < (d_vlen - 1))
            fprintf(d_fp, " ");
        }
        if (d_vlen == 3) { // special case for vector3d
          fprintf(d_fp, " [%.15e]", std::sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]));
        }
        if (n < (N - 1))
          fprintf(d_fp, "\t");
      }
      fprintf(d_fp, "%s", d_delimiter.c_str());
    }

    template<>
    void
    number_file_sink_impl<double>::print(const double *values[], std::size_t N)
    {
      for (std::size_t n = 0; n < N; ++n) {
        const double *v = values[n];
        for (std::size_t i = 0; i < d_vlen; ++i) {
          fprintf(d_fp, "%.15e", v[i]);
          if (i < (d_vlen - 1))
            fprintf(d_fp, " ");
        }
        if (d_vlen == 3) { // special case for vector3d
          fprintf(d_fp, " [%.15e]", std::sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]));
        }
        if (n < (N - 1))
          fprintf(d_fp, "\t");
      }
      fprintf(d_fp, "%s", d_delimiter.c_str());
    }

    template<>
    void
    number_file_sink_impl<std::complex<float>>::print(const std::complex<float> *values[], std::size_t N)
    {
      for (std::size_t n = 0; n < N; ++n) {
        const std::complex<float> *v = values[n];
        for (std::size_t i = 0; i < d_vlen; ++i) {
          fprintf(d_fp, "%.15e %.15e", v[i].real(), v[i].imag());
          if (i < (d_vlen - 1))
            fprintf(d_fp, " ");
        }
        if (n < (N - 1))
          fprintf(d_fp, "\t");
      }
      fprintf(d_fp, "%s", d_delimiter.c_str());
    }

    template<>
    void
    number_file_sink_impl<std::complex<double>>::print(const std::complex<double> *values[], std::size_t N)
    {
      for (std::size_t n = 0; n < N; ++n) {
        const std::complex<double> *v = values[n];
        for (std::size_t i = 0; i < d_vlen; ++i) {
          fprintf(d_fp, "%.15e %.15e", v[i].real(), v[i].imag());
          if (i < (d_vlen - 1))
            fprintf(d_fp, " ");
        }
        if (n < (N - 1))
          fprintf(d_fp, "\t");
      }
      fprintf(d_fp, "%s", d_delimiter.c_str());
    }

    template<typename ITYPE>
    int
    number_file_sink_impl<ITYPE>::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      if (d_fp == NULL)
        return 0;

      const std::size_t N = input_items.size();
      const ITYPE* values[N];

      for (int i = 0; i < noutput_items; i += d_vlen) {
        for (std::size_t n = 0; n < N; ++n) {
          const ITYPE* iptrn = (const ITYPE*) input_items[n];
          values[n] = iptrn + i;
        }
        print(values, N);
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

    template class number_file_sink<std::int8_t>;
    template class number_file_sink<std::uint8_t>;
    template class number_file_sink<std::int16_t>;
    template class number_file_sink<std::uint16_t>;
    template class number_file_sink<std::int32_t>;
    template class number_file_sink<std::uint32_t>;
    template class number_file_sink<std::int64_t>;
    template class number_file_sink<std::uint64_t>;
    template class number_file_sink<float>;
    template class number_file_sink<double>;
    template class number_file_sink<std::complex<float>>;
    template class number_file_sink<std::complex<double>>;

  } /* namespace gnss */
} /* namespace gr */
