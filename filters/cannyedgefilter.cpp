#include <cv.h>
#include "cannyedgefilter.h"
#include "parameter.h"
#include "store.h"

CannyEdgeFilter::CannyEdgeFilter(Store* st, double tl, double th, int ks) :
    BaseFilter(st, "Canny Edge"),
    visCanny(false),
    threshLow(tl), threshHigh(th),
    sobelKernelSize(ks)
{
    _images.push_back(new ImageModeParam("Canny edges", &visCanny, &visCannyImg, &st->dispImg));
    _params.push_back(new RangeParam<double>("Canny threshold low", Param::RANGE_DBL, &threshLow, 0, 90000, 10));
    _params.push_back(new RangeParam<double>("Canny threshold high", Param::RANGE_DBL, &threshHigh, 0, 90000, 10));
    _params.push_back((new RangeParam<int>("Sobel kernel size (ODD NUMBERS ONLY)", Param::RANGE, &sobelKernelSize, 3, 7, 2)));
}

void CannyEdgeFilter::setParams(double tl, double th, int ks)
{
    threshLow = tl;
    threshHigh = th;
    sobelKernelSize = ks;
}

void CannyEdgeFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, cv::Mat& dstMsk)
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

void CannyEdgeFilter::_filter(const cv::Mat& src)
{
    // Prepare input
    cv::Mat grayChannel;

    // Find Canny edges
    cvtColor(src, grayChannel, CV_BGR2GRAY);
    sobelKernelSize = sobelKernelSize % 2 ? sobelKernelSize++ : sobelKernelSize;
    Canny(grayChannel, cannyImg, threshLow, threshHigh, sobelKernelSize);
}

void CannyEdgeFilter::_store(cv::Mat& dstImg, cv::Mat& dstMsk)
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
