# Find Python Libraries
if(${CMAKE_SYSTEM_NAME} MATCHES "Android")
	set(PYTHON_NAME python3)
	if(NOT TARGET ${PYTHON_NAME})
		add_library(${PYTHON_NAME} SHARED IMPORTED)
		set_target_properties(${PYTHON_NAME} PROPERTIES
			IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/android/${CMAKE_ANDROID_ARCH_ABI}/libpython3.5m.so)
	endif()

	set(PYTHON_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/android/python)
	set(PYTHON_LIBRARIES ${PYTHON_NAME})
else()
	find_package(PythonLibs 3)
endif()
