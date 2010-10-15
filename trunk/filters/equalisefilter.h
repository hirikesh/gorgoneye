#ifndef EQUALISEFILTER_H
#define EQUALISEFILTER_H

#include "basefilter.h"

class Store;

class EqualiseFilter : public BaseFilter
{
public:
    EqualiseFilter(Store* st);
    void filter(const cv::Mat& srcImg, cv::Mat& dstImg, cv::Mat& dstMsk);

private:
    bool useHSV;

    cv::Mat hsvImg;
    cv::Mat grayImg;
    cv::Mat equalisedImg;

    void _filter(const cv::Mat& src);
    void _store(cv::Mat &dstImg, cv::Mat &dstMsk);
    void _visualise();
};

#endif // EQUALISEFILTER_H
