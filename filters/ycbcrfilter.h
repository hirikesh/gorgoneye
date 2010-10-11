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
    YCbCrFilter(const std::string& nm, Store* st, int mny = 0, int mxy = 256, int mnr = 0, int mxr = 256, int mnb = 0, int mxb = 256);

    void setParams(int, int, int, int, int, int);
    void filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Mat& srcMsk, cv::Mat& dstMsk);
//    void filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Rect& srcRoi, cv::Rect& dstRoi);

    cv::Mat lumaChannel;
    cv::Mat crChannel;
    cv::Mat cbChannel;

private:
    bool dstCr;
    bool visCr;
    bool visCb;
    bool visMask;

    int minLuma;
    int maxLuma;
    int minCr;
    int maxCr;
    int minCb;
    int maxCb;

    cv::Mat yccImg;
    cv::Mat visCrImg;
    cv::Mat visCbImg;
    cv::Mat visMaskImg;

    void _filter(const cv::Mat &src, cv::Mat &dst);
    void _visualise();
};

#endif // YCBCRFILTER_H
