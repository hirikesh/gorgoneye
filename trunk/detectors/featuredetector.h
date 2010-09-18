/* FeatureDetector:
   Concrete BaseDetector class using
   CAMshift tracking. */

#ifndef FEATURE_H
#define FEATURE_H

#include <cv.h>
#include "basedetector.h"

class FeatureDetector : public BaseDetector
{
public:
    FeatureDetector(int mins, int maxs, int minv, int maxv);

    bool locate(const cv::Mat& srcImg, cv::Rect& srcRoi);

private:
    bool histCalibrate;

    int minSaturation;
    int maxSaturation;
    int minValue;
    int maxValue;

    // should only keep configuration vars and
    // detector flags, etc here. vars that need
    // to be exposed to external entities, in other
    // words.
    // persistent vars that are valid only for
    // single function calls should be declared
    // within the scope of the called function.
    // declare them statically where possible.

//    Mat cHSVImg;
//    Mat hueImg;
//    Mat satImg;
//    Mat valImg;
//    Mat maskImg;
    cv::Mat backProjImg;
    cv::Mat hueVisImg;
    cv::Mat backProjGrayImg;
//    MatND hist;
};

#endif // FEATURE_H
