#include <cv.h>
#include "cannyedgefilter.h"
#include "parameter.h"
#include "store.h"

CannyEdgeFilter::CannyEdgeFilter(Store* st, double tl, double th) :
    BaseFilter(st, "Canny Edge"),
    visCanny(false),
    threshLow(tl), threshHigh(th)
{
    _images.push_back(new ImageModeParam("Canny edges", &visCanny, &visCannyImg, &st->dispImg));
    _params.push_back(new RangeParam<double>("Canny threshold low", Param::RANGE_DBL, &threshLow, 0, 1200 , 10));
    _params.push_back(new RangeParam<double>("Canny threshold high", Param::RANGE_DBL, &threshHigh, 0, 1200, 10));
}

void CannyEdgeFilter::setParams(double tl, double th)
{
    threshLow = tl;
    threshHigh = th;
}

void CannyEdgeFilter::filter(const cv::Mat &srcImg, cv::Mat &dstImg, cv::Mat &dstMsk)
{
    // Stop here if disabled
    if(!enabled) return;

    // Canny edge operation
    _filter(srcImg);

    // Store result
    _store(dstImg, dstMsk);

    // Visualise on request
    _visualise();
}

void CannyEdgeFilter::_filter(const cv::Mat &src)
{
    // Prepare input
    cv::Mat grayChannel;

    // Find Canny edges
    cvtColor(src, grayChannel, CV_BGR2GRAY);
    Canny(grayChannel, cannyImg, threshLow, threshHigh, 3);
}

void CannyEdgeFilter::_store(cv::Mat &dstImg, cv::Mat &dstMsk)
{
    // Store Canny edge results
    if(dstImg.data)
    {
        if(dstImg.type() == CV_8UC1)
            cannyImg.copyTo(dstImg);
        else
            cvtColor(cannyImg, dstImg, CV_GRAY2BGR);
    }
}

void CannyEdgeFilter::_visualise()
{
    // Visualise on request
    if(visCanny)
        cvtColor(cannyImg, visCannyImg, CV_GRAY2BGR);
}
