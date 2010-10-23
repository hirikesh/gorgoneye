#ifndef FACEMTRACKER_H
#define FACEMTRACKER_H

#include "basetracker.h"

class FaceMTracker : public BaseTracker
{
public:
    FaceMTracker(Store *st);
    void track();

private:
    BaseDetector* haarDetector;
    BaseFilter* ycbcrFilter;
    BaseFilter* hsvFilter;
    BaseFilter* erodeDilateFilter;
    BaseDetector* meanShiftDetector;
};

#endif // FACEMTRACKER_H
