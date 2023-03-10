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
/* BINDTOOL_HEADER_FILE(resampler.h)                                               */
/* BINDTOOL_HEADER_FILE_HASH(42eb01958e7075159892f6429d8886f2)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <gnuradio/gnss/resampler.h>
#include <resampler_pydoc.h>

void bind_resampler(py::module& m)
{
    using resampler = gr::gnss::resampler;

    py::class_<resampler,
               gr::block,
               gr::basic_block,
               std::shared_ptr<resampler>>(m, "resampler", D(resampler))
        .def(
            py::init(&resampler::make),
            py::arg("fs_in"),
            py::arg("fs_out"),
            D(resampler,make)
        );
}
