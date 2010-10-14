#ifndef CANNYCONTOURFILTER_H
#define CANNYCONTOURFILTER_H

#include "basefilter.h"

class CannyContourFilter : public BaseFilter
{
public:
    CannyContourFilter(Store* st,
                       double tl = 80, double th = 160,
                       double mnd = 0.6, double mxd = 0.65);

    void setParams();
    void filter(const cv::Mat &srcImg, cv::Mat &dstImg, cv::Mat &dstMsk);

private:
    double minGrad;
    double maxGrad;
    double minDist;
    double maxDist;

    cv::Mat testImg;
    cv::Mat testImg2;
    cv::Mat testImg3;
    cv::Mat testImg4;

    void _filter(const cv::Mat &src);
    void _store(cv::Mat &dstImg, cv::Mat &dstMsk);
    void _visualise();


};


bool _predicate(const cv::Point&, const cv::Point&);

#endif // CANNYCONTOURFILTER_H
