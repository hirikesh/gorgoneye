#ifndef GRAYSCALEFILTER_H
#define GRAYSCALEFILTER_H

#include "basefilter.h"

class GrayscaleFilter : public BaseFilter
{
public:
    GrayscaleFilter(const std::string& nm, Store* st);
    bool hasParams() const;
    void filter(const cv::Mat& src, cv::Mat& dst, const cv::Mat& mask);
    void filter(const cv::Mat& src, cv::Mat& dst, const cv::Rect& roi);
private:
    int bool1;
    int bool2;
    int bool3;

};

#endif // GRAYSCALEFILTER_H
