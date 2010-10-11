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
    YCbCrFilter(Store* st,
                int mny = 0, int mxy = 256,
                int mnr = 0, int mxr = 256,
                int mnb = 0, int mxb = 256,
                int dcr = 1);

    void setParams(int, int, int, int, int, int, int);
    void filter(const cv::Mat& srcImg, cv::Mat& dstImg, cv::Mat& dstMsk);
//    void filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Rect& srcRoi, cv::Rect& dstRoi);

private:
    bool visCr;
    bool visCb;
    bool visMask;

    int minY;
    int maxY;
    int minCr;
    int maxCr;
    int minCb;
    int maxCb;
    int dstCr;

    cv::Mat yccImg;
    cv::Mat yChannel;
    cv::Mat crChannel;
    cv::Mat cbChannel;
    cv::Mat visCrImg;
    cv::Mat visCbImg;

    cv::Mat maskImg;
    cv::Mat visMaskImg;

    void _filter(const cv::Mat &src);
    void _store(cv::Mat &dstImg, cv::Mat &dstMsk);
    void _visualise();
    void _visualise3ch();
};

#endif // YCBCRFILTER_H
