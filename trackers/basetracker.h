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
    void setDetector(int type);
    int getCurrDetectorType();
    std::vector<BaseDetector*> getDetectors();
    void enable();
    void disable();
    bool isEnabled();
    bool* getEnabled();
    const std::string name() const;
    virtual cv::Mat* getDispImg() = 0;

protected:
    Store* store;
    const std::string _name;
    bool enabled;

    std::vector<BaseDetector*> detectors;
    BaseDetector* currDetector;
};

#endif // TRACKER_H
