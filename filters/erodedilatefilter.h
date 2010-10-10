#ifndef ERODEDILATEFILTER_H
#define ERODEDILATEFILTER_H

#include "basefilter.h"

class ErodeDilateFilter : public BaseFilter
{
public:
    ErodeDilateFilter(const std::string& nm, Store* st);

    bool hasParams() const;
    void setParams(int);

    void filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Mat& srcMsk, cv::Mat& dstMsk);
//    void filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Rect& srcRoi, cv::Rect& dstRoi);

private:
    bool visMorph;
    int morphLevel;

    cv::Mat morphImg;
    cv::Mat visMorphImg;

    void _filter(const cv::Mat &src, cv::Mat &dst);
    void _visualise();
};

#endif // ERODEDILATEFILTER_H
