cmake_minimum_required(VERSION 3.6)
project(SunSpot LANGUAGES C CXX)

set(CMAKE_C_STANDARD   11)
set(CMAKE_CXX_STANDARD 17)

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
add_definitions("-DPYTHON_VERSION=${PYTHON_VERSION}")

# Modules
set(MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/module)

include(AddLogSpot)
include(AddMathSpot)
include(AddFileSpot)
include(AddPySpot)
include(AddDataSpot)
include(AddGltfSpot)
include(AddHitSpot)

# SOIL
set(SOIL soil)
set(SOIL_DIR ${MODULE_DIR}/${SOIL})
add_subdirectory(${SOIL_DIR})
set(SOIL_INCLUDE_DIR ${SOIL_DIR}/src)
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
	${PROJECT_SOURCE_DIR}/include/sunspot
	${PROJECT_SOURCE_DIR}/source
	${PROJECT_BINARY_DIR}/include
	${PROJECT_BINARY_DIR}/src
	${SOIL_INCLUDE_DIR})

if(ANDROID)
	set(DEPENDENCY_LIBRARIES GLESv3 android EGL log m)
	list(APPEND DEPENDENCY_INCLUDE_DIRS ${ANDROID_NDK}/sources)
else()
	list(APPEND CMAKE_PREFIX_PATH
		${CMAKE_CURRENT_SOURCE_DIR}/include
		${CMAKE_CURRENT_SOURCE_DIR}/lib)

	find_package(OpenGL REQUIRED)
	find_package(GLEW   REQUIRED)
	find_package(GLFW   REQUIRED)
	set(DEPENDENCY_LIBRARIES
		${OPENGL_LIBRARIES}
		${GLEW_LIBRARIES}
		${GLFW_LIBRARIES})
	list(APPEND DEPENDENCY_INCLUDE_DIRS
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


# Pyspot extension files
set(GEN_SOURCES
	${CMAKE_CURRENT_BINARY_DIR}/include/pyspot/Bindings.h
	${CMAKE_CURRENT_BINARY_DIR}/include/pyspot/Extension.h
	${CMAKE_CURRENT_BINARY_DIR}/src/pyspot/Bindings.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/pyspot/Extension.cpp
)
# Join function
FUNCTION(JOIN VALUES GLUE OUTPUT)
	STRING(REGEX REPLACE "([^\\]|^);" "\\1${GLUE}" _TMP_STR "${VALUES}")
	STRING(REGEX REPLACE "[\\](.)" "\\1" _TMP_STR "${_TMP_STR}") #fixes escaping
	SET(${OUTPUT} "${_TMP_STR}" PARENT_SCOPE)
ENDFUNCTION()

# Sources to export
set(EXPORT_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/include/sunspot/component/Rigidbody.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/sunspot/component/Transform.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/sunspot/input/Action.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/sunspot/input/Input.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/sunspot/input/Key.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/sunspot/input/Type.h
	${MATHSPOT_INCLUDE_DIR}/mathspot/Math.h
)
# Generation command
JOIN("${DEPENDENCY_INCLUDE_DIRS};${MATHSPOT_INCLUDE_DIRS}" "\" -I\"" GEN_INCLUDE_DIRS)
set(GEN_INCLUDE_DIRS "\"${GEN_INCLUDE_DIRS}\"")
add_custom_command(OUTPUT ${GEN_SOURCES}
	COMMAND pywrap.exe
	ARGS ${EXPORT_SOURCES} -- -I${GEN_INCLUDE_DIRS} -xc++ -std=c++14 -DPYTHON_VERSION=${PYTHON_VERSION}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMENT "Generating Bindings and Extension"
)

set(SOURCES
	${GEN_SOURCES}
	source/Graphics.h
	source/core/ShaderProgram.cpp
	source/core/String.cpp
	source/system/Collision.cpp
	source/Cursor.h
	source/Cursor.cpp
	source/Texture.h
	source/Texture.cpp
	source/Material.h
	source/Material.cpp
	source/Mesh.h
	source/Mesh.cpp
	source/component/Camera.cpp
	source/entity/Script.cpp
	source/entity/Object.cpp
	source/entity/Entity.cpp
	source/repository/ModelRepository.h
	source/repository/ModelRepository.cpp
	source/repository/EntityRepository.h
	source/repository/EntityRepository.cpp
	source/view/GltfCamera.h
	source/view/GltfCamera.cpp
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
		source/core/SunSpot.cpp
		source/core/Renderer.cpp
		${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
		source/core/android/Graphics.cpp
		source/core/android/App.cpp
		source/core/android/SunspotNativeActivity.cpp)
else()
	set(GLFW_SOURCES
		source/Window.h
		source/Window.cpp
		source/GlfwWindow.h
		source/GlfwWindow.cpp
	)
	set(EXE_SOURCES
		${GLFW_SOURCES}
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

	# Export ANativeActivity_onCreate(),
	# Refer to: https://github.com/android-ndk/ndk/issues/381.
	set_target_properties(${SST_NAME} PROPERTIES LINK_FLAGS "-u ANativeActivity_onCreate")

	add_custom_command(
		TARGET PRE_BUILD
			${SST_NAME}
		COMMAND
			${CMAKE_COMMAND} -E copy
			${CMAKE_CURRENT_SOURCE_DIR}/pyspot/lib/${CMAKE_ANDROID_ARCH_ABI}/libpython2.7.so
			${CMAKE_BINARY_DIR}/libpython2.7.so)
else()
	add_executable(${SST_NAME} ${EXE_SOURCES} ${SHADERS})
endif()

target_link_libraries(${SST_NAME} ${SST_NAME}-lib)


file(COPY ${PROJECT_SOURCE_DIR}/shader DESTINATION ${PROJECT_BINARY_DIR})

# Test
set(SST_TEST_DIR ${PROJECT_SOURCE_DIR}/test)
include(${SST_TEST_DIR}/Tests.cmake)
