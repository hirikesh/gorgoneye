#include <cv.h>
#include "grayscalefilter.h"
#include "parameter.h"
#include "store.h"

using cv::Mat;
using cv::Size;
using cv::Scalar;

GrayscaleFilter::GrayscaleFilter(const std::string& nm, Store* st) :
    BaseFilter(nm, st),
    visGray(false),
    minGray(0), maxGray(255)
{
    filterParams.push_back(new ImageModeParam("Grayscale visual", &visGray, &visGrayImg, &st->dispImg));
    filterParams.push_back(new RangeParam<int>("Min. Gray", Param::RANGE, &minGray, 0, 255, 2));
    filterParams.push_back(new RangeParam<int>("Max. Gray", Param::RANGE, &maxGray, 0, 255, 2));
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
    grayImg = Mat(src.size(), CV_8UC1);

    // Do colour conversion
    cvtColor(src, grayImg, CV_BGR2GRAY);

    // Apply thresholds
    inRange(grayImg, Scalar(minGray), Scalar(maxGray), dst);
}

void GrayscaleFilter::_visualise()
{
    cvtColor(grayImg, visGrayImg, CV_GRAY2BGR);
}
