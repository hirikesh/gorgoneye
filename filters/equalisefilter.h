#ifndef EQUALISEFILTER_H
#define EQUALISEFILTER_H

#include <cv.h>
#include <string>
#include "filters/basefilter.h"

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

    void _filter(const cv::Mat& src);
    void _store(cv::Mat &dstImg, cv::Mat &dstMsk);
    void _visualise();



    cv::Mat equalisedImg;
};

#endif // EQUALISEFILTER_H
