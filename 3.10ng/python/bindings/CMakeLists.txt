#
# SPDX-License-Identifier: GPL-3.0-or-later
#
# python/bindings/CMakeLists.txt for gr-acars
# Handles the generation of pybind11 bindings from C++ source files.
#

########################################################################
# 1) Ensure we have C++ sources defined (acars_sources)
########################################################################
if(NOT acars_sources)
    message(STATUS "No C++ sources in acars_sources... skipping python bindings generation.")
    return()
endif()

########################################################################
# 2) Check for pygccxml (optional, but used in older bindtool workflows)
########################################################################
GR_PYTHON_CHECK_MODULE_RAW(
    "pygccxml"
    "import pygccxml"
    PYGCCXML_FOUND
)

include(GrPybind)  # Provides GR_PYBIND_MAKE_OOT macro, etc.

########################################################################
# 3) List your pybind source files
########################################################################
# Typically includes:
#  - One file with the PYBIND11_MODULE(...) definition (python_bindings.cc)
#  - Additional files for each block or module (e.g. acars_python.cc)
list(APPEND acars_python_files
    acars_python.cc
    python_bindings.cc
)

########################################################################
# 4) Generate the pybind module
########################################################################
# GR_PYBIND_MAKE_OOT(<module_name> <relative_path_to_top_level> <c++ namespace> <list_of_sources>)
#
# - <module_name> is the library name (e.g. "acars").
# - <relative_path_to_top_level> typically "../.." if this CMakeLists is in python/bindings.
# - <c++ namespace> is the namespace, e.g. gr::acars
# - <list_of_sources> is the .cc files that define your bindings.

GR_PYBIND_MAKE_OOT(
    acars                 # module_name
    ../..                 # relative path to top-level
    gr::acars             # C++ namespace
    "${acars_python_files}"
)

# The above command creates a target called "acars_python" (by default),
# which compiles to a shared object "acars_python.so"

########################################################################
# 5) Copy the bindings .so file for QA tests (optional)
########################################################################
# Some OOT modules copy .so files to a local test directory so that
# Python QA scripts can import them without installing. This is optional.
add_custom_target(
  copy_bindings_for_tests ALL
  COMMAND
    ${CMAKE_COMMAND} -E copy
      "${CMAKE_CURRENT_BINARY_DIR}/*.so"
      "${CMAKE_BINARY_DIR}/test_modules/acars/"
  DEPENDS acars_python
)

########################################################################
# 6) Install the generated module
########################################################################
install(
    TARGETS acars_python
    DESTINATION ${GR_PYTHON_DIR}/acars
    COMPONENT pythonapi
)
