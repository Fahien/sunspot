cmake_minimum_required (VERSION 2.6)
project (StereoscopicTest)

# Version number
set (SST_VERSION_MAJOR 0)
set (SST_VERSION_MINOR 1)

# Configure a header file to pass some of the CMake settings to the source code
configure_file (
	"${PROJECT_SOURCE_DIR}/config.h.in"
	"${PROJECT_BINARY_DIR}/config.h"
)

# Add the binary tree to the search path for include files so that we will find config.h
include_directories("${PROJECT_BINARY_DIR}")

add_executable (stereoscopic-test stereoscopic-test.c)
