#ifndef EYESHAARX2TRACKER_H
#define EYESHAARX2TRACKER_H

#include "basetracker.h"

class EyesHaarX2Tracker : public BaseTracker
{
public:
    EyesHaarX2Tracker(Store* st);

    void track();

private:
    BaseDetector* haarDetector;
};

#endif // EYESHAARX2TRACKER_H
