include_directories(../include)


# Add tests here:

# Calibration
add_executable(TEST_CALIBRATION test_calibration.cpp)
target_link_libraries(TEST_CALIBRATION visiontracker gtest ${OpenCV_LIBS})
add_test( NAME Calibration COMMAND TEST_CALIBRATION)

# Tracker
add_executable(TEST_TRACKER test_tracker.cpp)
target_link_libraries(TEST_TRACKER visiontracker gtest ${OpenCV_LIBS})
add_test( NAME Tracker COMMAND TEST_TRACKER)

# Make sure to add them here as well

set(TEST_EXECUTABLES TEST_TRACKER TEST_CALIBRATION)
