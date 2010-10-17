#ifndef FACEHAARCAMSHIFTACTRACKER_H
#define FACEHAARCAMSHIFTACTRACKER_H

#include "basetracker.h"

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
