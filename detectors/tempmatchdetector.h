#ifndef TEMPMATCHDETECTOR_H
#define TEMPMATCHDETECTOR_H

#include <cv.h>
#include "basedetector.h"

class TempMatchDetector : public BaseDetector
{
public:
    TempMatchDetector(Store* st, int md = 0);

    bool locate(const cv::Mat& srcImg, const cv::Mat& srcTmp, cv::Rect& srcRoi);

private:
    int method;
};

#endif // TEMPMATCHDETECTOR_H
