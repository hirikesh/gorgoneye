#ifndef FACEHAARCAMSHIFTTRACKER_H
#define FACEHAARCAMSHIFTTRACKER_H

#include "basetracker.h"
#include "filters/hsvfilter.h"

class FaceHaarCAMShiftTracker : public BaseTracker
{
public:
    FaceHaarCAMShiftTracker(Store* st);

    void track();

private:
    BaseDetector* haarDetector;
    BaseFilter* ycbcrFilter;
    HSVFilter* hsvFilter; // need access to hue channel
    BaseFilter* erodeDilateFilter;
    BaseDetector* camShiftDetector;
};

#endif // FACEHAARCAMSHIFTTRACKER_H
