#ifndef GRAYSCALEFILTER_H
#define GRAYSCALEFILTER_H

#include "basefilter.h"

class GrayscaleFilter : public BaseFilter
{
public:
    GrayscaleFilter(Store* st, int mng = 0, int mxg = 256);

    void setParams(int, int);
    void filter(const cv::Mat& srcImg, cv::Mat& dstImg, cv::Mat& dstMsk);
//    void filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Rect& srcRoi, cv::Rect& dstRoi);

private:
    bool visGray;
    bool visMask;

    int minGray;
    int maxGray;

    cv::Mat grayChannel;
    cv::Mat visGrayImg;

    cv::Mat maskImg;
    cv::Mat visMaskImg;

    void _filter(const cv::Mat& src);
    void _store(cv::Mat& dstImg, cv::Mat& dstMsk);
    void _visualise();
};

#endif // GRAYSCALEFILTER_H
