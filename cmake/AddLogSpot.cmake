# Add LogSpot
set(LOGSPOT logspot)
set(LOGSPOT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/${LOGSPOT}/include)
set(LOGSPOT_LIBRARY     ${LOGSPOT})
if(ANDROID)
	set(LOGSPOT_LIBRARY ${LOGSPOT_LIBRARY} log)
endif()

set(LOGSPOT_INCLUDE_DIRS ${LOGSPOT_INCLUDE_DIR})
set(LOGSPOT_LIBRARIES    ${LOGSPOT_LIBRARY})

# Check LogSpot
if(NOT TARGET ${LOGSPOT})
	add_subdirectory(${CMAKE_SOURCE_DIR}/${LOGSPOT})
endif()
