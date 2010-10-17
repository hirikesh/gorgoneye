/* BaseTracker:
   Polymorphic class to ease implementation
   of trackers tracking different features,
   but all with the same functional structure.
   Follows the Strategy design pattern. */

#ifndef TRACKER_H
#define TRACKER_H

#include "filters/basefilter.h"
#include "detectors/basedetector.h"
#include "config.h"
#include <vector>
#include <string>

class BaseDetector;
class Store;
class Param;

class BaseTracker
{
public:
    BaseTracker(Store* st, const std::string&);

    virtual void track() = 0;
    std::vector<BaseFilter*> getFilters();
    std::vector<BaseDetector*> getDetectors();
    void enable();
    void disable();
    bool isEnabled();
    const std::string name() const;
    std::vector<Param*> getImageModes();

protected:
    void initImageModes();

    Store* store;
    const std::string _name;
    bool enabled;
    bool located;

    std::vector<BaseFilter*> filters;
    std::vector<BaseDetector*> detectors;
    std::vector<Param*> imageModes;
};

#endif // TRACKER_H
