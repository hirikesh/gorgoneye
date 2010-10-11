#ifndef GRAYSCALEFILTER_H
#define GRAYSCALEFILTER_H

#include "basefilter.h"

class GrayscaleFilter : public BaseFilter
{
public:
    GrayscaleFilter(const std::string& nm, Store* st, int mng = 0, int mxg = 256);

    void setParams(int, int);
    void filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Mat& srcMsk, cv::Mat& dstMsk);
//    void filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Rect& srcRoi, cv::Rect& dstRoi);

    cv::Mat grayChannel;

private:
    bool visGray;
    bool visMask;

    int minGray;
    int maxGray;

    cv::Mat visGrayImg;
    cv::Mat visMaskImg;

    void _filter(const cv::Mat &src, cv::Mat &dst);
    void _visualise();
};

#endif // GRAYSCALEFILTER_H
