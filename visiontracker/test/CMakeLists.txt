include_directories(../include)

add_executable(Test test_calibration.cpp)
target_link_libraries(Test visiontracker gtest ${OpenCV_LIBS})
