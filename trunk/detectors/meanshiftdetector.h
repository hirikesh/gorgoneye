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

    bool enBackProjImg;
    bool histCalibrate;

    cv::Rect searchRoi;
    cv::MatND hist;
    cv::MatND ratioHist;
    cv::Mat backProjImg;
    cv::Mat backProjVisImg;

    std::vector<double> _vec;
};

#endif // NEWCAMSHIFTDETECTOR_H
