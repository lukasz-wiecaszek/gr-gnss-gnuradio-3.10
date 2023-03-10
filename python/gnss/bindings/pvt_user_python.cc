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
/* BINDTOOL_HEADER_FILE(pvt_user.h)                                                */
/* BINDTOOL_HEADER_FILE_HASH(57e1efb9aa45198474b845f823f21169)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <gnuradio/gnss/pvt_user.h>
#include <pvt_user_pydoc.h>

void bind_pvt_user(py::module& m)
{
    using pvt_user = gr::gnss::pvt_user;

    py::class_<pvt_user,
               gr::block,
               gr::basic_block,
               std::shared_ptr<pvt_user>>(m, "pvt_user", D(pvt_user))
        .def(
            py::init(&pvt_user::make),
            py::arg("skip_zeros"),
            D(pvt_user,make)
        );
}








