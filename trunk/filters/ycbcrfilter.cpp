#include <cv.h>
#include "ycbcrfilter.h"
#include "parameter.h"
#include "store.h"

using cv::Mat;
using cv::Scalar;

YCbCrFilter::YCbCrFilter(Store *st, int mny, int mxy, int mnr, int mxr, int mnb, int mxb, int dcr) :
    BaseFilter(st, "YCbCr"),
    visCr(false), visCb(false),
    minY(mny), maxY(mxy),
    minCr(mnr), maxCr(mxr),
    minCb(mnb), maxCb(mxb),
    dstCr(dcr)
{
    _params.push_back(new RangeParam<int>("Chrom. Blue | Chrom. Red", Param::RANGE, &dstCr, 0, 1, 1));
    _images.push_back(new ImageModeParam("Chrominance Red visual", &visCr, &visCrImg, &st->dispImg));
    _images.push_back(new ImageModeParam("Chrominance Blue visual", &visCb, &visCbImg, &st->dispImg));
    _images.push_back(new ImageModeParam("YCbCr mask", &visMask, &visMaskImg, &st->dispImg));
    _params.push_back(new RangeParam<int>("Min. Luma", Param::RANGE, &minY, 0, 256, 2));
    _params.push_back(new RangeParam<int>("Max. Luma", Param::RANGE, &maxY, 0, 256, 2));
    _params.push_back(new RangeParam<int>("Min. Chrom. Red", Param::RANGE, &minCr, 0, 256, 2));
    _params.push_back(new RangeParam<int>("Max. Chrom. Red", Param::RANGE, &maxCr, 0, 256, 2));
    _params.push_back(new RangeParam<int>("Min. Chrom. Blue", Param::RANGE, &minCb, 0, 256, 2));
    _params.push_back(new RangeParam<int>("Max. Chrom. Blue", Param::RANGE, &maxCb, 0, 256, 2));
}

void YCbCrFilter::setParams(int mny, int mxy, int mnr, int mxr, int mnb, int mxb, int dcr)
{
    minY = mny;
    maxY = mxy;
    minCr = mnr;
    maxCr = mxr;
    minCb = mnb;
    maxCb = mxb;
    dstCr = dcr;
}

//3CH input (image), 3CH output (cr or cb), 1CH mask (binary)
//3CH input (image), 1CH output (cb or cb), 1CH mask (binary)
//3CH input (image), nothing, 1CH mask (binary)

void YCbCrFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, cv::Mat& dstMsk)
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


//void YCbCrFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Rect& srcRoi, cv::Rect& dstRoi)


void YCbCrFilter::_filter(const cv::Mat& src)
{
    // Alias
    cv::Size size = src.size();

    // Do colour conversion
    cvtColor(src, yccImg, CV_BGR2YCrCb);

    // Populate individual channels
    yChannel = Mat(size, CV_8UC1);
    crChannel = Mat(size, CV_8UC1);
    cbChannel = Mat(size, CV_8UC1);
    Mat yccChannels[] = {yChannel, crChannel, cbChannel};

    // Extract YCC channels
    split(yccImg, yccChannels);

    // Apply thresholds
    inRange(yccImg,
            Scalar(minY, minCr, minCb),
            Scalar(maxY, maxCr, maxCb),
            maskImg);
}

void YCbCrFilter::_store(cv::Mat& dstImg, cv::Mat& dstMsk)
{
    // Store the conversion result
    if(dstImg.data)
    {
        if(dstImg.type() == CV_8UC1)
        {
            if(dstCr)
                crChannel.copyTo(dstImg);
            else
                cbChannel.copyTo(dstImg);
        }
        else
        {
            _visualise3ch();
            if(dstCr)
                visCrImg.copyTo(dstImg);
            else
                visCbImg.copyTo(dstImg);
        }
    }

    // Store thresholding result
    if(dstMsk.data)
        dstMsk = maskImg;
}

void YCbCrFilter::_visualise()
{
    // Visualise hue on request
    if(visCr || visCb)
        _visualise3ch();

    // Visualise mask on request
    if(visMask)
        cvtColor(maskImg, visMaskImg, CV_GRAY2BGR);
}

void YCbCrFilter::_visualise3ch()
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
