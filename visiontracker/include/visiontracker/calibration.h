/*
 * Calibration Header
 * This submodule will ease camera calibration
 *
 * Copyright (c) 2017, EchoVision
 *
 * Contributors:
 *   - Taranvir Waraich
 */

#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

// Defining real dimensions in meters
#define SQUAREDIMENSION 0.025f

namespace vt {

typedef std::vector<cv::Point2f> Point2fVec;
typedef std::vector<cv::Point3f> Point3fVec;
typedef std::vector<Point2fVec> Point2fMat;
typedef std::vector<Point3fVec> Point3fMat;

class Serializable {
public:
  virtual void write(cv::FileStorage &fs) const = 0;
  virtual void read(const cv::FileNode &fs) = 0;
  virtual void setDefault() = 0;
};

class Settings : public Serializable {
protected:
  cv::Size boardSize;
  float squareSize;

public:
  void write(cv::FileStorage &fs) const;
  void read(const cv::FileNode &node);
  void setDefault();

  cv::Size getBoardSize();
}; // Settings

class CalibrationData : public Serializable {
public:
  cv::Mat distCoeffs, cameraMatrix;
  std::vector<cv::Mat> calibrationImages, rvecs, tvecs;
  std::string filename;
  Point2fMat imagePoints;
  // std::vector<std::vector<cv::Point3f> > worldPoints;
  cv::Size boardSize;

public:
  CalibrationData(Settings &s);
  CalibrationData();
  ~CalibrationData();
  void write(cv::FileStorage &fs) const;
  void read(const cv::FileNode &node);
  void setDefault();
  void createKnownBoardPositions(cv::Size boardSize, float squareEdgeLength,
                                 Point3fVec &corners);
  void getChessboardCorners(std::vector<cv::Mat> images,
                            Point2fMat &allFoundCorners,
                            bool showResults = false);
  void calibrate(cv::VideoCapture &vid);
  void calibrate(const std::vector<cv::Mat> &matv);
  cv::Mat getCameraMatrix();
  cv::Mat getDistCoeffs();
}; // CalibrationData
}; // namespace vt
#endif
