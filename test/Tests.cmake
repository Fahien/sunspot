include(CTest)

set(GLTF_OUT ${CMAKE_CURRENT_SOURCE_DIR}/${GLTFSPOT}/test/model)
include(${GLTFSPOT}/test/GltfFiles.cmake)

function(make_test TEST_NAME)
	# Sources
	set(TEST_SOURCES ${SST_TEST_DIR}/${TEST_NAME}.cpp)
	source_group(${TEST_NAME} FILES ${TEST_SOURCES})

	# Executable
	add_executable(${TEST_NAME} ${TEST_SOURCES})
	set_target_properties(${TEST_NAME} PROPERTIES LINKER_LANGUAGE CXX)
	target_link_libraries(${TEST_NAME} ${SST_NAME}-lib ${DEPENDENCY_LIBRARIES})

	# Tests
	foreach(GLTF ${GLTF_FILES})
		# Extract model name
		string(FIND ${GLTF} "/" LAST_SLASH REVERSE)
		string(FIND ${GLTF} "." LAST_DOT REVERSE)
		math(EXPR LAST_SLASH "${LAST_SLASH} + 1")
		math(EXPR SUB_LENGTH "${LAST_DOT} - ${LAST_SLASH}")
		string(SUBSTRING ${GLTF} ${LAST_SLASH} ${SUB_LENGTH} GLTF_NAME)
		message(STATUS ${GLTF_NAME})
		# Add test
		add_test(${TEST_NAME}-${GLTF_NAME} ${EXECUTABLE_OUTPUT_PATH}/${TEST_NAME} ${GLTF})
	endforeach()
endfunction(make_test)

if(NOT ANDROID)
make_test(gst-integration-test)
endif()