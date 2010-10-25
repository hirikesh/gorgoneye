#include <cv.h>
#include "harriscornerfilter.h"
#include "parameter.h"
#include "store.h"

HarrisCornerFilter::HarrisCornerFilter(Store* st, int ns, int ks, double k) :
    BaseFilter(st, "Harris Corner"),
    visHarris(false),
    neighbourhoodSize(ns),
    sobelKernelSize(ks),
    freeParamK(k)
{
    _images.push_back(new ImageModeParam("Harris corners", &visHarris, &visHarrisImg, &st->dispImg));
    _params.push_back(new RangeParam<int>("Block size", Param::RANGE, &neighbourhoodSize, 1, 64, 2));
    _params.push_back(new RangeParam<double>("Free parameter k", Param::RANGE_DBL, &freeParamK, 0.00, 1.00 , 0.01));
    _params.push_back((new RangeParam<int>("Sobel kernel size (odd)", Param::RANGE, &sobelKernelSize, 1, 21, 2)));
}

void HarrisCornerFilter::setParams(int ns, int ks, double k)
{
    neighbourhoodSize = ns;
    sobelKernelSize = ks;
    freeParamK = k;
}

void HarrisCornerFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, cv::Mat& dstMsk)
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

void HarrisCornerFilter::_filter(const cv::Mat& src)
{
    // Prepare input
    cv::Mat grayChannel;
    cv::Mat harrisImgF;

    // Find Canny edges
    cvtColor(src, grayChannel, CV_BGR2GRAY);
    cornerHarris(grayChannel, harrisImgF, neighbourhoodSize, sobelKernelSize + sobelKernelSize%2 - 1, freeParamK);

    // Convert from 32FC1 to 8UC1
    harrisImgF.convertTo(harrisImg, CV_8UC1, 255, 0);
}

void HarrisCornerFilter::_store(cv::Mat& dstImg, cv::Mat& dstMsk)
{
    // Store Canny edge results
    if(dstImg.data)
    {
        if(dstImg.type() == CV_8UC1)
            harrisImg.copyTo(dstImg);
        else
            cvtColor(harrisImg, dstImg, CV_GRAY2BGR);
    }
}

void HarrisCornerFilter::_visualise()
{
    // Visualise on request
    if(visHarris)
        cvtColor(harrisImg, visHarrisImg, CV_GRAY2BGR);
}
