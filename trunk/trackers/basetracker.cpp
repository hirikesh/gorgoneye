#include "basetracker.h"

BaseDetector* BaseTracker::nullDetector = new BaseDetector("Null Detector");

BaseTracker::BaseTracker(Store* st, const string& s) :
    store(st),
    name(s),
    enabled(false),
    currDetector(nullDetector)
{
}

void BaseTracker::track()
{
    currDetector->locate(store->sceneImg, store->faceRoi);
    // add other stuff here
    // to qualify this function
    // as "tracking". ie. state
    // variables showing the
    // result of locate() etc.
}

vector<BaseDetector*> BaseTracker::getDetectors()
{
    return detectors;
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
