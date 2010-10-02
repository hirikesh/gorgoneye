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
    FeatureDetector(const int type, int mins, int maxs, int minv, int maxv,
                                    int mincb, int maxcb, int mincr, int maxcr);

    bool locate(const cv::Mat& srcImg, cv::Rect& srcRoi);

private:
    bool histCalibrate;

    int minSaturation;
    int maxSaturation;
    int minValue;
    int maxValue;

    int minChromaBlue;
    int maxChromaBlue;
    int minChromaRed;
    int maxChromaRed;

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
    cv::Mat backProjImg2;
    cv::Mat hueVisImg;
    cv::Mat chromaBlueVisImg;
    cv::Mat chromaRedVisImg;
    cv::Mat backProjGrayImg;
//    MatND hist;
};

#endif // FEATURE_H
