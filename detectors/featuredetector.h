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
    // configuration parameters
};

#endif // FEATURE_H
