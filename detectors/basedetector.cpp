#include <cv.h>
#include "store.h"
#include "basedetector.h"

using std::string;
using std::vector;
using cv::Mat;
using cv::Rect;

BaseDetector::BaseDetector(Store* st, const string& s) :
    _name(s)
{
}

void BaseDetector::train(const cv::Mat& inputs, const cv::Mat& outputs)
{
    return;
}

bool BaseDetector::locate(const cv::Mat& input, cv::Mat& output)
{
    return false;
}

bool BaseDetector::locate(const Mat& srcImg, const Mat& srcMsk, Rect& srcRoi)
{
    srcRoi = Rect(0, 0, srcImg.cols, srcImg.rows);
    return false;
}

const string BaseDetector::name() const
{
    return _name;
}

vector<Param*> BaseDetector::params() const
{
    return _params;
}

std::vector<Param*> BaseDetector::images() const
{
    return _images;
}
