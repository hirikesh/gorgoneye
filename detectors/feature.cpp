#include "detector.h"
#include "feature.h"

Feature::Feature() : Detector()
{

}

bool Feature::locate(const Mat& srcImg, Rect& srcRoi)
{
    return false;
}
