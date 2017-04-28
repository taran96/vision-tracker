#include "visiontracker/tracker.h"

using namespace vt;

ArucoDetector::ArucoDetector() {
    markerDictionary = cv::aruco::getPredefinedDictionary(cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);
    squareDimension = ARUCODIMENSION;
}


ArucoDetector::ArucoDetector(vt::CalibrationData &calibData) {
    squareDimension = ARUCODIMENSION;
    cameraMatrix = calibData.getCameraMatrix();
    distCoeffs = calibData.getDistCoeffs(); 
    markerDictionary = cv::aruco::getPredefinedDictionary(cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);
    squareDimension = ARUCODIMENSION; 
}

void ArucoDetector::createArucoMarkers() {
    cv::Mat outputMarker;
    for (int i = 0; i < 50; i++) {  // hard coded needs to be replaced
        cv::aruco::drawMarker(markerDictionary, i, ARUCOPIXELS, outputMarker, 1);
        std::ostringstream convert;
        std::string imgName = "./data/aruco/4X4Marker_";
       convert << imgName << i << ".jpg";
       cv::imwrite(convert.str(), outputMarker);
    }
}

int ArucoDetector::startMonitoring(cv::VideoCapture vid) {
    if (!vid.isOpened())
        return 0;
    cv::Mat frame;
    std::vector<int> markerIds;
    Point2fMat markerCorners, rejectedCandidates;
    cv::aruco::DetectorParameters parameters;
    std::vector<cv::Vec3d> rvecs, tvecs;

    cv::namedWindow("WebCam", CV_WINDOW_AUTOSIZE);

    while (vid.read(frame)) {
        cv::aruco::detectMarkers(frame, markerDictionary, markerCorners, markerIds);
        cv::aruco::estimatePoseSingleMarkers(markerCorners, squareDimension, cameraMatrix, distCoeffs, rvecs, tvecs);

        for (int i = 0; i < markerIds.size(); i++) {
            cv::aruco::drawAxis(frame, cameraMatrix, distCoeffs, rvecs, tvecs, 0.1f);
            cv::imshow("WebCam", frame);
            if (cv::waitKey(30) >= 0)
                break;
        }
    }

    return 1;
}
