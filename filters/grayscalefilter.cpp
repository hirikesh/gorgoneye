#include <cv.h>
#include "grayscalefilter.h"
#include "parameter.h"

using cv::Mat;

GrayscaleFilter::GrayscaleFilter(const std::string& nm, Store* st) :
        BaseFilter(nm, st)
{
    filterParams.push_back(new ModeParam("GS Version 1", &bool1, false));
    filterParams.push_back(new ModeParam("GS Version 2", &bool2, false));
    filterParams.push_back(new ModeParam("GS Version 3", &bool3, false));
}

bool GrayscaleFilter::hasParams()const
{
    return true;
}

void GrayscaleFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Mat& srcMsk, cv::Mat& dstMsk)
{
    if (!enabled)
        return;
    Mat tmp(srcImg.size(), CV_8UC1);
    cvtColor(srcImg, tmp, CV_BGR2GRAY);
    cvtColor(tmp, dstImg, CV_GRAY2BGR);
    // TODO: Apply Mask

}

void GrayscaleFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Rect& srcRoi, cv::Rect& dstRoi)
{
    if (!enabled)
        return;
    Mat srcROI = srcImg(srcRoi);
    Mat tmp(srcROI.size(), CV_8UC1);
    Mat dstROI = dstImg(srcRoi);
    cvtColor(srcROI, tmp, CV_BGR2GRAY);
    cvtColor(tmp, dstROI, CV_GRAY2BGR);
}
