/* BaseTracker:
   Polymorphic class to ease implementation
   of trackers tracking different features,
   but all with the same functional structure.
   Follows the Strategy design pattern. */

#ifndef TRACKER_H
#define TRACKER_H

#include "detectors/basedetector.h"
#include <vector>
#include <string>

class BaseDetector;
class Store;

class BaseTracker
{
public:
    BaseTracker(Store* st, const std::string&);

    virtual void track() = 0;
    virtual void setDetector(int type) = 0;
    std::vector<BaseDetector*> getDetectors();
    void enable();
    void disable();
    bool isEnabled();
    bool* getEnabled();
    const std::string getName();

protected:
    Store* store;
    const std::string name;
    bool enabled;

    std::vector<BaseDetector*> detectors;
    BaseDetector* currDetector;
    static BaseDetector* nullDetector;
};

#endif // TRACKER_H
