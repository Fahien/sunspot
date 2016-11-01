cmake_minimum_required (VERSION 2.6)
project (StereoscopicTest)

option(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

# Version number
set (SST_VERSION_MAJOR 0)
set (SST_VERSION_MINOR 5)
set (SST_TITLE "StereoscopicTest")

# Configure a header file to pass some of the CMake settings to the source code
configure_file (
	"${PROJECT_SOURCE_DIR}/src/Config.h.in"
	"${PROJECT_BINARY_DIR}/Config.h"
)

# Add the binary tree to the search path for include files so that we will find config.h
include_directories ("${PROJECT_BINARY_DIR}")

set (SOURCES
	src/Window.h
	src/Window.cpp
	src/Model.h
	src/Model.cpp
	src/Quad.h
	src/Quad.cpp
	src/ShaderProgram.h
	src/ShaderProgram.cpp
	src/Framebuffer.h
	src/Framebuffer.cpp
	src/Camera.h
	src/Camera.cpp
	src/Main.cpp)

add_executable (stereoscopic-test ${SOURCES})

find_package(OpenGL REQUIRED)

target_link_libraries (stereoscopic-test glew32s glfw3)
