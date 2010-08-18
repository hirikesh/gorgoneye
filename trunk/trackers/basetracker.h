/* BaseTracker:
   Polymorphic class to ease implementation
   of trackers tracking different features,
   but all with the same functional structure.
   Follows the Strategy design pattern. */

#include "store.h"
#include "detectors/basedetector.h"

#ifndef TRACKER_H
#define TRACKER_H

class BaseTracker
{
public:
    BaseTracker(Store* st);

    virtual void track();
    virtual void setDetector(int type) = 0;
    BaseDetector* getDetector();
    bool isEnabled();
    void setEnable(bool);
protected:
    Store* store;
    BaseDetector* detector;
    bool enabled;
};

#endif // TRACKER_H
