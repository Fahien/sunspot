cmake_minimum_required (VERSION 2.6)
project (StereoscopicTest)

option(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

# Version number
set (SST_VERSION_MAJOR 0)
set (SST_VERSION_MINOR 4)
set (SST_TITLE "StereoscopicTest")

# Configure a header file to pass some of the CMake settings to the source code
configure_file (
	"${PROJECT_SOURCE_DIR}/src/Config.h.in"
	"${PROJECT_BINARY_DIR}/Config.h"
)

# Add the binary tree to the search path for include files so that we will find config.h
include_directories ("${PROJECT_BINARY_DIR}")

set (SOURCES
	src/Window.cpp
	src/Model.cpp
	src/Quad.cpp
	src/ShaderProgram.cpp
	src/Framebuffer.cpp
	src/Main.cpp)

add_executable (stereoscopic-test ${SOURCES})

find_package (glfw3 3.2 REQUIRED)

target_link_libraries (stereoscopic-test GL GLEW glfw)
