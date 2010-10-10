#include <cv.h>
#include "grayscalefilter.h"
#include "parameter.h"

using cv::Mat;
using cv::Size;
using cv::Scalar;

GrayscaleFilter::GrayscaleFilter(const std::string& nm, Store* st) :
    BaseFilter(nm, st),
    visGray(false),
    minGray(0), maxGray(255)
{
    //filterParams.push_back(new ImageModeParam("Grayscale visual", &visGray, &visGrayImg, &st->dispImg));
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

void GrayscaleFilter::filter(const cv::Mat& srcImg, cv::Mat& dstMsk)
{
    // Stop here if disabled
    if (!enabled) return;

    // Convert and threshold
    _filter(srcImg, dstMsk);

    // Visualise if set
    _visualise();
}

void GrayscaleFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, cv::Mat& dstMsk)
{
    // Stop here if disabled
    if (!enabled) return;

    // Convert and threshold
    filter(srcImg, dstMsk);

    // Convert back
    _visualise();
    dstImg = visGrayImg;
}

//void GrayscaleFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Mat& srcMsk, cv::Mat& dstMsk)
//{
//    // Stop here if disabled
//    if (!enabled) return;

//    // Convert and threshold
//    filter(srcImg, dstMsk);

//    // Convert back
//    cvtColor(grayChannel, dstImg, CV_GRAY2BGR);
//}

//void GrayscaleFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Rect& srcRoi, cv::Rect& dstRoi)
//{
//    if (!enabled)
//        return;

//    // Prepare images to process
//    Mat srcImgRoi = srcImg(srcRoi);
//    Mat dstImgRoi = dstImg(srcRoi);
//    Mat tmpImgRoi(srcRoi.size(), CV_8UC1);

//    // Do colour conversion
//    cvtColor(srcImgRoi, tmpImgRoi, CV_BGR2GRAY);

//    // Apply thresholds
////    cv::inRange(tmpImgRoi, Scalar(minGray), Scalar(maxGray), dstRoi);

//    // Convert to BGR if visual is needed
//    if(&dstImg != NULL)
//        cvtColor(tmpImgRoi, dstImgRoi, CV_GRAY2BGR);
//}

// Basic filter: Input source image produces Output thresholded mask image
void GrayscaleFilter::_filter(const cv::Mat& srcImg, cv::Mat& dstMsk)
{
    // Prepare images to process
    grayImg = Mat(srcImg.size(), CV_8UC1);

    // Do colour conversion
    cvtColor(srcImg, grayImg, CV_BGR2GRAY);

    // Apply thresholds
    inRange(grayImg, Scalar(minGray), Scalar(maxGray), dstMsk);
}

void GrayscaleFilter::_visualise()
{
    cvtColor(grayImg, visGrayImg, CV_GRAY2BGR);
}
