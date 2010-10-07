#include <cv.h>
#include "hsvfilter.h"
#include "parameter.h"

using cv::Mat;

HSVFilter::HSVFilter(const std::string& nm, Store* st) :
        BaseFilter(nm, st)
{
//    hsvChannels[0] = hueChannel;
//    hsvChannels[1] = satChannel;
//    hsvChannels[2] = valChannel;
    filterParams.push_back(new ModeParam("HSV Version 1", &bool1, false));
    filterParams.push_back(new ModeParam("HSV Version 2", &bool2, false));
    filterParams.push_back(new ModeParam("HSV Version 3", &bool3, false));
}

bool HSVFilter::hasParams()const
{
    return true;
}

void HSVFilter::filter(const cv::Mat& src, cv::Mat& dst, const cv::Mat& mask)
{
    if (!enabled)
        return;
    cv::Size srcSize = src.size();
    Mat tmp(srcSize, CV_8UC3);
    cvtColor(src, tmp, CV_BGR2HSV);
    prepareChannels(srcSize);

    Mat hsvChannels[] = {*hueChannel, *satChannel, *valChannel};
    split(tmp, hsvChannels);
    Mat hueImg[] = {*hueChannel, *whiteImg, *whiteImg};
    merge(hueImg, 3, dst);
    cvtColor(dst, dst, CV_HSV2RGB);
    cleanChannels(srcSize);
    // TODO: Apply Mask
}

void HSVFilter::filter(const cv::Mat& src, cv::Mat& dst, const cv::Rect& roi)
{
    if (!enabled)
        return;
    Mat srcROI = src(roi);
    Mat dstROI = dst(roi);
    cv::Size srcSize = src(roi).size();
    Mat tmp(srcSize, CV_8UC3);
    cvtColor(srcROI, tmp, CV_BGR2HSV);
    prepareChannels(srcSize);

    Mat hsvChannels[] = {*hueChannel, *satChannel, *valChannel};
    split(tmp, hsvChannels);
    Mat hueImg[] = {*hueChannel, *whiteImg, *whiteImg};
    merge(hueImg, 3, dstROI);
    cvtColor(dstROI, dstROI, CV_HSV2BGR);
    cleanChannels(srcSize);
}

void HSVFilter::prepareChannels(cv::Size size)
{
    whiteImg = new Mat(size, CV_8UC1, cv::Scalar(255));
    hueChannel = new Mat(size, CV_8UC1);
    satChannel = new Mat(size, CV_8UC1);
    valChannel= new Mat(size, CV_8UC1);
}

void HSVFilter::cleanChannels(cv::Size size)
{
    delete whiteImg;
    delete hueChannel;
    delete satChannel;
    delete valChannel;
}
