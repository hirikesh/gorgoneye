#ifndef DOGNORMFILTER_H
#define DOGNORMFILTER_H

#include "basefilter.h"

class DoGNormFilter : public BaseFilter
{
public:
    DoGNormFilter(Store* st, int ks = 7, double ls = 1, double us = 2,
                  int sm = MEDIAN, double t = 10, double b = 3.0);

    void setParams();
    void filter(const cv::Mat& srcImg, cv::Mat& dstImg, cv::Mat& dstMsk);

private:
    bool visDoG;
    int kernelSize;
    double lowerSigma;
    double upperSigma;
    int scaleMethod;
    double tau;
    double beta;

    enum { MEDIAN = 1, MEAN = 2};

    cv::Mat doGKernel;
    cv::Mat doGImg;
    cv::Mat visDoGImg;

    void _filter(const cv::Mat& src);
    void _store(cv::Mat& dstImg, cv::Mat& dstMsk);
    void _visualise();
};

#endif // DOGNORMFILTER_H
