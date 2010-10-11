#include <cv.h>
#include "grayscalefilter.h"
#include "parameter.h"
#include "store.h"

using cv::Mat;
using cv::Size;
using cv::Scalar;

GrayscaleFilter::GrayscaleFilter(const std::string& nm, Store* st, int mng, int mxg) :
    BaseFilter(nm, st),
    visGray(false),
    minGray(mng), maxGray(mxg)
{
    filterParams.push_back(new ImageModeParam("Grayscale visual", &visGray, &visGrayImg, &imageStore->dispImg));
    filterParams.push_back(new RangeParam<int>("Min. Gray", Param::RANGE, &minGray, 0, 256, 2));
    filterParams.push_back(new RangeParam<int>("Max. Gray", Param::RANGE, &maxGray, 0, 256, 2));
}

bool GrayscaleFilter::hasParams() const
{
    return true;
}

void GrayscaleFilter::setParams(int mng, int mxg)
{
    minGray = mng;
    maxGray = mxg;
}

void GrayscaleFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Mat& srcMsk, cv::Mat& dstMsk)
{
    // Stop here if disabled
    if(!enabled) return;

    // Convert and threshold
    _filter(srcImg, dstMsk);

    // Combine if requested
    if(srcMsk.data)
        bitwise_and(srcMsk, dstMsk, dstMsk);

    // Convert back
    if(dstImg.data || visGray)
        _visualise();

    // Store the result
    if(dstImg.data)
        dstImg = visGrayImg;
}


//void GrayscaleFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Rect& srcRoi, cv::Rect& dstRoi)


void GrayscaleFilter::_filter(const cv::Mat& src, cv::Mat& dst)
{
    // Prepare images to process
    grayChannel = Mat(src.size(), CV_8UC1);

    // Do colour conversion
    cvtColor(src, grayChannel, CV_BGR2GRAY);

    // Apply thresholds
    inRange(grayChannel, Scalar(minGray), Scalar(maxGray), dst);
}

void GrayscaleFilter::_visualise()
{
    cvtColor(grayChannel, visGrayImg, CV_GRAY2BGR);
}
