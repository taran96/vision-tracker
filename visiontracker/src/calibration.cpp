/*
 * Source for "visiontracker/calibration.h"
 *
 * Copyright (c) 2017, EchoVision
 *
 * Contributors:
 *   - Taranvir Waraich
 */

#include "visiontracker/calibration.h"

using namespace vt;

/* Settings */

void Settings::write(cv::FileStorage &fs) const {
  fs << "settings"
     << "{"
     << "boardSize" << boardSize << "squareSize" << squareSize << "}";
}

void Settings::read(const cv::FileNode &node) {
  node["boardSize"] >> boardSize;
  node["squareSize"] >> squareSize;
}

void Settings::setDefault() {
  boardSize = cv::Size(9, 6);
  squareSize = SQUAREDIMENSION;
}

void Settings::open(const cv::String filename) {
  cv::FileStorage fs(filename, cv::FileStorage::READ);
  read(fs["settings"]);
  fs.release();
}

void Settings::save(const cv::String filename) {
  cv::FileStorage fs(filename, cv::FileStorage::WRITE);
  write(fs);
  fs.release();
}

bool Settings::operator==(const Settings &s) const {
  return boardSize == s.boardSize && squareSize == s.squareSize;
}
cv::Size Settings::getBoardSize() { return boardSize; }

/* CalibrationData */

CalibrationData::CalibrationData(Settings &s) { boardSize = s.getBoardSize(); }

CalibrationData::CalibrationData() {}
CalibrationData::~CalibrationData() {}

void CalibrationData::write(cv::FileStorage &fs) const {
  fs << "calibration"
     << "{"
     << "distanceCoeffs" << distCoeffs << "cameraMatrix" << cameraMatrix
     << "rvecs" << rvecs << "tvecs" << tvecs << "}";
}

void CalibrationData::read(const cv::FileNode &node) {
  node["cameraMatrix"] >> cameraMatrix;
  node["distCoeffs"] >> distCoeffs;
  node["rvecs"] >> rvecs;
  node["tvecs"] >> tvecs;
}

void CalibrationData::setDefault() {
  cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
  distCoeffs = cv::Mat::eye(3, 3, CV_64F);
  rvecs = std::vector<cv::Mat>(0);
  tvecs = std::vector<cv::Mat>(0);
}

void CalibrationData::createKnownBoardPositions(cv::Size boardSize,
                                                float squareEdgeLength,
                                                Point3fVec &corners) {

  for (int i = 0; i < boardSize.height; i++) {
    for (int j = 0; j < boardSize.width; j++) {
      corners.push_back(
          cv::Point3f(j * squareEdgeLength, i * squareEdgeLength, 0.0f));
    }
  }
}

void CalibrationData::getChessboardCorners(std::vector<cv::Mat> images,
                                           Point2fMat &allFoundCorners,
                                           bool showResults) {

  for (std::vector<cv::Mat>::iterator iter = images.begin();
       iter != images.end(); iter++) {
    std::vector<cv::Point2f> pointBuf;
    bool found = cv::findChessboardCorners(*iter, cv::Size(9, 6), pointBuf,
                                           CV_CALIB_CB_ADAPTIVE_THRESH |
                                               CV_CALIB_CB_NORMALIZE_IMAGE);
    if (found) {
      allFoundCorners.push_back(pointBuf);
    }
    if (showResults) {
      cv::drawChessboardCorners(*iter, cv::Size(9, 6), pointBuf, found);
      cv::imshow("Looking for corners", *iter);
      cv::waitKey(0);
    }
  }
}

void CalibrationData::calibrate(cv::VideoCapture &vid) {

  if (!vid.isOpened()) {
    std::cerr << "[ERROR] Camera not open" << std::endl;
    return;
  }

  std::vector<cv::Point2f> pointBuf;

  cv::Mat frame;
  cv::Mat drawToFrame;

  cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
  cameraMatrix.at<double>(0, 0) = 1.0;

  Point3fMat worldPoints(1);

  int fps = 20;

  cv::namedWindow("Webcam", CV_WINDOW_AUTOSIZE);

  while (vid.read(frame)) {

    std::vector<cv::Vec2f> foundPoints;
    bool found = false;
    found = cv::findChessboardCorners(frame, boardSize, foundPoints,
                                      CV_CALIB_CB_NORMALIZE_IMAGE |
                                          CV_CALIB_CB_ADAPTIVE_THRESH);

    frame.copyTo(drawToFrame);

    cv::drawChessboardCorners(drawToFrame, boardSize, foundPoints, found);

    if (found) {
      std::cout << "[INFO] Found corners" << std::endl;
      cv::imshow("Webcam", drawToFrame);
    } else {
      cv::imshow("Webcam", frame);
    }

    char chr = cv::waitKey(1000 / fps);

    switch (chr) {
    case ' ': // space
      if (found) {
        std::cout << "[INFO] Saving image" << std::endl;
        cv::Mat temp;
        frame.copyTo(temp);
        calibrationImages.push_back(temp);
      }
      break;
    case 13: // enter key
      // start calib
      if (calibrationImages.size() > 15) {
        getChessboardCorners(calibrationImages, imagePoints, false);
        createKnownBoardPositions(boardSize, SQUAREDIMENSION, worldPoints[0]);
        distCoeffs = cv::Mat::zeros(8, 1, CV_64F);
        worldPoints.resize(imagePoints.size(), worldPoints[0]);
        cv::calibrateCamera(worldPoints, imagePoints, boardSize, cameraMatrix,
                            distCoeffs, rvecs, tvecs);
      } else {
        std::cerr << "[ERROR] Not enough frames" << std::endl;
      }

      break;
    case 27: // esc key
      // exit
      return;
      break;
    default:
      break;
    }
  }
}

void CalibrationData::calibrate(const std::vector<cv::Mat> &matv) {
  cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
  cameraMatrix.at<double>(0, 0) = 1.0;
  Point3fVec worldPoints(1);
  getChessboardCorners(matv, imagePoints, false);
  createKnownBoardPositions(boardSize, SQUAREDIMENSION, worldPoints);
  distCoeffs = cv::Mat::zeros(8, 1, CV_64F);
  cv::calibrateCamera(worldPoints, imagePoints, boardSize, cameraMatrix,
                      distCoeffs, rvecs, tvecs);
}

void CalibrationData::open(const cv::String filename) {
  cv::FileStorage fs(filename, cv::FileStorage::READ);
  read(fs["calibration"]);
  fs.release();
}

void CalibrationData::save(const cv::String filename) {
  cv::FileStorage fs(filename, cv::FileStorage::WRITE);
  write(fs);
  fs.release();
}

cv::Mat CalibrationData::getDistCoeffs() { return distCoeffs; }

cv::Mat CalibrationData::getCameraMatrix() { return cameraMatrix; }

bool CalibrationData::operator==(const CalibrationData &a) const {
  return cv::countNonZero(a.cameraMatrix != cameraMatrix) &&
         (a.boardSize == boardSize) &&
         cv::countNonZero(a.distCoeffs != distCoeffs);
}
