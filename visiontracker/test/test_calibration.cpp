
#include "visiontracker/calibration.h"
#include <opencv2/opencv.hpp>
#include <gtest/gtest.h>

TEST(CalibrationTestCase, Namespace) {
  EXPECT_EQ(vt::Point2fVec(0), vt::Point2fVec(0));
}

TEST(CalibrationTestCase, Types) {
  EXPECT_EQ(std::vector<cv::Point2f>(0), vt::Point2fVec(0));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
