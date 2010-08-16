/* Tracker:
   Polymorphic class to ease implementation
   of trackers tracking different features,
   but all with the same functional structure.
   Follows the Strategy design pattern. */

#include "store.h"
#include "detector.h"

#ifndef TRACKER_H
#define TRACKER_H

class Tracker
{
public:
    Tracker(Store* st);

    virtual void track();
    virtual void setDetector(int type) = 0;
    Detector* getDetector();
protected:
    Store* store;
    Detector* detector;
};

#endif // TRACKER_H
