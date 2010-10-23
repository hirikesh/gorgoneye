#ifndef CORNERFILTER_H
#define CORNERFILTER_H

#include "basefilter.h"

class CornerFilter : public BaseFilter
{
public:
    CornerFilter(Store* st,
                 int mxc = 10, double ql = 0.01, double md = 8,
                 int bs = 3, bool uh = true, double fk = 0.04);

    void setParams();
    void filter(const cv::Mat& srcImg, cv::Mat& dstImg, cv::Mat& dstMsk);

private:
    bool visCorner;
    int maxCorners;
    double qualityLevel;
    double minDistance;
    int blockSize;
    bool useHarris;
    double freeK;

    cv::Mat cornerImg;
    cv::Mat visCornerImg;
    std::vector<cv::Point2f> corners;

    void _filter(const cv::Mat& src);
    void _store(cv::Mat& dstImg, cv::Mat& dstMsk);
    void _visualise();
};

#endif // CORNERFILTER_H
