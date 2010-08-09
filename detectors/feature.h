/* Feature:
   Concreate Detector class using
   CAMshift tracking. */

#include "store.h"
#include "detector.h"

#ifndef FEATURE_H
#define FEATURE_H

class Feature : public Detector
{
public:
    Feature();

    bool locate(const Mat& srcImg, Rect& srcRoi);

private:
    // configuration parameters
};

#endif // FEATURE_H
