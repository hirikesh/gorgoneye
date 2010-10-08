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
    void filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Mat& srcMsk, cv::Mat& dstMsk);
    void filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Rect& srcRoi, cv::Rect& dstRoi);
private:
    void prepareChannels(cv::Size size);
    void cleanChannels(cv::Size size);
    cv::Mat* whiteImg;
    cv::Mat* hueChannel;
    cv::Mat* satChannel;
    cv::Mat* valChannel;

    int bool1;
    int bool2;
    int bool3;
};

#endif // HSVFILTER_H
