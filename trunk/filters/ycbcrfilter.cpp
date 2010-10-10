#include <cv.h>
#include "ycbcrfilter.h"
#include "parameter.h"

using cv::Mat;
using cv::Size;
using cv::Scalar;

YCbCrFilter::YCbCrFilter(const std::string& nm, Store* st) :
    BaseFilter(nm, st),
    dstCr(true),
    visCr(false), visCb(false),
    minLuma(16), maxLuma(240),
    minCr(0), maxCr(255),
    minCb(0), maxCb(255)
{
    filterParams.push_back((new ModeParam("Chrom. Red | Chrom. Blue", &dstCr, true)));
    /*filterParams.push_back(new ImageModeParam("Chrominance Red visual", &visCr, &visCrImg));
    filterParams.push_back(new ImageModeParam("Chrominance Blue visual", &visCb, &visCbImg))*/;
    filterParams.push_back(new RangeParam<int>("Min. Luma", Param::RANGE, &minLuma, 16, 240, 2));
    filterParams.push_back(new RangeParam<int>("Max. Luma", Param::RANGE, &maxLuma, 16, 240, 2));
    filterParams.push_back(new RangeParam<int>("Min. Chrom. Red", Param::RANGE, &minCr, 0, 255, 2));
    filterParams.push_back(new RangeParam<int>("Max. Chrom. Red", Param::RANGE, &maxCr, 0, 255, 2));
    filterParams.push_back(new RangeParam<int>("Min. Chrom. Blue", Param::RANGE, &minCb, 0, 255, 2));
    filterParams.push_back(new RangeParam<int>("Max. Chrom. Blue", Param::RANGE, &maxCb, 0, 255, 2));
}

bool YCbCrFilter::hasParams() const
{
    return true;
}

void YCbCrFilter::setParams(int mny, int mxy, int mncr, int mxcr, int mncb, int mxcb)
{
    minLuma = mny;
    maxLuma = mxy;
    minCr = mncr;
    maxCr = mxcr;
    minCb = mncb;
    maxCb = mxcb;
}

void YCbCrFilter::filter(const cv::Mat &srcImg, cv::Mat &dstMsk)
{
    // Stop here if disabled
    if (!enabled) return;

    // Convert and threshold
    _filter(srcImg, dstMsk);

    // Visualise if set
    if(visCr || visCb)
        _visualise();
}

void YCbCrFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, cv::Mat& dstMsk)
{
    // Stop here if disabled
    if (!enabled) return;

    // Convert and threshold
    _filter(srcImg, dstMsk);

    // Convert back
    _visualise();
    if(dstCr)
        dstImg = visCrImg;
    else
        dstImg = visCbImg;
}

//void YCbCrFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Mat& srcMsk, cv::Mat& dstMsk)

//void YCbCrFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Rect& srcRoi, cv::Rect& dstRoi)


void YCbCrFilter::_filter(const cv::Mat &srcImg, cv::Mat &dstMsk)
{
    // Prepare images to process
    yccImg = Mat(srcImg.size(), CV_8UC3);

    // Do colour conversion
    cvtColor(srcImg, yccImg, CV_BGR2YCrCb);

    // Apply thresholds
    inRange(yccImg,
            Scalar(minLuma, minCr, minCb),
            Scalar(maxLuma, maxCr, maxCb),
            dstMsk);
}

void YCbCrFilter::_visualise()
{
    // Alias
    Size size = yccImg.size();

    // Prepare individual channels
    Mat neutralChannel(size, CV_8UC1, Scalar(128));
    Mat lumaChannel(size, CV_8UC1);
    Mat crChannel(size, CV_8UC1);
    Mat cbChannel(size, CV_8UC1);
    Mat yccChannels[] = {lumaChannel, crChannel, cbChannel};

    // Extract YCC channels
    split(yccImg, yccChannels);

    // Chroma red image
    Mat crImg[] = {neutralChannel, crChannel, neutralChannel};
    merge(crImg, 3, visCrImg);
    cvtColor(visCrImg, visCrImg, CV_YCrCb2BGR);

    // Chroma blue image
    Mat cbImg[] = {neutralChannel, neutralChannel, cbChannel};
    merge(cbImg, 3, visCbImg);
    cvtColor(visCbImg, visCbImg, CV_YCrCb2BGR);
}
