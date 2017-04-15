/*
 * Source for "visiontracker/calibration.h"
 *
 * Copyright (c) 2017, EchoVision
 *
 * Contributors:
 *   - Taranvir Waraich
 */


#include "visiontracker/calibration.h"

/* Settings */

void Settings::write(cv::FileStorage& fs) const {
  fs << "{" << "boardSize" << boardSize
     << "squareSize" << squareSize << "}";
}

void Settings::read(const cv::FileNode& node) {
  node["boardSize"] >> boardSize;
  node["squareSize"] >> squareSize;
}

void Settings::setDefault() {
  boardSize = cv::Size(9,6);
  squareSize = SQUAREDIMENSION;
}

cv::Size Settings::getBoardSize() {
  return boardSize;
}
