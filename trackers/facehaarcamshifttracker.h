#ifndef FACEHAARCAMSHIFTTRACKER_H
#define FACEHAARCAMSHIFTTRACKER_H

#include "basetracker.h"

class FaceHaarCAMShiftTracker : public BaseTracker
{
public:
    FaceHaarCAMShiftTracker(Store* st);

    void track();

private:
    BaseDetector* haarDetector;
    BaseFilter* ycbcrFilter;
    BaseFilter* hsvFilter;
    BaseFilter* erodeDilateFilter;
    BaseDetector* camShiftDetector;
};

#endif // FACEHAARCAMSHIFTTRACKER_H
