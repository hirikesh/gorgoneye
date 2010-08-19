#include <cv.h>
using namespace cv;

#include "basedetector.h"

BaseDetector::BaseDetector(const string& s) :
        name(s)
{
}

bool BaseDetector::locate(const Mat& srcImg, Rect& srcRoi)
{
    srcRoi = Rect(0, 0, srcImg.cols, srcImg.rows);
    return true;
}

const string BaseDetector::getName() const
{
    return name;
}

vector<Param*> BaseDetector::getParams()
{
    return params;
}
