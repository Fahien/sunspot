cmake_minimum_required(VERSION 3.6)
project(SunSpot LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

# Modules and libs
set(MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/module)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

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
		${LIB_DIR})

	find_package(OpenGL REQUIRED)
	include(AddGlew)
	include(AddGlfw)
	set(DEPENDENCY_LIBRARIES
		${OPENGL_LIBRARIES}
		${GLEW_LIBRARIES}
		${GLFW_LIBRARIES})
	list(APPEND DEPENDENCY_INCLUDE_DIRS
		${OPENGL_INCLUDE_DIR}
		${GLEW_INCLUDE_DIR}
		${GLFW_INCLUDE_DIR})
endif()


# ImGui
set(IMGUI imgui)
set(IMGUI_DIR ${MODULE_DIR}/${IMGUI})
set(IMGUI_INCLUDE_DIR ${MODULE_DIR})
set(IMGUI_SOURCES
	${IMGUI_DIR}/imgui.cpp
	${IMGUI_DIR}/imgui_draw.cpp
	${IMGUI_DIR}/imgui_widgets.cpp
	${IMGUI_DIR}/imgui_demo.cpp
	${IMGUI_DIR}/examples/imgui_impl_opengl3.cpp
	${IMGUI_DIR}/examples/imgui_impl_glfw.cpp)
add_library(${IMGUI} ${IMGUI_SOURCES})
target_link_libraries(${IMGUI}
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
	${GLFW_LIBRARIES})
target_include_directories(${IMGUI}
	PUBLIC
		${IMGUI_INCLUDE_DIR}
		${OPENGL_INCLUDE_DIR}
		${GLEW_INCLUDE_DIR}
		${GLFW_INCLUDE_DIR}
	PRIVATE ${IMGUI_DIR})
target_compile_definitions(${IMGUI} PUBLIC "-DIMGUI_IMPL_OPENGL_LOADER_GLEW" "-DGLEW_STATIC")


set(SHADERS
	shader/base.vert
	shader/base.frag
	shader/depth.frag
	shader/quad.vert
	shader/quad.frag)
source_group("shader" FILES ${SHADERS})


# Pyspot extension files
set(GEN_SOURCES
	${CMAKE_CURRENT_BINARY_DIR}/src/pyspot/Bindings.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/pyspot/Extension.cpp)

# Join function
function(join VALUES GLUE OUTPUT)
	string(REGEX REPLACE "([^\\]|^);" "\\1${GLUE}" _TMP_STR "${VALUES}")
	string(REGEX REPLACE "[\\](.)" "\\1" _TMP_STR "${_TMP_STR}") #fixes escaping
	set(${OUTPUT} "${_TMP_STR}" PARENT_SCOPE)
endfunction()

# Sources to export
set(EXPORT_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/include/sunspot/util/Export.hpp
)

# Includes for export
set(EXPORT_INCLUDES
	${DEPENDENCY_INCLUDE_DIRS} ${MATHSPOT_INCLUDE_DIRS} ${GLTFSPOT_INCLUDE_DIRS} ${IMGUI_INCLUDE_DIR} ${PYSPOT_INCLUDE_DIRS}
)

message(STATUS "Gltf include dirs ${GLTFSPOT_INCLUDE_DIRS}")
# Generation command
set( COMMAND_ARGS
	${EXPORT_HEADERS} -o sunspot
	-- -xc++ -std=c++17 -DPY_MAJOR_VERSION=3
	-I${DEPENDENCY_INCLUDE_DIRS}
	-I${MATHSPOT_INCLUDE_DIRS}
	-I${GLTFSPOT_INCLUDE_DIRS}
	-I${IMGUI_INCLUDE_DIR}
)

add_custom_command(OUTPUT ${GEN_SOURCES}
	COMMAND pywrap
	ARGS ${COMMAND_ARGS}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMENT "Generating Bindings and Extension"
	COMMAND_EXPAND_LISTS
	VERBATIM
)

add_library( bindings ${GEN_SOURCES} )
target_link_libraries( bindings ${PYSPOT_LIBRARIES})
target_include_directories( bindings PUBLIC ${EXPORT_INCLUDES})

set(EXPORT_LIBRARIES
	$<TARGET_FILE:pyspot>
	$<TARGET_FILE:mathspot>
	$<TARGET_FILE:gltfspot>
	$<TARGET_FILE:sunspot-lib>
)

# Build extension module with distutils
find_package(Python3 COMPONENTS Interpreter)
add_custom_command( OUTPUT sunspot.so
	COMMAND ${Python3_EXECUTABLE}
	ARGS ${CMAKE_CURRENT_SOURCE_DIR}/script/build-module.py sunspot ${GEN_SOURCES} --includes ${EXPORT_INCLUDES} --libraries ${EXPORT_LIBRARIES}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/script
	COMMENT "Building python module"
	COMMAND_EXPAND_LISTS
)
add_custom_target(gen_bindings SOURCES sunspot.so)

# Sources
set(SOURCES
	source/util/Util.cpp
	source/util/Cube.cpp
	source/util/Config.cpp
	source/core/Config.cpp
	source/graphics/Graphics.cpp
	source/graphics/Shader.cpp
	source/core/String.cpp
	source/core/Collisions.cpp
	source/core/Animations.cpp
	source/core/Scripts.cpp
	source/Cursor.h
	source/Cursor.cpp
	source/graphics/Texture.cpp
	source/graphics/Material.cpp
	source/Mesh.h
	source/Mesh.cpp
	source/core/Gui.cpp
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
	source/editor/Editor.cpp
	source/Color.h
	source/Color.cpp
	source/graphics/Light.cpp
	source/Quad.h
	source/Quad.cpp
	source/Camera.h
	source/Camera.cpp)

if(ANDROID)
	set(ANDROID_SOURCES
		source/core/SunSpot.cpp
		${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
		source/core/android/Graphics.cpp
		source/core/android/App.cpp
		source/core/android/SunspotNativeActivity.cpp)
else()
	set(GLFW_SOURCES
		source/core/Game.cpp
		source/Window.h
		source/Window.cpp
		source/core/GlfwWindow.cpp)
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
	${HITSPOT_LIBRARIES}
	${IMGUI}
	bindings)

# SunSpot library
add_library(${SST_NAME}-lib ${SOURCES} ${SHADERS})
target_compile_features( ${SST_NAME}-lib PRIVATE cxx_std_17 )
target_include_directories(${SST_NAME}-lib PUBLIC ${DEPENDENCY_INCLUDE_DIRS})
target_link_libraries(${SST_NAME}-lib ${DEPENDENCY_LIBRARIES})
add_dependencies(${SST_NAME}-lib bindings)

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
