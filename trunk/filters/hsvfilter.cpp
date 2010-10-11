#include <cv.h>
#include "hsvfilter.h"
#include "parameter.h"
#include "store.h"
#include <QDebug>

using cv::Mat;
using cv::Size;
using cv::Scalar;

HSVFilter::HSVFilter(Store* st, int mnh, int mxh, int mns, int mxs, int mnv, int mxv) :
    BaseFilter(st, "HSV"),
    visHue(false),
    minHue(mnh), maxHue(mxh),
    minSat(mns), maxSat(mxs),
    minVal(mnv), maxVal(mxv)
{
    _images.push_back(new ImageModeParam("Hue visual", &visHue, &visHueImg, &imageStore->dispImg));
    _images.push_back(new ImageModeParam("HSV mask", &visMask, &visMaskImg, &imageStore->dispImg));
    _params.push_back(new RangeParam<int>("Min. Hue", Param::RANGE, &minHue, 0, 181, 2));
    _params.push_back(new RangeParam<int>("Max. Hue", Param::RANGE, &maxHue, 0, 181, 2));
    _params.push_back(new RangeParam<int>("Min. Saturation", Param::RANGE, &minSat, 0, 256, 2));
    _params.push_back(new RangeParam<int>("Max. Saturation", Param::RANGE, &maxSat, 0, 256, 2));
    _params.push_back(new RangeParam<int>("Min. Value", Param::RANGE, &minVal, 0, 256, 2));
    _params.push_back(new RangeParam<int>("Max. Value", Param::RANGE, &maxVal, 0, 256, 2));
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

//3CH input (image), 3CH output (hue), 1CH mask (binary)
//3CH input (image), 1CH output (hue), 1CH mask (binary)
//3CH input (image), nothing, 1CH mask (binary)

void HSVFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, cv::Mat& dstMsk)
{
    // Stop here if disabled
    if(!enabled) return;

    // Convert and threshold
    _filter(srcImg);

    // Store results
    _store(dstImg, dstMsk);

    // Visualise on request
    _visualise();
}


//void HSVFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Rect& srcRoi, cv::Rect& dstRoi)


void HSVFilter::_filter(const cv::Mat &src)
{
    // Alias
    Size size = src.size();

    // Prepare images to process
    hsvImg = Mat(size, CV_8UC3);

    // Do colour conversion
    cvtColor(src, hsvImg, CV_BGR2HSV);

    // Apply thresholds
    inRange(hsvImg,
            Scalar(minHue, minSat, minVal),
            Scalar(maxHue, maxSat, maxVal),
            maskImg);

    // Populate invidivual channels
    hueChannel = Mat(size, CV_8UC1);
    satChannel = Mat(size, CV_8UC1);
    valChannel = Mat(size, CV_8UC1);
    Mat hsvChannels[] = {hueChannel, satChannel, valChannel};

    // Extract HSV channels
    split(hsvImg, hsvChannels);
}

void HSVFilter::_store(cv::Mat& dstImg, cv::Mat& dstMsk)
{
    // Store conversion result
    if(dstImg.data)
    {
        if(dstImg.type() == CV_8UC1)
            dstImg = hueChannel;
        else
        {
            _visualise3ch();
            dstImg = visHueImg;
        }
    }

    // Store thresholding result
    if(dstMsk.data)
        bitwise_and(dstMsk, maskImg, dstMsk);
    else
        dstMsk = maskImg;
}

void HSVFilter::_visualise()
{
    // Visualise hue on request
    if(visHue)
        _visualise3ch();

    // Visualise mask on request
    if(visMask)
        cvtColor(maskImg, visMaskImg, CV_GRAY2BGR);
}

void HSVFilter::_visualise3ch()
{
    // Prepare individual channels
    Mat neutralChannel(hsvImg.size(), CV_8UC1, Scalar(255));

    // Hue image
    Mat hueImg[] = {hueChannel, neutralChannel, neutralChannel};
    merge(hueImg, 3, visHueImg);
    cvtColor(visHueImg, visHueImg, CV_HSV2BGR);
}
