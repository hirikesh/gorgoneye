#ifndef HSVFILTER_H
#define HSVFILTER_H

#include "basefilter.h"

class HSVFilter : public BaseFilter
{
public:
    HSVFilter(const std::string& nm, Store* st);

    bool hasParams() const;
    void filter(const cv::Mat& src, cv::Mat& dst, const cv::Mat& mask);
    void filter(const cv::Mat& src, cv::Mat& dst, const cv::Rect& roi);
private:
    void prepareChannels(cv::Size size);
    void cleanChannels(cv::Size size);
    cv::Mat* whiteImg;
    cv::Mat* hueChannel;
    cv::Mat* satChannel;
    cv::Mat* valChannel;
};

#endif // HSVFILTER_H
