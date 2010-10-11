#ifndef EQUALISEFILTER_H
#define EQUALISEFILTER_H

#include <cv.h>
#include <string>
#include "filters/basefilter.h"

class Store;

class EqualiseFilter : public BaseFilter
{
public:
    EqualiseFilter(const std::string& nm, Store* st);
    void filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Mat& srcMsk, cv::Mat& dstMsk);

private:
    void _filter(const cv::Mat& src, cv::Mat& dst);
    void _visualise();

    bool useHSV;

    cv::Mat equalisedImg;
};

#endif // EQUALISEFILTER_H
