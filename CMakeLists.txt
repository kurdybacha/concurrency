# CMake version
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# Include cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#
# Project description and (meta) information
#

# Meta information about the project
set(META_PROJECT_NAME        "concurrency")
set(META_PROJECT_DESCRIPTION "Concurrency presentation")
set(META_AUTHOR_ORGANIZATION "Pawel Kurdybacha")
set(META_AUTHOR_DOMAIN       "https://github.com/kurdybacha/")
set(META_AUTHOR_MAINTAINER   "")
set(META_VERSION_MAJOR       "0")
set(META_VERSION_MINOR       "0")
set(META_VERSION_PATCH       "1")
set(META_VERSION             "${META_VERSION_MAJOR}.${META_VERSION_MINOR}.${META_VERSION_PATCH}")
set(META_NAME_VERSION        "${META_PROJECT_NAME} v${META_VERSION} (${META_VERSION_REVISION})")

string(MAKE_C_IDENTIFIER ${META_PROJECT_NAME} META_PROJECT_ID)
string(TOUPPER ${META_PROJECT_ID} META_PROJECT_ID)

#
# Project configuration options
#

# Project options
option(BUILD_SHARED_LIBS     "Build shared instead of static libraries."              ON)
option(BUILD_TESTS           "Build tests."                                           ON)

#
# Declare project
#
project(${META_PROJECT_NAME} C CXX)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

# Create version file
file(WRITE "${PROJECT_BINARY_DIR}/VERSION" "${META_NAME_VERSION}")

#
# Compiler settings and options
#
#include(cmake/CompileOptions.cmake)
set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(src)
add_subdirectory(tests)
