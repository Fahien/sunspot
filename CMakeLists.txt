cmake_minimum_required(VERSION 3.6)
include(CMakeToolsHelpers OPTIONAL)
project(SunSpoT)

set(CMAKE_C_STANDARD   11)
set(CMAKE_CXX_STANDARD 14)

if(${CMAKE_SYSTEM_NAME} MATCHES Android)
	set(ANDROID TRUE)
else()
	set(ANDROID FALSE)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${PROJECT_SOURCE_DIR}/cmake")
message("Searching modules in ${CMAKE_MODULE_PATH}")

option(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

# Python 3
find_package(PythonInterp 3)
find_package(PythonLibs   3)

# LogSpoT
set(LOGSPOT logspot)
add_subdirectory(${LOGSPOT})
set(LOGSPOT_INCLUDE_DIR ${LOGSPOT}/include)
set(LOGSPOT_LIBRARIES ${LOGSPOT})

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

# PySpoT
set(PYSPOT pyspot)
add_subdirectory(${PYSPOT})
set(PYSPOT_INCLUDE_DIR ${PYSPOT}/include)
set(PYSPOT_LIBRARIES ${PYSPOT})

# DataSpoT
set(DATASPOT dataspot)
add_subdirectory(${DATASPOT})
set(DATASPOT_INCLUDE_DIR ${DATASPOT}/include)
set(DATASPOT_LIBRARIES ${DATASPOT})

# GltfSpoT
set(GLTFSPOT gltfspot)
add_subdirectory(${GLTFSPOT})
set(GLTFSPOT_INCLUDE_DIR ${GLTFSPOT}/include)
set(GLTFSPOT_LIBRARIES ${GLTFSPOT})

# HitSpoT
set(HITSPOT hitspot)
add_subdirectory(${HITSPOT})
set(HITSPOT_INCLUDE_DIR ${HITSPOT}/include)
set(HITSPOT_LIBRARIES ${HITSPOT})

# Version number
set(SST_VERSION_MAJOR 0)
set(SST_VERSION_MINOR 14)
set(SST_TITLE "SunSpoT")
set(SST_NAME sunspot)

# Configure a header file to pass some of the CMake settings to the source code
configure_file(
	"${PROJECT_SOURCE_DIR}/source/SunSpotConfig.h.in"
	"${PROJECT_SOURCE_DIR}/include/SunSpotConfig.h")

# Specify the paths in which the linker should search for libraries
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}" "${PROJECT_SOURCE_DIR}")


