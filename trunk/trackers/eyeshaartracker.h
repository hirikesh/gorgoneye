#ifndef EYESHAARTRACKER_H
#define EYESHAARTRACKER_H

#include "basetracker.h"

class EyesHaarTracker : public BaseTracker
{
public:
    EyesHaarTracker(Store* st);

    void track();

private:
    BaseDetector* haarDetector;
};

#endif // EYESHAARTRACKER_H
