#include "cornerfilter.h"
#include "parameter.h"
#include "store.h"

CornerFilter::CornerFilter(Store* st, int mxc, double ql, double md, int bs, bool uh, double fk) :
    BaseFilter(st, "Feature Corner"),
    visCorner(false),
    maxCorners(mxc), qualityLevel(ql), minDistance(md),
    blockSize(bs), useHarris(uh), freeK(fk)
{
    _images.push_back(new ImageModeParam("Corners found", &visCorner, &visCornerImg, &st->dispImg));
    _params.push_back(new RangeParam<int>("Maximum number of corners", Param::RANGE, &maxCorners, 1, 500, 1));
    _params.push_back(new RangeParam<double>("Quality level", Param::RANGE_DBL, &qualityLevel, 0.01, 1.00, 0.01));
    _params.push_back(new RangeParam<double>("Minimum distance between 2 corners", Param::RANGE_DBL, &minDistance, 1, 100, 4));
    _params.push_back(new RangeParam<int>("Block size", Param::RANGE, &blockSize, 1, 20, 1));
    _params.push_back(new ModeParam("Use Harris Corner detection", &useHarris, uh));
    _params.push_back(new RangeParam<double>("Harris free parameter k", Param::RANGE_DBL, &freeK, 0.00, 1.00, 0.01));
}

void CornerFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, cv::Mat& dstMsk)
{
    // Stop here if disabled
    if(!enabled) return;

    // Corner detection operation
    _filter(srcImg);

    // Store result
    _store(dstImg, dstMsk);

    // Visualise on request
    _visualise();
}

void CornerFilter::setParams()
{

}

void CornerFilter::_filter(const cv::Mat& src)
{
    // Prepare input
    cv::Mat grayChannel;

    // Find Canny edges
    cvtColor(src, grayChannel, CV_BGR2GRAY);
    cv::goodFeaturesToTrack(grayChannel, corners, maxCorners, qualityLevel, minDistance, cv::Mat(), blockSize, useHarris, freeK);

    // Create matrix with channels as vector dimensions
    cornerImg = cv::Mat(corners);

    // Visualise stuff
    visCornerImg = src.clone();
}

void CornerFilter::_store(cv::Mat& dstImg, cv::Mat& dstMsk)
{
    if(dstImg.data)
    {
        for(int i = 0; i < corners.size(); i++)
            cv::circle(dstImg, corners[i], 2, cv::Scalar(0,0,255), -1);
    }
}

void CornerFilter::_visualise()
{
    if(visCorner)
    {
        for(int i = 0; i < corners.size(); i++)
            cv::circle(visCornerImg, corners[i], 2, cv::Scalar(255,0,0), -1);
    }
}
