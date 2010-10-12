#include <cv.h>
#include "erodedilatefilter.h"
#include "parameter.h"
#include "store.h"

using cv::Mat;
using cv::Size;
using cv::Scalar;

ErodeDilateFilter::ErodeDilateFilter(Store* st, int ml) :
    BaseFilter(st, "Erode-Dilate"),
    visMorph(false),
    morphLevel(ml)
{
    _images.push_back(new ImageModeParam("Morphology visual", &visMorph, &visMorphImg, &st->dispImg));
    _params.push_back(new RangeParam<int>("Morphology Depth", Param::RANGE, &morphLevel, 0, 12, 1));
}

void ErodeDilateFilter::setParams(int ml)
{
    morphLevel = ml;
}

//3CH input (image), 3CH output (morphed image), nothing
//1CH input (image), 1CH mask (binary), nothing

void ErodeDilateFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, cv::Mat& dstMsk)
{
    // Stop here if disabled
    if(!enabled) return;

    // Erode and dilate
    _filter(srcImg);

    // Store result
    _store(dstImg, dstMsk);

    // Visualise on request
    _visualise();
}


//void ErodeDilateFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Rect& srcRoi, cv::Rect& dstRoi)


void ErodeDilateFilter::_filter(const cv::Mat& src)
{
    // Perform erosion operations
    morphologyEx(src, morphImg, cv::MORPH_CLOSE, Mat(), cv::Point(-1,-1), morphLevel);

    // Perform dilation operations
    morphologyEx(morphImg, morphImg, cv::MORPH_OPEN, Mat(), cv::Point(-1,-1), morphLevel);
}

void ErodeDilateFilter::_store(cv::Mat &dstImg, cv::Mat &dstMsk)
{
    // Store erosion and dilation result
    if(dstImg.data)
        morphImg.copyTo(dstImg);
}

void ErodeDilateFilter::_visualise()
{
    // Visualise morphed image on request
    if(visMorph)
    {
        if(morphImg.type() == CV_8UC1)
            cvtColor(morphImg, visMorphImg, CV_GRAY2BGR);
        else
            visMorphImg = morphImg;
    }
}

