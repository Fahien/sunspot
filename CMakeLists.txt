cmake_minimum_required(VERSION 3.7.2)
include(CMakeToolsHelpers OPTIONAL)
project(SunSpoT)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${PROJECT_SOURCE_DIR}/cmake")
message("Searching modules in ${CMAKE_MODULE_PATH}")

option(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

# Version number
set(SST_VERSION_MAJOR 0)
set(SST_VERSION_MINOR 14)
set(SST_TITLE "SunSpoT")

# Configure a header file to pass some of the CMake settings to the source code
configure_file(
	"${PROJECT_SOURCE_DIR}/src/Config.h.in"
	"${PROJECT_SOURCE_DIR}/include/Config.h")

# Specify the paths in which the linker should search for libraries
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}" "${PROJECT_SOURCE_DIR}")
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW REQUIRED)

set(SHADERS
	shader/base.vert
	shader/base.frag
	shader/depth.frag
	shader/quad.vert
	shader/quad.frag)
source_group("Shaders" FILES ${SHADERS})

set(SOURCES
	src/Logger.h
	src/Logger.cpp
	src/Graphics.h
	src/ShaderProgram.h
	src/ShaderProgram.cpp
	src/Math.h
	src/Math.cpp
	src/collision/BoundingBox.h
	src/collision/BoundingBox.cpp
	src/collision/CollisionSystem.h
	src/collision/CollisionSystem.cpp
	src/Cursor.h
	src/Cursor.cpp
	src/Window.h
	src/Window.cpp
	src/GlfwWindow.h
	src/GlfwWindow.cpp
	src/Texture.h
	src/Texture.cpp
	src/Ifstream.h
	src/Ifstream.cpp
	src/Material.h
	src/Material.cpp
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
source_group("Sources" FILES ${SOURCES})

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
add_executable(sunspot ${SOURCES} ${SHADERS})

# SOIL
set(SOIL soil)
add_subdirectory(${SOIL})
set(SOIL_INCLUDE_DIR ${SOIL}/src)
set(SOIL_LIBRARIES SOIL)

# Add the given directories to those the compiler uses to search for include files
if(${OPENGL_INCLUDE_DIR})
	target_include_directories(sunspot PUBLIC "${OPENGL_INCLUDE_DIR}")
endif()
target_include_directories(sunspot PUBLIC
	${GLEW_INCLUDE_DIR}
	${GLFW_INCLUDE_DIR}
	${SOIL_INCLUDE_DIR}
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/shader)

if(WIN32)
	set_target_properties(sunspot PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libcmt")
endif(WIN32)

target_link_libraries(sunspot ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${SOIL_LIBRARIES})

file(COPY ${PROJECT_SOURCE_DIR}/shader DESTINATION ${PROJECT_BINARY_DIR})
