#ifndef SOBELFILTER_H
#define SOBELFILTER_H

#include "basefilter.h"

class SobelFilter : public BaseFilter
{
public:
    SobelFilter(Store* st, int dx = 0, int dy = 1, int ks = 3);

    void setParams(int, int, int);
    void filter(const cv::Mat& srcImg, cv::Mat& dstImg, cv::Mat& dstMsk);

private:
    bool visSobel;

    int derivX;
    int derivY;
    int kernelSize;

    cv::Mat sobelImg;
    cv::Mat visSobelImg;

    void _filter(const cv::Mat& src);
    void _store(cv::Mat& dstImg, cv::Mat& dstMsk);
    void _visualise();
};

#endif // SOBELFILTER_H
