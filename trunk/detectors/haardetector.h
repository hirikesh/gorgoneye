/* HaarDetector:
   Concrete BaseDetector class using
   HAAR classification. */

#ifndef HAAR_H
#define HAAR_H

#include <cv.h>
#include "basedetector.h"

class HaarDetector : public BaseDetector
{
public:
    HaarDetector(Store* st, std::string td, double sf = 1.2, int mn = 3, bool fg = NULL, cv::Size ms = cv::Size(24,24));

    bool locate(const cv::Mat& srcImg, const cv::Mat& srcMsk, cv::Rect& srcRoi);

private:
    double scaleFactor;
    int minNeighbours;
    int flags;
    cv::Size minSize;

    cv::CascadeClassifier cClassifier;
};

#endif // HAAR_H
