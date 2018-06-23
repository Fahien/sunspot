# Add PySpot
set(PYSPOT pyspot)
set(PYSPOT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/${PYSPOT}/include)
set(PYSPOT_LIBRARY ${PYSPOT})

set(PYSPOT_INCLUDE_DIRS ${PYSPOT_INCLUDE_DIR})
find_package(PythonLibraries)
set(PYSPOT_LIBRARIES ${PYSPOT_LIBRARY} ${PYTHON_LIBRARIES})

# Check PySpot
if(NOT TARGET ${PYSPOT})
	add_subdirectory(${CMAKE_SOURCE_DIR}/${PYSPOT})
endif()
