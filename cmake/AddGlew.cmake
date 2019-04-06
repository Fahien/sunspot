# Add OpenGL Extension Wrangler Library
find_package( GLEW )

if ( NOT ${GLEW_FOUND} )
	# Download latest
	set( GLEW_VERSION glew-2.1.0 )
	set( GLEW_FILE ${GLEW_VERSION}.tgz )
	set( OUT ${LIB_DIR}/${GLEW_FILE} )
	set( URL "https://github.com/nigels-com/glew/releases/download/${GLEW_VERSION}/${GLEW_FILE}" )
	message( STATUS "Downloading ${URL}" )
	file( DOWNLOAD ${URL} ${OUT} )
	message( STATUS "Extracting ${OUT}" )
	execute_process(
		COMMAND ${CMAKE_COMMAND} -E tar xzf ${OUT}
		WORKING_DIRECTORY ${LIB_DIR}
	)
	message( STATUS "Compiling ${GLEW_VERSION}" )
	execute_process(
		COMMAND make
		WORKING_DIRECTORY ${LIB_DIR}/${GLEW_VERSION}
	)
	message( STATUS "Installing ${GLEW_VERSION}" )
	execute_process(
		COMMAND sudo make install
		WORKING_DIRECTORY ${LIB_DIR}/${GLEW_VERSION}
	)
	message( STATUS "Cleaning ${GLEW_VERSION}" )
	execute_process(
		COMMAND make clean
		WORKING_DIRECTORY ${LIB_DIR}/${GLEW_VERSION}
	)
	file( REMOVE ${OUT} )
	file( REMOVE ${LIB_DIR}/${GLFW_VERSION} )
	find_package( GLEW REQUIRED )
endif()