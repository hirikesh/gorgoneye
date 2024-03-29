#include <cv.h>
#include "grayscalefilter.h"
#include "parameter.h"
#include "store.h"

GrayscaleFilter::GrayscaleFilter(Store* st, int mng, int mxg) :
    BaseFilter(st, "Grayscale"),
    visGray(false),
    minGray(mng), maxGray(mxg)
{
    _images.push_back(new ImageModeParam("Grayscale visual", &visGray, &visGrayImg, &st->dispImg));
    _images.push_back(new ImageModeParam("Grayscale mask", &visMask, &visMaskImg, &st->dispImg));
    _params.push_back(new RangeParam<int>("Min. Gray", Param::RANGE, &minGray, 0, 256, 2));
    _params.push_back(new RangeParam<int>("Max. Gray", Param::RANGE, &maxGray, 0, 256, 2));
}

void GrayscaleFilter::setParams(int mng, int mxg)
{
    minGray = mng;
    maxGray = mxg;
}

//3CH input (image), 3CH output (gray), 1CH mask (binary)
//3CH input (image), 1CH output (gray), 1CH mask (binary)
//3CH input (image), nothing, 1CH mask (binary)

void GrayscaleFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, cv::Mat& dstMsk)
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


//void GrayscaleFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Rect& srcRoi, cv::Rect& dstRoi)


void GrayscaleFilter::_filter(const cv::Mat& src)
{
    // Do colour conversion
    if(src.type() == CV_8UC1)
        grayChannel = src;
    else
        cvtColor(src, grayChannel, CV_BGR2GRAY);

    // Apply thresholds
    inRange(grayChannel, cv::Scalar(minGray), cv::Scalar(maxGray), maskImg);
}

void GrayscaleFilter::_store(cv::Mat& dstImg, cv::Mat& dstMsk)
{
    // Store conversion result
    if(dstImg.data)
    {
        if(dstImg.type() == CV_8UC1)
            grayChannel.copyTo(dstImg);
        else
            cvtColor(grayChannel, dstImg, CV_GRAY2BGR);
    }

    // Store thresholding result
    if(dstMsk.data)
        dstMsk = maskImg;
}

void GrayscaleFilter::_visualise()
{
    // Visualise gray on request
    if(visGray)
        cvtColor(grayChannel, visGrayImg, CV_GRAY2BGR);

    // Visualise mask on request
    if(visMask)
        cvtColor(maskImg, visMaskImg, CV_GRAY2BGR);
}
