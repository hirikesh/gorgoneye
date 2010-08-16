#include "detector.h"

Detector::Detector()
{
}

bool Detector::locate(const Mat& srcImg, Rect& srcRoi)
{
    return true;
}

vector<Param*> Detector::getParams()
{
    return params;
}
