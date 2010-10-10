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
    HSVFilter(const std::string& nm, Store* st);

    bool hasParams() const;
    void setParams(int, int, int, int, int, int);

    void filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Mat& srcMsk, cv::Mat& dstMsk);
//    void filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Rect& srcRoi, cv::Rect& dstRoi);

private:
    bool visHue;

    int minHue, maxHue;
    int minSat, maxSat;
    int minVal, maxVal;

    cv::Mat hsvImg;
    cv::Mat visHueImg;

    void _filter(const cv::Mat &src, cv::Mat &dst);
    void _visualise();
};

#endif // HSVFILTER_H
