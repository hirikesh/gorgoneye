#include <cv.h>
#include "parameter.h"
#include "equalisefilter.h"


using namespace cv;

EqualiseFilter::EqualiseFilter(Store* st) :
        BaseFilter(st, "Equalisation")
{
    _params.push_back(new ModeParam("Apply Alternative Equalisation (HSV)", &useHSV, false));
}

void EqualiseFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, cv::Mat& dstMsk)
{
    if (!enabled) return;

    _filter(srcImg);

    _store(dstImg, dstMsk);
}

void EqualiseFilter::_filter(const cv::Mat& srcImg)
{

    // Alias
    Size srcImgSize = srcImg.size();

    if (useHSV)
    {
         // Prepare images to process
        Mat hsvImg = Mat(srcImgSize, CV_8UC3);

        // Do colour conversion
        cvtColor(srcImg, hsvImg, CV_BGR2HSV);

        // Populate individual channels
        Mat hueChannel = Mat(srcImgSize, CV_8UC1);
        Mat satChannel = Mat(srcImgSize, CV_8UC1);
        Mat valChannel = Mat(srcImgSize, CV_8UC1);
        Mat hsvChannels[] = {hueChannel, satChannel, valChannel};
        // Extract HSV channels
        split(hsvImg, hsvChannels);

        // Histogram Equalisation performed on value channel
        equalizeHist(valChannel, valChannel);
        merge(hsvChannels, 3, hsvImg);

        cvtColor(hsvImg, equalisedImg, CV_HSV2BGR);
    }
    else
    {
        // Prepare images to process
        Mat yccImg = Mat(srcImgSize, CV_8UC3);

        // Do colour conversion
        cvtColor(srcImg, yccImg, CV_BGR2YCrCb);

        // Populate individual channels
        Mat lumaChannel = Mat(srcImgSize, CV_8UC1);
        Mat crChannel = Mat(srcImgSize, CV_8UC1);
        Mat cbChannel = Mat(srcImgSize, CV_8UC1);
        Mat yccChannels[] = {lumaChannel, crChannel, cbChannel};

        // Extract YCC channels
        split(yccImg, yccChannels);

        // Histogram Equalisation performed on luminance channel
        equalizeHist(lumaChannel, lumaChannel);
        merge(yccChannels, 3, yccImg);

        cvtColor(yccImg, equalisedImg, CV_YCrCb2BGR);
    }
}

void EqualiseFilter::_store(cv::Mat &dstImg, cv::Mat &dstMsk)
{
    if (dstImg.data)
        equalisedImg.copyTo(dstImg);;
}

void EqualiseFilter::_visualise()
{

    /* Do Nothing - No Visualisation for this. */
}
