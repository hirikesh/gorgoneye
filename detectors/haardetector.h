/* HaarDetector:
   Concrete BaseDetector class using
   HAAR classification. */

#ifndef HAAR_H
#define HAAR_H

#include <cv.h>
#include "basedetector.h"

#define HAAR_CC_DIR "c:\\opencv2.1\\data\\haarcascades"
#define HAAR_CC_FACE HAAR_CC_DIR"\\haarcascade_frontalface_default.xml"
#define HAAR_CC_EYES HAAR_CC_DIR"\\haarcascade_mcs_lefteye.xml"


class HaarDetector : public BaseDetector
{
public:
    HaarDetector(const int type, std::string td, double sf, int mn, bool fg, cv::Size ms);

    bool locate(const cv::Mat& srcImg, cv::Rect& srcRoi);

private:
    std::string trainingData;
    double scaleFactor;
    int minNeighbours;
    int flags;

    cv::Size minSize;
};

#endif // HAAR_H