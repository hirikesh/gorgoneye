#include <cv.h>
#include "store.h"
#include "basedetector.h"

using std::string;
using std::vector;
using cv::Mat;
using cv::Rect;

BaseDetector::BaseDetector(Store* st, const int& id, const string& s) :
    _type(id),
    _name(s)
{
}

bool BaseDetector::locate(const Mat& srcImg, const Mat& srcMsk, Rect& srcRoi)
{
    srcRoi = Rect(0, 0, srcImg.cols, srcImg.rows);
    return true;
}

const int BaseDetector::type() const
{
    return _type;
}

const string BaseDetector::name() const
{
    return _name;
}

vector<Param*> BaseDetector::params()
{
    return _params;
}

bool BaseDetector::hasParams()
{
    return (_params.size() > 0);
}

std::vector<Param*>* BaseDetector::getPtrParams()
{
    return &_params;
}

std::vector<Param*> BaseDetector::getImageModes()
{
    // TODO:
    // return parameters
    return imageModes;
}
