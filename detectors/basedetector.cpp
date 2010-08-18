#include "basedetector.h"

BaseDetector::BaseDetector(const string& s) :
        name(s)
{
}

bool BaseDetector::locate(const Mat& srcImg, Rect& srcRoi)
{
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
