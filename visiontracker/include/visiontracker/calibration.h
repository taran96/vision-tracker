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
#include <math.h>

#include <opencv2/core.hpp>


// Defining real dimensions in meters
#define SQUAREDIMENSION 0.025f

class Serializable {
public:
    virtual void write(cv::FileStorage& fs) const = 0;
    virtual void read(const cv::FileNode& fs) = 0;
    virtual void setDefault() = 0;
};

class Settings : public Serializable {
protected:
  cv::Size boardSize;
  float squareSize;

public:
  void write(cv::FileStorage& fs) const;
  void read(const cv::FileNode& node);
  void setDefault();

  cv::Size getBoardSize();
};

#endif
