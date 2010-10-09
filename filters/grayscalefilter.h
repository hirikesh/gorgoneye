#ifndef GRAYSCALEFILTER_H
#define GRAYSCALEFILTER_H

#include "basefilter.h"

class GrayscaleFilter : public BaseFilter
{
public:
    GrayscaleFilter(const std::string& nm, Store* st);

    bool hasParams() const;
    void setParams(int, int);

    void filter(const cv::Mat& srcImg, cv::Mat& dstMsk);
    void filter(const cv::Mat& srcImg, cv::Mat& dstImg, cv::Mat& dstMsk);
//    void filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Mat& srcMsk, cv::Mat& dstMsk);
//    void filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Rect& srcRoi, cv::Rect& dstRoi);

private:
    bool visGray;
    int minGray, maxGray;

    cv::Mat grayImg;
    cv::Mat visGrayImg;

    void _filter(const cv::Mat &srcImg, cv::Mat &dstMsk);
    void _visualise();
};

#endif // GRAYSCALEFILTER_H
