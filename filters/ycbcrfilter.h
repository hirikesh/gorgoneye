#ifndef YCBCRFILTER_H
#define YCBCRFILTER_H

#include "basefilter.h"

namespace cv
{
    template <class T>
    class Size_;
    typedef Size_<int> Size;
}

class YCbCrFilter : public BaseFilter
{
public:
    YCbCrFilter(const std::string& nm, Store* st);

    bool hasParams() const;
    void setParams(int, int, int, int, int, int);

    void filter(const cv::Mat& srcImg, cv::Mat& dstMsk);
    void filter(const cv::Mat& srcImg, cv::Mat& dstImg, cv::Mat& dstMsk);
//    void filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Mat& srcMsk, cv::Mat& dstMsk);
//    void filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Rect& srcRoi, cv::Rect& dstRoi);

private:
    bool dstCr;
    bool visCr, visCb;

    int minLuma, maxLuma;
    int minCr, maxCr;
    int minCb, maxCb;

    cv::Mat yccImg;
    cv::Mat visCrImg;
    cv::Mat visCbImg;

    void _filter(const cv::Mat &srcImg, cv::Mat &dstMsk);
    void _visualise();
};

#endif // YCBCRFILTER_H
