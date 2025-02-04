#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
#
# bind_oot_file.py
#
# A script to generate Python bindings for a GNU Radio Out-Of-Tree (OOT) block
# using gnuradio.bindtool.BindingGenerator. Typically invoked by CMake.

import os
import sys
import argparse
import warnings
import tempfile
from gnuradio.bindtool import BindingGenerator

def main():
    parser = argparse.ArgumentParser(
        description='Bind a GNU Radio OOT block file to Python using pybind11.'
    )

    parser.add_argument(
        '--module', type=str, required=True,
        help='Name of GR module (e.g. "fft", "digital", "analog", or your OOT name).'
    )
    parser.add_argument(
        '--filename', required=True,
        help="C++ header file (.h) to parse and generate bindings from."
    )
    parser.add_argument(
        '--prefix', required=True,
        help='Install prefix of GNU Radio (e.g. /usr, /usr/local).'
    )
    parser.add_argument(
        '--src', default=os.path.dirname(os.path.abspath(__file__)) + '/../../..',
        help='Root directory of the gnuradio source tree (rarely needed in OOT).'
    )
    parser.add_argument(
        '--output_dir', default=tempfile.gettempdir(),
        help='Output directory for the generated binding files.'
    )
    parser.add_argument(
        '--defines', default=(), nargs='*',
        help='Additional preprocessor definitions, separated by spaces.'
    )
    parser.add_argument(
        '--include', default=(), nargs='*',
        help='Additional include directories, separated by spaces.'
    )
    parser.add_argument(
        '--status', default=None,
        help='Path to an output file for binding status (used during cmake).'
    )
    parser.add_argument(
        '--flag_automatic', default='0',
        help='Indicates if this binding generation is automatic (0 or 1).'
    )
    parser.add_argument(
        '--flag_pygccxml', default='0',
        help='Use pygccxml (0 or 1). Typically unused for pybind generation.'
    )

    args = parser.parse_args()

    # Convert the 'defines' and 'include' lists into comma-separated strings
    define_symbols = tuple(','.join(args.defines).split(','))
    addl_includes = ','.join(args.include)

    # The first part of the namespace is always "gr", the second is your module
    namespace = ['gr', args.module]

    # Typically, the prefix_include_root is the same as your module name
    prefix_include_root = args.module

    # Suppress certain deprecation warnings that can appear in parsing
    with warnings.catch_warnings():
        warnings.filterwarnings("ignore", category=DeprecationWarning)

        bg = BindingGenerator(
            prefix=args.prefix,
            namespace_list=namespace,
            prefix_include_root=prefix_include_root,
            output_dir=args.output_dir,
            define_symbols=define_symbols,
            addl_includes=addl_includes,
            catch_exceptions=False,
            write_json_output=False,
            status_output=args.status,
            flag_automatic=(args.flag_automatic.lower() in ['1', 'true']),
            flag_pygccxml=(args.flag_pygccxml.lower() in ['1', 'true'])
        )

        # Actually generate the binding code from the specified header
        bg.gen_file_binding(args.filename)

if __name__ == "__main__":
    sys.exit(main())
