/* HaarDetector:
   Concrete BaseDetector class using
   HAAR classification. */

#ifndef HAAR_H
#define HAAR_H

#include <cv.h>
#include "basedetector.h"

#ifdef _WIN32
#define HAAR_CC_DIR "c:\\opencv2.1\\data\\haarcascades\\"
#else
#define HAAR_CC_DIR "/usr/local/share/opencv/haarcascades/"
#endif

#define HAAR_CC_FACE HAAR_CC_DIR"haarcascade_frontalface_default.xml"
#define HAAR_CC_EYES HAAR_CC_DIR"haarcascade_msc_lefteye.xml"


class HaarDetector : public BaseDetector
{
public:
    HaarDetector(const int type, std::string td, double sf, int mn, bool fg, cv::Size ms);

    bool locate(const cv::Mat& srcImg, cv::Rect& srcRoi);

private:
    cv::CascadeClassifier cClassifier;
//    std::string trainingData;
    double scaleFactor;
    int minNeighbours;
    int flags;
    cv::Size minSize;

    // Temporary vars
    std::vector<cv::Rect> rois;
};

#endif // HAAR_H
