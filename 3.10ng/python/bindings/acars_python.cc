/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * This file is part of GNU Radio.
 *
 * NOTE: The lines with "BINDTOOL_*" comments are for the binding tool
 * (gr_modtool) and can be automatically regenerated. If you manually
 * edit this file, set BINDTOOL_GEN_AUTOMATIC(0) to avoid overwriting.
 */

/***********************************************************************************/
/* BINDTOOL_GEN_AUTOMATIC(0)                                                       */
/* BINDTOOL_USE_PYGCCXML(0)                                                        */
/* BINDTOOL_HEADER_FILE(acars.h)                                                   */
/* BINDTOOL_HEADER_FILE_HASH(701a7a3f39cefe50a843014f55a9d9b8)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// For succinctness
namespace py = pybind11;

#include <acars/acars.h>
// pydoc.h is automatically generated during the build (via doxygen & gr_modtool)
#include <acars_pydoc.h>

// This function will be called by python_bindings.cc in PYBIND11_MODULE(acars_python, ...)
void bind_acars(py::module& m)
{
    using acars = ::gr::acars::acars;

    py::class_<acars, gr::sync_block, gr::block, gr::basic_block,
        std::shared_ptr<acars>>(m, "acars", D(acars))

        // Constructor (acars::make)
        .def(py::init(&acars::make),
             py::arg("seuil"),
             py::arg("filename"),
             py::arg("saveall"),
             D(acars, make)
        )

        // Expose set_seuil(...) to Python
        .def("set_seuil",
             &acars::set_seuil,
             py::arg("threshold"),
             D(acars, set_seuil)
        );
}
