cmake_minimum_required(VERSION 3.4.1)
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
	"${PROJECT_SOURCE_DIR}/source/Config.h.in"
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
	source/Logger.h
	source/Logger.cpp
	source/Graphics.h
	source/ShaderProgram.h
	source/ShaderProgram.cpp
	source/collision/BoundingBox.h
	source/collision/BoundingBox.cpp
	source/collision/CollisionSystem.h
	source/collision/CollisionSystem.cpp
	source/Cursor.h
	source/Cursor.cpp
	source/Window.h
	source/Window.cpp
	source/GlfwWindow.h
	source/GlfwWindow.cpp
	source/Texture.h
	source/Texture.cpp
	source/Ifstream.h
	source/Ifstream.cpp
	source/Material.h
	source/Material.cpp
	source/Mesh.h
	source/Mesh.cpp
	source/WavefrontObject.h
	source/WavefrontObject.cpp
	source/Color.h
	source/Color.cpp
	source/Material.h
	source/Light.h
	source/Light.cpp
	source/Quad.h
	source/Quad.cpp
	source/Framebuffer.h
	source/Framebuffer.cpp
	source/Camera.h
	source/Camera.cpp
	source/Main.cpp)
source_group("Sources" FILES ${SOURCES})

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
add_executable(sunspot ${SOURCES} ${SHADERS})

# MathSpoT
set(MATHSPOT mathspot)
add_subdirectory(${MATHSPOT})
set(MATHSPOT_INCLUDE_DIR ${MATHSPOT}/include)
set(MATHSPOT_LIBRARIES mathspot)

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
	${MATHSPOT_INCLUDE_DIR}
	${SOIL_INCLUDE_DIR}
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/shader)

if(WIN32)
	set_target_properties(sunspot PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libcmt")
endif(WIN32)

target_link_libraries(sunspot ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${MATHSPOT_LIBRARIES} ${SOIL_LIBRARIES})

file(COPY ${PROJECT_SOURCE_DIR}/shader DESTINATION ${PROJECT_BINARY_DIR})
