/*
 * Copyright 2020 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include <pybind11/pybind11.h>

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

namespace py = pybind11;

// Headers for binding functions
/**************************************/
// The following comment block is used for
// gr_modtool to insert function prototypes
// Please do not delete
/**************************************/
// BINDING_FUNCTION_PROTOTYPES(
    void bind_navigation_system(py::module& m);
    void bind_acquisition_and_tracking(py::module& m);
    void bind_signal_normalizer(py::module& m);
    void bind_ca_code_domain(py::module& m);
    void bind_ca_code_generator(py::module& m);
    void bind_type_converter(py::module& m);
    void bind_ca_symbols_to_nav_bits(py::module& m);
    void bind_nav_message_decoder(py::module& m);
    void bind_resampler(py::module& m);
    void bind_gnss_channel(py::module& m);
    void bind_pseudoranges_decoder(py::module& m);
    void bind_pvt(py::module& m);
    void bind_rft_type(py::module& m);
    void bind_rft(py::module& m);
    void bind_skip_zeros(py::module& m);
    void bind_doppler_shift(py::module& m);
    void bind_geojson_file_sink(py::module& m);
    void bind_number_file_sink(py::module& m);
    void bind_pvt_satellite(py::module& m);
// ) END BINDING_FUNCTION_PROTOTYPES


// We need this hack because import_array() returns NULL
// for newer Python versions.
// This function is also necessary because it ensures access to the C API
// and removes a warning.
void* init_numpy()
{
    import_array();
    return NULL;
}

PYBIND11_MODULE(gnss_python, m)
{
    // Initialize the numpy C API
    // (otherwise we will see segmentation faults)
    init_numpy();

    // Allow access to base block methods
    py::module::import("gnuradio.gr");

    /**************************************/
    // The following comment block is used for
    // gr_modtool to insert binding function calls
    // Please do not delete
    /**************************************/
    // BINDING_FUNCTION_CALLS(
    bind_navigation_system(m);
    bind_acquisition_and_tracking(m);
    bind_signal_normalizer(m);
    bind_ca_code_domain(m);
    bind_ca_code_generator(m);
    bind_type_converter(m);
    bind_ca_symbols_to_nav_bits(m);
    bind_nav_message_decoder(m);
    bind_resampler(m);
    bind_gnss_channel(m);
    bind_pseudoranges_decoder(m);
    bind_pvt(m);
    bind_rft_type(m);
    bind_rft(m);
    bind_skip_zeros(m);
    bind_doppler_shift(m);
    bind_geojson_file_sink(m);
    bind_number_file_sink(m);
    bind_pvt_satellite(m);
    // ) END BINDING_FUNCTION_CALLS
}