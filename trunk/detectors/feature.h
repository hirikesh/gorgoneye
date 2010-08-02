/* Feature:
   Concreate Detector class using
   CAMshift tracking. */

#include "detector.h"

#ifndef FEATURE_H
#define FEATURE_H

class Feature : public Detector
{
public:
//    Feature();

    void locate();

private:
    // configuration parameters
    // as an object should go here.
};

#endif // FEATURE_H
