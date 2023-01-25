/*
 * Copyright 2023 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/***********************************************************************************/
/* This file is automatically generated using bindtool and can be manually edited  */
/* The following lines can be configured to regenerate this file during cmake      */
/* If manual edits are made, the following tags should be modified accordingly.    */
/* BINDTOOL_GEN_AUTOMATIC(0)                                                       */
/* BINDTOOL_USE_PYGCCXML(0)                                                        */
/* BINDTOOL_HEADER_FILE(signal_normalizer.h)                                       */
/* BINDTOOL_HEADER_FILE_HASH(965ed78d1926c749204311bf505311d0)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <gnuradio/gnss/signal_normalizer.h>
#include <signal_normalizer_pydoc.h>

template<typename ITYPE, typename OTYPE>
void bind_signal_normalizer_template(py::module& m, const char* classname)
{
    using signal_normalizer = gr::gnss::signal_normalizer<ITYPE, OTYPE>;

    py::class_<signal_normalizer,
               gr::block,
               gr::basic_block,
               std::shared_ptr<signal_normalizer>>(m, classname, D(signal_normalizer))
        .def(
            py::init(&signal_normalizer::make),
            py::arg("vlen"),
            py::arg("adc_resolution"),
            D(signal_normalizer,make)
        );
}

void bind_signal_normalizer(py::module& m)
{
    bind_signal_normalizer_template<std::int8_t, std::complex<float>>(m, "signal_normalizer_s8_fc32");
    bind_signal_normalizer_template<std::uint8_t, std::complex<float>>(m, "signal_normalizer_u8_fc32");
    bind_signal_normalizer_template<std::int16_t, std::complex<float>>(m, "signal_normalizer_s16_fc32");
    bind_signal_normalizer_template<std::uint16_t, std::complex<float>>(m, "signal_normalizer_u16_fc32");

    bind_signal_normalizer_template<std::int8_t, std::complex<double>>(m, "signal_normalizer_s8_fc64");
    bind_signal_normalizer_template<std::uint8_t, std::complex<double>>(m, "signal_normalizer_u8_fc64");
    bind_signal_normalizer_template<std::int16_t, std::complex<double>>(m, "signal_normalizer_s16_fc64");
    bind_signal_normalizer_template<std::uint16_t, std::complex<double>>(m, "signal_normalizer_u16_fc64");
}
