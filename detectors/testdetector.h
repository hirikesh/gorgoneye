#ifndef TEST_H
#define TEST_H

#include <cv.h>
#include "basedetector.h"

class TestDetector : public BaseDetector
{
public:
    TestDetector(Store *st, const int type);

    bool locate(const cv::Mat& srcImg, const cv::Mat& srcMsk, cv::Rect& srcRoi);

private:
    int minSobel;
    int maxSobel;

    cv::Mat testImg;
    cv::Mat sobelImg;
};

#endif // TEST_H
