/* HaarDetector:
   Concreate BaseDetector class using
   HAAR classification. */

#ifndef HAAR_H
#define HAAR_H

#include <cv.h>
#include "basedetector.h"


class HaarDetector : public BaseDetector
{
public:
    HaarDetector(std::string td, double sf, int mn, bool fg, cv::Size ms);

    bool locate(const cv::Mat& srcImg, cv::Rect& srcRoi);

private:
    std::string trainingData;
    double scaleFactor;
    int minNeighbours;
    int flags;

    cv::Size minSize;
};

#endif // HAAR_H
