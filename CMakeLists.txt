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
	"${PROJECT_SOURCE_DIR}/include/Config.h"
)

# Add the given directories to those the compiler uses to search for include files
include_directories ("${PROJECT_SOURCE_DIR}/include", "${PROJECT_SOURCE_DIR}/shader")

# Specify the paths in which the linker should search for libraries
link_directories("${PROJECT_SOURCE_DIR}/lib")

set (SOURCES
	src/Graphics.h
	src/Math.h
	src/Math.cpp
	src/Window.h
	src/Window.cpp
	src/GlfwWindow.h
	src/GlfwWindow.cpp
	src/SdlWindow.h
	src/SdlWindow.cpp
	src/Texture.h
	src/Texture.cpp
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

target_link_libraries (stereoscopic-test opengl32 glew32s glfw3 SDL2 SDL2main)