cmake_minimum_required(VERSION 3.10)
project(gr-acars CXX C)

# Enable tests
enable_testing()

# Install to PyBOMBS target prefix if defined
if(DEFINED ENV{PYBOMBS_PREFIX})
    set(CMAKE_INSTALL_PREFIX $ENV{PYBOMBS_PREFIX})
    message(STATUS "PyBOMBS installed GNU Radio. Setting CMAKE_INSTALL_PREFIX to $ENV{PYBOMBS_PREFIX}")
endif()

# Default build type is Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
    message(STATUS "Build type not specified: defaulting to Release.")
endif()
set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "")

# Ensure our local CMake modules come first
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake/Modules)

# Version info
set(VERSION_MAJOR 1)
set(VERSION_API   0)
set(VERSION_ABI   0)
set(VERSION_PATCH 0)

# For older CMake policies
cmake_policy(SET CMP0011 NEW)

# Generate compile_commands.json for IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

########################################################################
# Compiler-specific setup
########################################################################
if((CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
     CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
     AND NOT WIN32)
    # http://gcc.gnu.org/wiki/Visibility
    add_definitions(-fvisibility=hidden)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
else()
    message(WARNING
        "C++ standard could not be set because the compiler is not GNU, Clang, or MSVC.")
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_C_STANDARD_REQUIRED ON)
elseif(CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_C_STANDARD_REQUIRED ON)
elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_C_STANDARD_REQUIRED ON)
else()
    message(WARNING
        "C standard could not be set because the compiler is not GNU, Clang, or MSVC.")
endif()

########################################################################
# Locate GNU Radio 3.10 components
########################################################################
find_package(Gnuradio "3.10" REQUIRED
    COMPONENTS
        runtime
        blocks
        filter
        fft
        # add other components if your code uses them
)

include(GrVersion)
include(GrPlatform) # defines LIB_SUFFIX, etc.

# If you have custom build logic, you can link your targets inside
# the subdirectories, for example:
#   target_link_libraries(yourlibrary
#       PUBLIC
#       gnuradio::gnuradio-runtime
#       gnuradio::gnuradio-blocks
#       gnuradio::gnuradio-fft
#   )

# By default, we do NOT link libraries here at the top level.
# Link them in the relevant subdirectory (lib/ or apps/).

########################################################################
# Define install directories
########################################################################
if(NOT CMAKE_MODULES_DIR)
    set(CMAKE_MODULES_DIR lib${LIB_SUFFIX}/cmake)
endif()

set(GR_INCLUDE_DIR      include/acars)
set(GR_CMAKE_DIR        ${CMAKE_MODULES_DIR}/gnuradio-acars)
set(GR_PKG_DATA_DIR     ${GR_DATA_DIR}/${CMAKE_PROJECT_NAME})
set(GR_PKG_DOC_DIR      ${GR_DOC_DIR}/${CMAKE_PROJECT_NAME})
set(GR_PKG_CONF_DIR     ${GR_CONF_DIR}/${CMAKE_PROJECT_NAME}/conf.d)
set(GR_PKG_LIBEXEC_DIR  ${GR_LIBEXEC_DIR}/${CMAKE_PROJECT_NAME})

########################################################################
# On Apple only, set install name / rpath
########################################################################
if(APPLE)
    if(NOT CMAKE_INSTALL_NAME_DIR)
        set(CMAKE_INSTALL_NAME_DIR
            ${CMAKE_INSTALL_PREFIX}/${GR_LIBRARY_DIR} CACHE
            PATH "Library Install Name Destination Directory" FORCE)
    endif()

    if(NOT CMAKE_INSTALL_RPATH)
        set(CMAKE_INSTALL_RPATH
            ${CMAKE_INSTALL_PREFIX}/${GR_LIBRARY_DIR} CACHE
            PATH "Library Install RPath" FORCE)
    endif()

    if(NOT CMAKE_BUILD_WITH_INSTALL_RPATH)
        set(CMAKE_BUILD_WITH_INSTALL_RPATH ON CACHE
            BOOL "Do Build Using Library Install RPath" FORCE)
    endif()
endif()

########################################################################
# Doxygen (optional)
########################################################################
find_package(Doxygen)
if(DOXYGEN_FOUND)
    option(ENABLE_DOXYGEN "Build docs using Doxygen" ON)
else()
    option(ENABLE_DOXYGEN "Build docs using Doxygen" OFF)
endif()

########################################################################
# Create uninstall target
########################################################################
configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
    @ONLY
)

add_custom_target(uninstall
    ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)

########################################################################
# Add subdirectories
########################################################################
add_subdirectory(include/acars)
add_subdirectory(lib)
add_subdirectory(apps)
add_subdirectory(docs)

# NOTE: If your code has any Python blocks or GRC bindings, ensure
#       ENABLE_PYTHON is set if you want to build them:
if(ENABLE_PYTHON)
    message(STATUS "PYTHON and GRC components are enabled")
    add_subdirectory(python)
    add_subdirectory(grc)
else()
    message(STATUS "PYTHON and GRC components are disabled")
endif()
