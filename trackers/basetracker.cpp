#include "basetracker.h"

BaseDetector* BaseTracker::nullDetector = new BaseDetector("Null Detector");

BaseTracker::BaseTracker(Store* st, const string& s) :
    store(st),
    name(s),
    enabled(false),
    detector(nullDetector)
{
}

void BaseTracker::track()
{
    detector->locate(store->sceneImg, store->faceRoi);
    // add other stuff here
    // to qualify this function
    // as "tracking". ie. state
    // variables showing the
    // result of locate() etc.
}

BaseDetector* BaseTracker::getDetector()
{
    return detector;
}

void BaseTracker::setEnable(bool en)
{
    enabled = en;
}

bool BaseTracker::isEnabled()
{
    return enabled;
}

bool* BaseTracker::getEnabled()
{
    return &enabled;
}

const string BaseTracker::getName()
{
    return name;
}
