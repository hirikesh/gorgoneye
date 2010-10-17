#ifndef HARRISCORNERFILTER_H
#define HARRISCORNERFILTER_H

#include "basefilter.h"

class HarrisCornerFilter : public BaseFilter
{
public:
    HarrisCornerFilter(Store* st,
                       int ns = 7,
                       int ks = 5,
                       double k = 0.04);

    void setParams(int, int, double);
    void filter(const cv::Mat& srcImg, cv::Mat& dstImg, cv::Mat& dstMsk);

private:
    bool visHarris;

    int neighbourhoodSize;
    int sobelKernelSize;
    double freeParamK;

    cv::Mat harrisImg;
    cv::Mat visHarrisImg;

    void _filter(const cv::Mat& src);
    void _store(cv::Mat& dstImg, cv::Mat& dstMsk);
    void _visualise();
};

#endif // HARRISCORNERFILTER_H
