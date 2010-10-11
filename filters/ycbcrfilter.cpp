#include <cv.h>
#include "ycbcrfilter.h"
#include "parameter.h"
#include "store.h"

using cv::Mat;
using cv::Size;
using cv::Scalar;

YCbCrFilter::YCbCrFilter(const std::string &nm, Store *st, int mny, int mxy, int mnr, int mxr, int mnb, int mxb) :
    BaseFilter(nm, st),
    dstCr(true),
    visCr(false), visCb(false),
    minLuma(mny), maxLuma(mxy),
    minCr(mnr), maxCr(mxr),
    minCb(mnb), maxCb(mxb)
{
    _params.push_back((new ModeParam("Chrom. Red | Chrom. Blue", &dstCr, true)));
    _images.push_back(new ImageModeParam("Chrominance Red visual", &visCr, &visCrImg, &imageStore->dispImg));
    _images.push_back(new ImageModeParam("Chrominance Blue visual", &visCb, &visCbImg, &imageStore->dispImg));
    _images.push_back(new ImageModeParam("YCbCr mask", &visMask, &visMaskImg, &imageStore->dispImg));
    _params.push_back(new RangeParam<int>("Min. Luma", Param::RANGE, &minLuma, 0, 256, 2));
    _params.push_back(new RangeParam<int>("Max. Luma", Param::RANGE, &maxLuma, 0, 256, 2));
    _params.push_back(new RangeParam<int>("Min. Chrom. Red", Param::RANGE, &minCr, 0, 256, 2));
    _params.push_back(new RangeParam<int>("Max. Chrom. Red", Param::RANGE, &maxCr, 0, 256, 2));
    _params.push_back(new RangeParam<int>("Min. Chrom. Blue", Param::RANGE, &minCb, 0, 256, 2));
    _params.push_back(new RangeParam<int>("Max. Chrom. Blue", Param::RANGE, &maxCb, 0, 256, 2));
}

void YCbCrFilter::setParams(int mny, int mxy, int mnr, int mxr, int mnb, int mxb)
{
    minLuma = mny;
    maxLuma = mxy;
    minCr = mnr;
    maxCr = mxr;
    minCb = mnb;
    maxCb = mxb;
}

void YCbCrFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Mat& srcMsk, cv::Mat& dstMsk)
{
    // Stop here if disabled
    if(!enabled) return;

    // Convert and threshold
    Mat tmpMsk;
    _filter(srcImg, tmpMsk);

    // View mask if asked
    if(visMask)
        cvtColor(tmpMsk, visMaskImg, CV_GRAY2BGR);

    // Combine if requested
    if(srcMsk.data)
        bitwise_and(srcMsk, tmpMsk, dstMsk);
    else
        dstMsk = tmpMsk;

    // Convert back
    if(dstImg.data || visCr || visCb)
        _visualise();

    // Store the result
    if(dstImg.data)
    {
        if(dstCr)
            dstImg = visCrImg;
        else
            dstImg = visCbImg;
    }
}


//void YCbCrFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Rect& srcRoi, cv::Rect& dstRoi)


void YCbCrFilter::_filter(const cv::Mat &src, cv::Mat &dst)
{
    // Alias
    Size srcImgSize = src.size();

    // Prepare images to process
    yccImg = Mat(srcImgSize, CV_8UC3);

    // Do colour conversion
    cvtColor(src, yccImg, CV_BGR2YCrCb);

    // Apply thresholds
    inRange(yccImg,
            Scalar(minLuma, minCr, minCb),
            Scalar(maxLuma, maxCr, maxCb),
            dst);

    // Populate individual channels
    lumaChannel = Mat(srcImgSize, CV_8UC1);
    crChannel = Mat(srcImgSize, CV_8UC1);
    cbChannel = Mat(srcImgSize, CV_8UC1);
    Mat yccChannels[] = {lumaChannel, crChannel, cbChannel};

    // Extract YCC channels
    split(yccImg, yccChannels);
}

void YCbCrFilter::_visualise()
{
    // Prepare individual channels
    Mat neutralChannel(yccImg.size(), CV_8UC1, Scalar(128));

    // Chroma red image
    Mat crImg[] = {neutralChannel, crChannel, neutralChannel};
    merge(crImg, 3, visCrImg);
    cvtColor(visCrImg, visCrImg, CV_YCrCb2BGR);

    // Chroma blue image
    Mat cbImg[] = {neutralChannel, neutralChannel, cbChannel};
    merge(cbImg, 3, visCbImg);
    cvtColor(visCbImg, visCbImg, CV_YCrCb2BGR);
}
