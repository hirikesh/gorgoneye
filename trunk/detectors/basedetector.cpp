#include "basedetector.h"

BaseDetector::BaseDetector()
{
}

bool BaseDetector::locate(const Mat& srcImg, Rect& srcRoi)
{
    return true;
}

vector<Param*> BaseDetector::getParams()
{
    return params;
}
