#ifndef NEWCAMSHIFTDETECTOR_H
#define NEWCAMSHIFTDETECTOR_H

#include <cv.h>
#include "basedetector.h"

class MeanShiftDetector : public BaseDetector
{
public:
    MeanShiftDetector(Store *st);

    bool locate(const cv::Mat& srcImg, const cv::Mat& srcMsk, cv::Rect& srcRoi);

private:
    void calcWeightedHist(const cv::Mat&, const cv::Mat&, cv::MatND&, const int);
    void calcRatioHist(const cv::Mat&, const cv::Mat&, const cv::Rect&, cv::MatND&, const int);
    void calcWRHist(const cv::Mat&, const cv::Rect&, const cv::Mat&, cv::MatND&);
    bool enBackProjImg;
    bool histCalibrate;

    cv::Rect searchRoi;
    cv::MatND hist;
    cv::Mat backProjImg;
    cv::Mat backProjVisImg;
};

#endif // NEWCAMSHIFTDETECTOR_H
