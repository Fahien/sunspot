# Add Graphics Library Framework
find_package(GLFW)

if(NOT ${GLFW_FOUND})
	set( GLFW_DIR ${MODULE_DIR}/glfw )
	add_subdirectory( ${GLFW_DIR} )
	set( GLFW_INCLUDE_DIRS ${GLFW_DIR}/include )
	set( GLFW_LIBRARIES glfw )
endif()
