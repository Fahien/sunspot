cmake_minimum_required(VERSION 3.6)
project(SunSpot)

set(CMAKE_C_STANDARD   11)
set(CMAKE_CXX_STANDARD 14)

if(${CMAKE_SYSTEM_NAME} MATCHES "Android")
	set(ANDROID TRUE)
	add_definitions(-DANDROID)
else()
	set(ANDROID FALSE)
endif()

# CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)
message(STATUS "Searching modules in ${CMAKE_MODULE_PATH}")

option(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

# Python 3
find_package(PythonInterp 3)
find_package(PythonLibraries)

include(AddLogSpot)
include(AddMathSpot)
include(AddFileSpot)
include(AddPySpot)
include(AddDataSpot)
include(AddGltfSpot)
include(AddHitSpot)

# SOIL
set(SOIL soil)
add_subdirectory(${SOIL})
set(SOIL_INCLUDE_DIR ${SOIL}/src)
set(SOIL_LIBRARIES SOIL)

# Version number
set(SST_VERSION_MAJOR 0)
set(SST_VERSION_MINOR 15)
set(SST_TITLE SunSpot)
set(SST_NAME sunspot)

# Configure a header file to pass some of the CMake settings to the source code
configure_file(${PROJECT_SOURCE_DIR}/source/SunSpotConfig.h.in
               ${PROJECT_SOURCE_DIR}/include/SunSpotConfig.h)


set(DEPENDENCY_INCLUDE_DIRS
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/source
	${PROJECT_BINARY_DIR}/include
	${PROJECT_BINARY_DIR}/include/sunspot
	${SOIL_INCLUDE_DIR})

if(ANDROID)
	set(DEPENDENCY_LIBRARIES GLESv3 android EGL log m)
else()
	find_package(OpenGL REQUIRED)
	find_package(GLEW   REQUIRED)
	find_package(GLFW   REQUIRED)
	set(DEPENDENCY_LIBRARIES
		${OPENGL_LIBRARIES}
		${GLEW_LIBRARIES}
		${GLFW_LIBRARIES})
	set(DEPENCENDY_INCLUDE_DIRS
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
source_group("shader" FILES ${SHADERS})


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
	source/android/ShaderProgram.h
	source/android/ShaderProgram.cpp
	source/system/Collision.h
	source/system/Collision.cpp
	source/Cursor.h
	source/Cursor.cpp
	source/Texture.h
	source/Texture.cpp
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

if(ANDROID)
	set(ANDROID_SOURCES
		source/android/SunSpot.cpp
		source/android/Renderer.cpp)
else()
	set(SOURCES ${SOURCES}
		source/Window.h
		source/Window.cpp
		source/GlfwWindow.h
		source/GlfwWindow.cpp
		source/Main.cpp)
endif()

source_group("src" FILES ${SOURCES})


set(DEPENDENCY_LIBRARIES
	${DEPENDENCY_LIBRARIES}
	${LOGSPOT_LIBRARIES}
	${MATHSPOT_LIBRARIES}
	${FILESPOT_LIBRARIES}
	${SOIL_LIBRARIES}
	${PYSPOT_LIBRARIES}
	${DATASPOT_LIBRARIES}
	${GLTFSPOT_LIBRARIES}
	${HITSPOT_LIBRARIES})

# SunSpot library
add_library(${SST_NAME}-lib ${SOURCES} ${SHADERS})
target_include_directories(${SST_NAME}-lib PUBLIC ${DEPENDENCY_INCLUDE_DIRS})
target_link_libraries(${SST_NAME}-lib ${DEPENDENCY_LIBRARIES})

if(WIN32)
	set_target_properties(${SST_NAME}-lib PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libcmt")
endif(WIN32)

# SunSpot target
if(ANDROID)
	add_library(${SST_NAME} SHARED ${ANDROID_SOURCES})
else()
	add_executable(${SST_NAME} ${SOURCES} ${SHADERS})
endif()

target_link_libraries(${SST_NAME} ${SST_NAME}-lib)


file(COPY ${PROJECT_SOURCE_DIR}/shader DESTINATION ${PROJECT_BINARY_DIR})

# Test
set(SST_TEST_DIR ${PROJECT_SOURCE_DIR}/test)
include(${SST_TEST_DIR}/Tests.cmake)
