#include "visiontracker/calibration.h"
#include <gtest/gtest.h>
#include <iostream>
#include <opencv2/opencv.hpp>

TEST(SettingsTestCase, ReadFromFile) {
  vt::Settings s;
  const cv::String inputSettingsFile = "./data/calib/default.yml";
  cv::FileStorage settingsfs(inputSettingsFile, cv::FileStorage::READ);
  EXPECT_EQ(true, settingsfs.isOpened());

  s.read(settingsfs["settings"]);
  settingsfs.release();
}

TEST(SettingsTestCase, WriteToFile) {
  vt::Settings s;
  const cv::String outputSettingsFile = "./data/calib/default.yml";
  cv::FileStorage settingsfs(outputSettingsFile, cv::FileStorage::WRITE);
  EXPECT_EQ(true, settingsfs.isOpened());

  s.write(settingsfs);
  settingsfs.release();
}

TEST(SettingsTestCase, OpenMethod) {
  vt::Settings s, orig;
  orig.boardSize = cv::Size(2, 2);
  orig.squareSize = 0.0123;
  cv::FileStorage settingsfs("./data/calib/default.yml",
                             cv::FileStorage::WRITE);
  EXPECT_EQ(true, settingsfs.isOpened());

  orig.write(settingsfs);
  settingsfs.release();
  s.open("./data/calib/default.yml");

  EXPECT_EQ(s, orig);
}

TEST(SettingsTestCase, SaveMethod) {
  vt::Settings s, orig;
  s.boardSize = cv::Size(1, 1);
  s.squareSize = 0.234;
  s.save("./data/calib/default.yml");

  orig.open("./data/calib/default.yml");

  EXPECT_EQ(s, orig);
}

TEST(CalibrationTestCase, ReadFromFile) {
  vt::Settings s;
  const cv::String inputSettingsFile = "./data/calib/default.yml";
  cv::FileStorage settingsfs(inputSettingsFile, cv::FileStorage::READ);
  EXPECT_EQ(true, settingsfs.isOpened());

  s.read(settingsfs["settings"]);
  settingsfs.release();
  vt::CalibrationData cd(s);
  cv::FileStorage calibfs("./data/calib/calib.yml", cv::FileStorage::WRITE);
  EXPECT_EQ(true, calibfs.isOpened());

  cd.read(calibfs["calibration"]);
  calibfs.release();
}

TEST(CalibrationTestCase, WriteToFile) {
  vt::Settings s;
  const cv::String inputSettingsFile = "./data/calib/default.yml";
  cv::FileStorage settingsfs(inputSettingsFile, cv::FileStorage::READ);
  EXPECT_EQ(true, settingsfs.isOpened());

  s.read(settingsfs["settings"]);
  settingsfs.release();

  vt::CalibrationData cd(s);
  cv::FileStorage calibfs("./data/calib/calib.yml", cv::FileStorage::WRITE);
  EXPECT_EQ(true, calibfs.isOpened());

  cd.write(calibfs);
  calibfs.release();
}

TEST(CalibrationTestCase, OpenMethod) {
  vt::CalibrationData c, orig;
  cv::FileStorage calibfs("./data/calib/calib.yml", cv::FileStorage::WRITE);
  EXPECT_EQ(true, calibfs.isOpened());

  orig.write(calibfs);
  calibfs.release();
  c.open("./data/calib/calib.yml");

  EXPECT_EQ(c, orig);
}

TEST(CalibrationTestCase, SaveMethod) {
  vt::CalibrationData c, orig;
  c.boardSize = cv::Size(1, 1);
  c.cameraMatrix = cv::Mat::eye(1, 1, CV_64F);
  c.distCoeffs = cv::Mat::eye(1, 1, CV_64F);
  c.save("./data/calib/calib.yml");

  orig.open("./data/calib/calib.yml");

  EXPECT_EQ(c, orig);
}

int main(int argc, char **argv) {
  // Creating Test Files
  std::system("mkdir -p data/calib/");
  const cv::String inputSettingsFile = "./data/calib/default.yml";
  cv::FileStorage settingsfs(inputSettingsFile, cv::FileStorage::WRITE);
  settingsfs.release();

  const cv::String inputCalibFile = "./data/calib/calib.yml";
  cv::FileStorage calibfs(inputCalibFile, cv::FileStorage::WRITE);
  calibfs.release();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
