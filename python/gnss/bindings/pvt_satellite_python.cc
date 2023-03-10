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
/* BINDTOOL_HEADER_FILE(pvt_satellite.h)                                           */
/* BINDTOOL_HEADER_FILE_HASH(2eaab6c7a074c1007b54ee2caa327264)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <gnuradio/gnss/pvt_satellite.h>
#include <pvt_satellite_pydoc.h>

void bind_pvt_satellite(py::module& m)
{
    using pvt_satellite = gr::gnss::pvt_satellite;

    py::class_<pvt_satellite,
               gr::block,
               gr::basic_block,
               std::shared_ptr<pvt_satellite>>(m, "pvt_satellite", D(pvt_satellite))
        .def(
            py::init(&pvt_satellite::make),
            D(pvt_satellite,make)
        )
        .def(
            "set_acq_params",
            &pvt_satellite::set_acq_params,
            py::arg("system"),
            py::arg("id")
        )
        .def(
            "get_acq_params",
            &pvt_satellite::get_acq_params,
            py::arg("system"),
            py::arg("id")
        )
        ;
}
