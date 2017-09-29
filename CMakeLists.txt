cmake_minimum_required(VERSION 3.6)
include(CMakeToolsHelpers OPTIONAL)
project(SunSpoT)

#set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Wall")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -Wall")

if(${CMAKE_SYSTEM_NAME} MATCHES Android)
	set(ANDROID TRUE)
else()
	set(ANDROID FALSE)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${PROJECT_SOURCE_DIR}/cmake")
message("Searching modules in ${CMAKE_MODULE_PATH}")

option(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

# Version number
set(SST_VERSION_MAJOR 0)
set(SST_VERSION_MINOR 14)
set(SST_TITLE "SunSpoT")

# Configure a header file to pass some of the CMake settings to the source code
configure_file(
	"${PROJECT_SOURCE_DIR}/source/SunSpotConfig.h.in"
	"${PROJECT_SOURCE_DIR}/include/SunSpotConfig.h")

# Specify the paths in which the linker should search for libraries
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}" "${PROJECT_SOURCE_DIR}")

# Python 3
find_package(PythonInterp 3)
find_package(PythonLibs 3)

set(DEPENDENCY_INCLUDE_DIRECTORIES
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/include/entity
	${PROJECT_SOURCE_DIR}/include/component
	${PROJECT_SOURCE_DIR}/source/android
	${PROJECT_SOURCE_DIR}/source
	${PYTHON_INCLUDE_DIR})

if(${ANDROID})
	set(DEPENDENCY_LIBRARIES GLESv3 android EGL log m)
else()
	find_package(OpenGL REQUIRED)
	find_package(GLEW REQUIRED)
	find_package(GLFW REQUIRED)
	set(DEPENDENCY_LIBRARIES
		${OPENGL_LIBRARIES}
		${GLEW_LIBRARIES}
		${GLFW_LIBRARIES})
	set(DEPENCENDY_INCLUDE_DIRECTORIES
		${OPENGL_INCLUDE_DIR}
		${GLEW_INCLUDE_DIR}
		${GLFW_INCLUDE_DIR})
endif()


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
	source/android/AssetManager.h
	source/android/AssetManager.cpp
	source/android/ShaderProgram.h
	source/android/ShaderProgram.cpp
	source/collision/BoundingBox.h
	source/collision/BoundingBox.cpp
	source/collision/CollisionSystem.h
	source/collision/CollisionSystem.cpp
	source/Cursor.h
	source/Cursor.cpp
	source/Texture.h
	source/Texture.cpp
	source/Ifstream.h
	source/Ifstream.cpp
	source/Material.h
	source/Material.cpp
	source/Mesh.h
	source/Mesh.cpp
	source/entity/PyType.cpp
	source/entity/Script.cpp
	source/entity/Entity.cpp
	source/component/Transform.cpp
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
	source/Camera.cpp)

if(NOT ${ANDROID})
	set(SOURCES ${SOURCES}
		source/Window.h
		source/Window.cpp
		source/GlfwWindow.h
		source/GlfwWindow.cpp
		source/Main.cpp)
endif()

source_group("Sources" FILES ${SOURCES})

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
if(${ANDROID})
	add_library(sunspot ${SOURCES} ${SHADERS})
else()
	add_executable(sunspot ${SOURCES} ${SHADERS})
endif()

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

# PySpot
set(PYSPOT pyspot)
add_subdirectory(${PYSPOT})
set(PYSPOT_INCLUDE_DIR ${PYSPOT}/include)
set(PYSPOT_LIBRARIES ${PYSPOT})

# Add the given directories to those the compiler uses to search for include files

target_include_directories(sunspot PUBLIC
	${MATHSPOT_INCLUDE_DIR}
	${SOIL_INCLUDE_DIR}
	${PYSPOT_INCLUDE_DIR}
	${DEPENDENCY_INCLUDE_DIRECTORIES})

set(DEPENDENCY_LIBRARIES
	${DEPENDENCY_LIBRARIES}
	${MATHSPOT_LIBRARIES}
	${SOIL_LIBRARIES}
	${PYTHON_LIBRARIES}
	${PYSPOT_LIBRARIES})

if(WIN32)
	set_target_properties(sunspot PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libcmt")
endif(WIN32)

set_target_properties(sunspot PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(sunspot ${DEPENDENCY_LIBRARIES})

file(COPY ${PROJECT_SOURCE_DIR}/shader DESTINATION ${PROJECT_BINARY_DIR})
