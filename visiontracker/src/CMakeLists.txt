SET(INCLUDE_DIRS ../include)

include_directories(${INCLUDE_DIRS})

SET( HEADER_FILES
${INCLUDE_DIRS}/visiontracker/calibration.h
)

SET( SOURCES
calibration.cpp
${HEADER_FILES})

add_library( visiontracker ${SOURCES} )
target_link_libraries( visiontracker PUBLIC ${OpenCV_LIBS} )
