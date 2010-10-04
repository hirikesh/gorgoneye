#include <cv.h>
#include "grayscalefilter.h"

using cv::Mat;

GrayscaleFilter::GrayscaleFilter(const std::string& nm, Store* st) :
        BaseFilter(nm, st)
{
}

bool GrayscaleFilter::hasParams()const
{
    return true;
}

void GrayscaleFilter::filter(const cv::Mat& src, cv::Mat& dst, const cv::Mat& mask)
{
    Mat tmp(src.size(), CV_8UC1);
    cvtColor(src, tmp, CV_BGR2GRAY);
    cvtColor(tmp, dst, CV_GRAY2BGR);
    // TODO: Apply Mask
}

void GrayscaleFilter::filter(const cv::Mat& src, cv::Mat& dst, const cv::Rect& roi)
{
    Mat srcROI = src(roi);
    Mat tmp(srcROI.size(), CV_8UC1);
    Mat dstROI = dst(roi);
    cvtColor(srcROI, tmp, CV_BGR2GRAY);
    cvtColor(tmp, dstROI, CV_GRAY2BGR);
}
