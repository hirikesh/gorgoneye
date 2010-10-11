#include <cv.h>
#include "erodedilatefilter.h"
#include "parameter.h"
#include "store.h"

using cv::Mat;
using cv::Size;
using cv::Scalar;

ErodeDilateFilter::ErodeDilateFilter(const std::string& nm, Store* st, int ml) :
    BaseFilter(nm, st),
    visMorph(false),
    morphLevel(ml)
{
    filterParams.push_back(new ImageModeParam("Morphology visual", &visMorph, &visMorphImg, &imageStore->dispImg));
    filterParams.push_back(new RangeParam<int>("Morphology Depth", Param::RANGE, &morphLevel, 0, 12, 1));
}

bool ErodeDilateFilter::hasParams() const
{
    return true;
}

void ErodeDilateFilter::setParams(int ml)
{
    morphLevel = ml;
}

void ErodeDilateFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Mat& srcMsk, cv::Mat& dstMsk)
{
    // Stop here if disabled
    if(!enabled) return;

    // Erode and dilate
    _filter(srcMsk, dstMsk);

    // Visualise
    if(dstImg.data || visMorph)
        _visualise();

    // Store the result
    if(dstImg.data)
        dstImg = visMorphImg;
}


//void ErodeDilateFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Rect& srcRoi, cv::Rect& dstRoi)


void ErodeDilateFilter::_filter(const cv::Mat& src, cv::Mat& dst)
{
    // Perform erosion operations
    morphologyEx(src, morphImg, cv::MORPH_CLOSE, Mat(), cv::Point(-1,-1), morphLevel);

    // Perform dilation operations
    morphologyEx(morphImg, morphImg, cv::MORPH_OPEN, Mat(), cv::Point(-1,-1), morphLevel);

    // Apply result
    dst = morphImg;
}

void ErodeDilateFilter::_visualise()
{
    cvtColor(morphImg, visMorphImg, CV_GRAY2BGR);
}

