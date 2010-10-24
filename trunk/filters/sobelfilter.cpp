#include <cv.h>
#include "sobelfilter.h"
#include "parameter.h"
#include "store.h"

SobelFilter::SobelFilter(Store *st, int dx, int dy, int ks) :
    BaseFilter(st, "Sobel"),
    visSobel(false),
    derivX(dx),
    derivY(dy),
    kernelSize(ks)
{
    _images.push_back(new ImageModeParam("Sobel operation", &visSobel, &visSobelImg, &st->dispImg));
    _params.push_back((new RangeParam<int>("Order of X derivative", Param::RANGE, &derivX, 0, 2, 1)));
    _params.push_back((new RangeParam<int>("Order of Y derivative", Param::RANGE, &derivY, 0, 2, 1)));
    _params.push_back((new RangeParam<int>("Kernel size (ODD NUMBERS ONLY)", Param::RANGE, &kernelSize, 1, 21, 2)));
}

void SobelFilter::setParams(int dx, int dy, int ks)
{
    derivX = dx;
    derivY = dy;
    kernelSize = ks;
}

void SobelFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, cv::Mat& dstMsk)
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

void SobelFilter::_filter(const cv::Mat& src)
{
    // Perform Sobel operation
    if(src.type() == CV_8UC1)
        Sobel(src, sobelImg, 1, derivX, derivY, kernelSize + kernelSize%2 - 1);
    else
        Sobel(src, sobelImg, 3, derivX, derivY, kernelSize + kernelSize%2 - 1);

    // Rescale pixel values
    convertScaleAbs(sobelImg, sobelImg);
}

void SobelFilter::_store(cv::Mat& dstImg, cv::Mat& dstMsk)
{
    // Store sobel-operated result
    if(dstImg.data)
        sobelImg.copyTo(dstImg);

    // Store thresholding result

}

void SobelFilter::_visualise()
{
    // Visualise sobel on request
    if(visSobel)
        visSobelImg = sobelImg;

    // Visualise thresholding result

}
