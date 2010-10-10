#include <cv.h>
#include "hsvfilter.h"
#include "parameter.h"
#include "store.h"

using cv::Mat;
using cv::Size;
using cv::Scalar;

HSVFilter::HSVFilter(const std::string& nm, Store* st) :
    BaseFilter(nm, st),
    visHue(false),
    minHue(0), maxHue(180),
    minSat(0), maxSat(255),
    minVal(0), maxVal(255)
{
    filterParams.push_back(new ImageModeParam("Hue visual", &visHue, &visHueImg, &st->dispImg));
    filterParams.push_back(new RangeParam<int>("Min. Hue", Param::RANGE, &minHue, 0, 180, 2));
    filterParams.push_back(new RangeParam<int>("Max. Hue", Param::RANGE, &maxHue, 0, 180, 2));
    filterParams.push_back(new RangeParam<int>("Min. Saturation", Param::RANGE, &minSat, 0, 255, 2));
    filterParams.push_back(new RangeParam<int>("Max. Saturation", Param::RANGE, &maxSat, 0, 255, 2));
    filterParams.push_back(new RangeParam<int>("Min. Value", Param::RANGE, &minVal, 0, 255, 2));
    filterParams.push_back(new RangeParam<int>("Max. Value", Param::RANGE, &maxVal, 0, 255, 2));
}

bool HSVFilter::hasParams() const
{
    return true;
}

void HSVFilter::setParams(int mnh, int mxh, int mns, int mxs, int mnv, int mxv)
{
    minHue = mnh;
    maxHue = mxh;
    minSat = mns;
    maxSat = mxs;
    minVal = mnv;
    maxVal = mxv;
}

void HSVFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Mat& srcMsk, cv::Mat& dstMsk)
{
    // Stop here if disabled
    if(!enabled) return;

    // Convert and threshold
    _filter(srcImg, dstMsk);

    // Convert back
    if(dstImg.data || visHue)
        _visualise();

    // Store the result
    if(dstImg.data)
        dstImg = visHueImg;
}

//void HSVFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Rect& srcRoi, cv::Rect& dstRoi)


void HSVFilter::_filter(const cv::Mat &srcImg, cv::Mat &dstMsk)
{
    // Prepare images to process
    hsvImg = Mat(srcImg.size(), CV_8UC3);

    // Do colour conversion
    cvtColor(srcImg, hsvImg, CV_BGR2HSV);

    // Apply thresholds
    inRange(hsvImg,
            Scalar(minHue, minSat, minVal),
            Scalar(maxHue, maxSat, maxVal),
            dstMsk);
}

void HSVFilter::_visualise()
{
    // Alias
    Size size = hsvImg.size();

    // Prepare individual channels
    Mat neutralChannel(size, CV_8UC1, Scalar(255));
    Mat hueChannel(size, CV_8UC1);
    Mat satChannel(size, CV_8UC1);
    Mat valChannel(size, CV_8UC1);
    Mat hsvChannels[] = {hueChannel, satChannel, valChannel};

    // Extract HSV channels
    split(hsvImg, hsvChannels);

    // Hue image
    Mat hueImg[] = {hueChannel, neutralChannel, neutralChannel};
    merge(hueImg, 3, visHueImg);
    cvtColor(visHueImg, visHueImg, CV_HSV2BGR);
}
