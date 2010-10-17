#ifndef FACECAMSHIFTTRACKER_H
#define FACECAMSHIFTTRACKER_H

#include "basetracker.h"

class FaceCAMShiftTracker : public BaseTracker
{
public:
    FaceCAMShiftTracker(Store* st);

    void track();

private:
    BaseFilter* ycbcrFilter;
    BaseFilter* hsvFilter;
    BaseFilter* erodeDilateFilter;
    BaseDetector* camShiftDetector;
};

#endif // FACECAMSHIFTTRACKER_H
