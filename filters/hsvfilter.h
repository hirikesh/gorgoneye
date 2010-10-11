#ifndef HSVFILTER_H
#define HSVFILTER_H

#include "basefilter.h"

namespace cv
{
    template <class T>
    class Size_;
    typedef Size_<int> Size;
}

class HSVFilter : public BaseFilter
{
public:
    HSVFilter(Store* st,
              int mnh = 0, int mxh = 181,
              int mns = 0, int mxs = 256,
              int mnv = 0, int mxv = 256);

    void setParams(int, int, int, int, int, int);
    void filter(const cv::Mat& srcImg, cv::Mat& dstImg, cv::Mat& dstMsk);
//    void filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Rect& srcRoi, cv::Rect& dstRoi);

private:
    bool visHue;
    bool visMask;

    int minHue;
    int maxHue;
    int minSat;
    int maxSat;
    int minVal;
    int maxVal;

    cv::Mat hsvImg;
    cv::Mat hueChannel;
    cv::Mat satChannel;
    cv::Mat valChannel;
    cv::Mat visHueImg;

    cv::Mat maskImg;
    cv::Mat visMaskImg;

    void _filter(const cv::Mat &src);
    void _store(cv::Mat &dstImg, cv::Mat &dstMsk);
    void _visualise();
    void _visualise3ch();
};

#endif // HSVFILTER_H
