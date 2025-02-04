/*
 * Copyright 2020 Free Software Foundation, Inc.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * This file is part of GNU Radio
 */

#include <pybind11/pybind11.h>

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

namespace py = pybind11;

/**************************************/
// BINDING_FUNCTION_PROTOTYPES(
//     void bind_acars(py::module& m);
// ) END BINDING_FUNCTION_PROTOTYPES
/**************************************/

// We need this helper because import_array() returns NULL
// for newer Python versions. This ensures we have access to
// the NumPy C-API without warnings or segmentation faults.
static void* init_numpy()
{
    import_array();  // Initialize NumPy C API
    return nullptr;
}

PYBIND11_MODULE(acars_python, m)
{
    // Initialize the NumPy C API
    init_numpy();

    // Ensure pybind knows about gnuradio.gr base classes/methods
    py::module::import("gnuradio.gr");

    /**************************************/
    // BINDING_FUNCTION_CALLS(
    bind_acars(m);
    // ) END BINDING_FUNCTION_CALLS
    /**************************************/
}
