#ifndef CANNYEDGEFILTER_H
#define CANNYEDGEFILTER_H

#include "basefilter.h"

class CannyEdgeFilter : public BaseFilter
{
public:
    CannyEdgeFilter(Store* st,
                   double tl = 160, double th = 320);

    void setParams(double, double);
    void filter(const cv::Mat &srcImg, cv::Mat &dstImg, cv::Mat &dstMsk);

private:
    bool visCanny;
    double threshLow;
    double threshHigh;

    cv::Mat cannyImg;
    cv::Mat visCannyImg;

    void _filter(const cv::Mat &src);
    void _store(cv::Mat &dstImg, cv::Mat &dstMsk);
    void _visualise();
};

#endif // CANNYEDGEFILTER_H
