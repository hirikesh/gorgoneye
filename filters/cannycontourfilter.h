#ifndef CANNYCONTOURFILTER_H
#define CANNYCONTOURFILTER_H

#include "basefilter.h"

class CannyContourFilter : public BaseFilter
{
public:
    CannyContourFilter(Store* st,
                       double tl = 160, double th = 320,
                       int dt = 164);

    void setParams();
    void filter(const cv::Mat &srcImg, cv::Mat &dstImg, cv::Mat &dstMsk);

private:
    double threshLow;
    double threshHigh;
    int distThresh;

    cv::Mat testImg;
    cv::Mat testImg2;
    cv::Mat testImg3;
    cv::Mat testImg4;

    void _filter(const cv::Mat &src);
    void _store(cv::Mat &dstImg, cv::Mat &dstMsk);
    void _visualise();
};

#endif // CANNYCONTOURFILTER_H
