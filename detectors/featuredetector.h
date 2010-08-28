/* FeatureDetector:
   Concreate BaseDetector class using
   CAMshift tracking. */

#include "store.h"
#include "basedetector.h"

#ifndef FEATURE_H
#define FEATURE_H

class FeatureDetector : public BaseDetector
{
public:
    FeatureDetector();

    bool locate(const Mat& srcImg, Rect& srcRoi);

private:
    bool firstRun;
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
      Mat backProjImg;
      Mat hueVisImg;
      Mat backProjGrayImg;
//    MatND hist;
};

#endif // FEATURE_H