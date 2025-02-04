#
# Copyright 2008,2009 Free Software Foundation, Inc.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

"""
This is the GNU Radio ACARS module.
It provides blocks and utilities for decoding ACARS signals.
"""

import os

# Attempt to import the compiled Python bindings (pybind11 or SWIG-generated),
# which might not exist if the module is pure Python only.
try:
    from .acars_python import *
except ModuleNotFoundError:
    # If the library isn't found, you may leave this as a silent pass
    pass

# If you have pure-Python classes or functions, import or define them here:
# from .my_python_block import MyPythonBlock
