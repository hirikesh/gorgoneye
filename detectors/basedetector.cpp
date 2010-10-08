#include <cv.h>
#include "basedetector.h"

using std::string;
using std::vector;
using cv::Mat;
using cv::Rect;

BaseDetector::BaseDetector(const int& id, const string& s) :
    type(id),
    _name(s)
{
}

bool BaseDetector::locate(const Mat& srcImg, Rect& srcRoi)
{
    srcRoi = Rect(0, 0, srcImg.cols, srcImg.rows);
    return true;
}

const int BaseDetector::getType() const
{
    return type;
}

const string BaseDetector::name() const
{
    return _name;
}

vector<Param*> BaseDetector::getParams()
{
    return params;
}

bool BaseDetector::hasParams()
{
    return (params.size() > 0);
}

std::vector<Param*>* BaseDetector::getPtrParams()
{
    return &params;
}
