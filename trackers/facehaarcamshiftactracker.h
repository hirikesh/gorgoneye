#ifndef FACEHAARCAMSHIFTACTRACKER_H
#define FACEHAARCAMSHIFTACTRACKER_H

#include "basetracker.h"

#define ERODE_DILATE_DEPTH 2

class FaceHaarCAMShiftACTracker : public BaseTracker
{
public:
    FaceHaarCAMShiftACTracker(Store* st);

    void track();

private:
    BaseDetector* haarDetector;
    BaseFilter* ycbcrFilter;
    BaseFilter* hsvFilter;
    BaseFilter* erodeDilateFilter;
    BaseDetector* camShiftDetector;
};

#endif // FACEHAARCAMSHIFTACTRACKER_H
