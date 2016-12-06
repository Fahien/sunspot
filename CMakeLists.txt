cmake_minimum_required (VERSION 2.8)
project (StereoscopicTest)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${PROJECT_SOURCE_DIR}/cmake")
message("Searching modules in ${CMAKE_MODULE_PATH}")

option (CMAKE_EXPORT_COMPILE_COMMANDS "ON")

# Version number
set (SST_VERSION_MAJOR 0)
set (SST_VERSION_MINOR 8)
set (SST_TITLE "StereoscopicTest")

# Configure a header file to pass some of the CMake settings to the source code
configure_file (
	"${PROJECT_SOURCE_DIR}/src/Config.h.in"
	"${PROJECT_SOURCE_DIR}/include/Config.h")

find_package (OpenGL REQUIRED)

set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}" "${PROJECT_SOURCE_DIR}" )
find_package (GLEW REQUIRED)
find_package (GLFW REQUIRED)
find_package (SDL2 REQUIRED)

if (${OPENGL_FOUND})
	message("Found OpenGL ${OPENGL_INCLUDE_DIR} ${OPENGL_LIBRARIES}")
endif()

if (${GLEW_FOUND})
	message("Found GLEW ${GLEW_INCLUDE_DIR} ${GLEW_LIBRARIES}")
endif()

if (${GLFW_FOUND})
	message("Found GLFW ${GLFW_INCLUDE_DIR} ${GLFW_LIBRARIES}")
endif()

if (${SDL2_FOUND})
	message ("Found SDL2 ${SDL2_INCLUDE_DIR} ${SDL2_LIBRARY}")
endif()

# Add the given directories to those the compiler uses to search for include files
if (${OPENGL_INCLUDE_DIR})
	include_directories ("${OPENGL_INCLUDE_DIR}")
endif()
include_directories (
	"${GLEW_INCLUDE_DIR}"
	"${GLFW_INCLUDE_DIR}"
	"${SDL2_INCLUDE_DIR}"
	"${PROJECT_SOURCE_DIR}/include"
	"${PROJECT_SOURCE_DIR}/shader")

# Specify the paths in which the linker should search for libraries

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
	src/Mesh.h
	src/Mesh.cpp
	src/WavefrontObject.h
	src/WavefrontObject.cpp
	src/Color.h
	src/Color.cpp
	src/Material.h
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

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
add_executable (stereoscopic-test ${SOURCES} ${SHADERS})

target_link_libraries (stereoscopic-test ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES}
		${SDL2_LIBRARY})

file(COPY ${PROJECT_SOURCE_DIR}/shader DESTINATION ${PROJECT_BINARY_DIR})
