#ifndef TRACKER_H
#define TRACKER_H

#include "opencv2/opencv.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/aruco.hpp"
#include "calibration.h"

#define ARUCODIMENSION 0.044f
#define ARUCOPIXELS 500


namespace vt {
    class ArucoDetector {
        cv::Ptr<cv::aruco::Dictionary> markerDictionary;
        cv::Mat cameraMatrix, distCoeffs;
        float squareDimension;

        public:
        ArucoDetector();
        ArucoDetector(CalibrationData& calibData);
        int startMonitoring(cv::VideoCapture vid);
        void createArucoMarkers();;
    };
};

#endif
