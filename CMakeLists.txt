cmake_minimum_required (VERSION 2.8)
project (StereoscopicTest)

option (CMAKE_EXPORT_COMPILE_COMMANDS "ON")

# Version number
set (SST_VERSION_MAJOR 0)
set (SST_VERSION_MINOR 6)
set (SST_TITLE "StereoscopicTest")

# Configure a header file to pass some of the CMake settings to the source code
configure_file (
	"${PROJECT_SOURCE_DIR}/src/Config.h.in"
	"${PROJECT_SOURCE_DIR}/include/Config.h")

# Add the given directories to those the compiler uses to search for include files
include_directories (
	"${PROJECT_SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/shader")

# Specify the paths in which the linker should search for libraries
link_directories ("${PROJECT_SOURCE_DIR}/lib")

set (SHADERS
	shader/base.vert
	shader/base.frag
	shader/depth.frag
	shader/quad.vert
	shader/quad.frag)
source_group ("Shaders" FILES ${SHADERS} )

set (SOURCES
	src/Graphics.h
	src/ShaderProgram.h
	src/ShaderProgram.cpp
	src/Math.h
	src/Math.cpp
	src/Cursor.h
	src/Cursor.cpp
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
	src/Color.h
	src/Color.cpp
	src/Light.h
	src/Light.cpp
	src/Quad.h
	src/Quad.cpp
	src/Framebuffer.h
	src/Framebuffer.cpp
	src/Camera.h
	src/Camera.cpp
	src/Main.cpp)
source_group ("Sources" FILES ${SOURCES})

add_executable (stereoscopic-test ${SOURCES} ${SHADERS})

target_link_libraries (stereoscopic-test opengl32 glew32s glfw3 SDL2 SDL2main)

file(COPY ${PROJECT_SOURCE_DIR}/shader DESTINATION ${PROJECT_BINARY_DIR})