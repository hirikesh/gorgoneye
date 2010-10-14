#include <cv.h>
#include "sobelfilter.h"
#include "parameter.h"
#include "store.h"

SobelFilter::SobelFilter(Store *st) :
    BaseFilter(st, "Sobel"),
    visSobel(false)
{
    _images.push_back(new ImageModeParam("Sobel operation", &visSobel, &visSobelImg, &st->dispImg));
}

void SobelFilter::setParams()
{

}

void SobelFilter::filter(const cv::Mat &srcImg, cv::Mat &dstImg, cv::Mat &dstMsk)
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

void SobelFilter::_filter(const cv::Mat &src)
{
    // Perform Sobel operation
    if(src.type() == CV_8UC1)
        Sobel(src, sobelImg, 1, 0, 1, 3);
    else
        Sobel(src, sobelImg, 3, 0, 1, 3);

    // Rescale pixel values
    convertScaleAbs(sobelImg, sobelImg);
}

void SobelFilter::_store(cv::Mat &dstImg, cv::Mat &dstMsk)
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
