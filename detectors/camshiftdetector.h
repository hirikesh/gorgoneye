#ifndef CAMSHIFTDETECTOR_H
#define CAMSHIFTDETECTOR_H

#include <cv.h>
#include "basedetector.h"

class CAMShiftDetector : public BaseDetector
{
public:
    CAMShiftDetector(Store *st);

    bool locate(const cv::Mat& srcImg, const cv::Mat& srcMsk, cv::Rect& srcRoi);

private:
    bool enBackProjImg;
    bool histCalibrate;

    cv::MatND hist;
    cv::Mat backProjImg;
    cv::Mat backProjVisImg;
};

#endif // CAMSHIFTDETECTOR_H
