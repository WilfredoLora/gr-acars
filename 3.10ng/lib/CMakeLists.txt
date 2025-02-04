########################################################################
# lib/CMakeLists.txt for gr-acars
########################################################################

# Only proceed if we actually have C++ source files in this directory
# (acars_impl.cc, etc.)
list(APPEND acars_sources
    acars_impl.cc
    # Add more .cc files here if needed
)

if(NOT acars_sources)
    message(STATUS "No C++ sources found in lib/. Skipping build of gnuradio-acars library.")
    return()
endif()

########################################################################
# Create the gnuradio-acars library
########################################################################
add_library(gnuradio-acars SHARED ${acars_sources})
set_target_properties(gnuradio-acars PROPERTIES
    DEFINE_SYMBOL "gnuradio_acars_EXPORTS"
    # Optional: versioning, SOVERSION, etc.
    # VERSION   ${PROJECT_VERSION}
    # SOVERSION ${PROJECT_VERSION_MAJOR}
)

# Link against the GNU Radio libraries you actually use.
# Currently linking to runtime and fft. Add more if needed:
target_link_libraries(gnuradio-acars
    PUBLIC
        gnuradio::gnuradio-runtime
        gnuradio::gnuradio-fft
)

# Ensure that when building, we include headers from ../include,
# and when installing, they go to include/
target_include_directories(gnuradio-acars
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        $<INSTALL_INTERFACE:include>
)

########################################################################
# Special handling for macOS (rpath/install_name)
########################################################################
if(APPLE)
    set_target_properties(gnuradio-acars PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif()

########################################################################
# Install this library
########################################################################
# Use the standard GR macro to handle library installation and symlinks
include(GrMiscUtils)
GR_LIBRARY_FOO(gnuradio-acars)

########################################################################
# Optionally, define unit tests
########################################################################
include(GrTest)

list(APPEND test_acars_sources
    # Add any test .cc files if you have unit tests, e.g.:
    # qa_acars_test.cc
)

list(APPEND GR_TEST_TARGET_DEPS
    gnuradio-acars
)

if(NOT test_acars_sources)
    message(STATUS "No C++ unit tests in lib/. Skipping")
    return()
endif()

foreach(qa_file ${test_acars_sources})
    GR_ADD_CPP_TEST("acars_${qa_file}"
        ${CMAKE_CURRENT_SOURCE_DIR}/${qa_file}
    )
endforeach(qa_file)
