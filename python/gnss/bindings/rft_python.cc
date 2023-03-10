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
/* BINDTOOL_HEADER_FILE(rft.h)                                                     */
/* BINDTOOL_HEADER_FILE_HASH(093326318382e2f9becabbcb3595b23d)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <gnuradio/gnss/rft.h>
#include <rft_pydoc.h>

void bind_rft(py::module& m)
{
    using rft = gr::gnss::rft;

    py::class_<rft,
               gr::sync_block,
               gr::block,
               gr::basic_block,
               std::shared_ptr<rft>>(m, "rft", D(rft))
        .def(
            py::init(&rft::make),
            D(rft,make)
        )
        .def(
            "set_transformation",
            &rft::set_transformation,
            py::arg("type")
        )
        .def(
            "get_transformation",
            &rft::get_transformation,
            py::arg("type")
        )
        ;
}