set(DEPENDENCY_INCLUDE_DIRECTORIES
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/source
	${PROJECT_SOURCE_DIR}/source/android
	${PROJECT_SOURCE_DIR}/source/entity
	${PROJECT_SOURCE_DIR}/source/repository
	${PROJECT_BINARY_DIR}/include
	${PROJECT_BINARY_DIR}/include/sunspot
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


# Generate the Sunspot Extension header
set(SST_EXTENSION_NAME "Sunspot")
set(SST_EXTENSION_JSON ${PROJECT_SOURCE_DIR}/extension/sunspot/${SST_EXTENSION_NAME}.json)
set(SST_EXTENSION_HEADER ${PROJECT_BINARY_DIR}/include/sunspot/extension/${SST_EXTENSION_NAME}.h)
set(SST_EXTENSION_SOURCE ${PROJECT_BINARY_DIR}/src/sunspot/extension/${SST_EXTENSION_NAME}.cpp)
add_custom_command(
	OUTPUT ${SST_EXTENSION_HEADER} ${SST_EXTENSION_SOURCE}
	COMMAND ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/pyspot/script/generate-extension.py ${SST_EXTENSION_JSON} -h ${SST_EXTENSION_HEADER}
	COMMAND ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/pyspot/script/generate-extension.py ${SST_EXTENSION_JSON} ${SST_EXTENSION_SOURCE}
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/pyspot/script
	COMMENT "Generating ${SST_EXTENSION_HEADER}"
	VERBATIM)


# Get all the folders under test/extension/pytest
set(EXTENSION_NAME sunspot)
set(EXTENSION_PATH ${PROJECT_SOURCE_DIR}/extension/${EXTENSION_NAME})
file(GLOB NAMESPACES LIST_DIRECTORY true ${EXTENSION_PATH}/*)
foreach(NAMESPACE ${NAMESPACES})
	message(STATUS "CHECK ${NAMESPACE}")
	if(IS_DIRECTORY ${NAMESPACE})
		# Get all the jsons and generate headers
		file(GLOB COMPONENTS ${NAMESPACE}/*json)
		foreach(COMPONENT ${COMPONENTS})
			get_filename_component(NAMESPACE_NAME ${NAMESPACE} NAME_WE)
			get_filename_component(NAME ${COMPONENT} NAME_WE)
			list(APPEND COMPONENT_NAMES ${NAME})
			set(COMPONENT_HEADER ${PROJECT_BINARY_DIR}/include/${EXTENSION_NAME}/${NAMESPACE_NAME}/${NAME}.h)
			list(APPEND COMPONENT_HEADERS ${COMPONENT_HEADER})
			add_custom_command(
				OUTPUT ${COMPONENT_HEADER}
				COMMAND ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/pyspot/script/generate-component.py ${EXTENSION_NAME} ${COMPONENT} ${COMPONENT_HEADER}
				WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/pyspot/script
				COMMENT "Generating ${COMPONENT_HEADER}"
				VERBATIM)
		endforeach()
	endif()
endforeach()


set(SOURCES
	${COMPONENT_HEADERS}
	${SST_EXTENSION_HEADER}
	${SST_EXTENSION_SOURCE}
	source/Graphics.h
	source/android/AssetManager.h
	source/android/AssetManager.cpp
	source/android/ShaderProgram.h
	source/android/ShaderProgram.cpp
	source/system/Collision.h
	source/system/Collision.cpp
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
	source/component/Model.h
	source/entity/Script.h
	source/entity/Script.cpp
	source/entity/Object.h
	source/entity/Object.cpp
	source/entity/Entity.h
	source/entity/Entity.cpp
	source/repository/ModelRepository.h
	source/repository/ModelRepository.cpp
	source/repository/EntityRepository.h
	source/repository/EntityRepository.cpp
	source/view/GltfPrimitive.h
	source/view/GltfPrimitive.cpp
	source/view/GltfMesh.h
	source/view/GltfMesh.cpp
	source/view/GltfRenderer.h
	source/view/GltfRenderer.cpp
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

add_library(${SST_NAME}-lib ${SOURCES} ${SHADERS})

add_executable(${SST_NAME} ${SOURCES} ${SHADERS})



# Add the given directories to those the compiler uses to search for include files

target_include_directories(${SST_NAME}-lib PUBLIC
	${LOGSPOT_INCLUDE_DIR}
	${MATHSPOT_INCLUDE_DIR}
	${SOIL_INCLUDE_DIR}
	${PYSPOT_INCLUDE_DIR}
	${DATASPOT_INCLUDE_DIR}
	${GLTFSPOT_INCLUDE_DIR}
	${HITSPOT_INCLUDE_DIR}
	${DEPENDENCY_INCLUDE_DIRECTORIES})

set(DEPENDENCY_LIBRARIES
	${DEPENDENCY_LIBRARIES}
	${LOGSPOT_LIBRARIES}
	${MATHSPOT_LIBRARIES}
	${SOIL_LIBRARIES}
	${PYTHON_LIBRARIES}
	${PYSPOT_LIBRARIES}
	${DATASPOT_LIBRARIES}
	${GLTFSPOT_LIBRARIES}
	${HITSPOT_LIBRARIES})

if(WIN32)
	set_target_properties(${SST_NAME}-lib PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libcmt")
endif(WIN32)

set_target_properties(${SST_NAME}-lib PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${SST_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${SST_NAME}-lib ${DEPENDENCY_LIBRARIES})
target_link_libraries(${SST_NAME} ${SST_NAME}-lib)

file(COPY ${PROJECT_SOURCE_DIR}/shader DESTINATION ${PROJECT_BINARY_DIR})

# Test
set(SST_TEST_DIR ${PROJECT_SOURCE_DIR}/test)
include(${SST_TEST_DIR}/Tests.cmake)
