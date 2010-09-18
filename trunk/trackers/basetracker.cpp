#include "basetracker.h"

BaseDetector* BaseTracker::nullDetector = new BaseDetector("Null Detector");

BaseTracker::BaseTracker(Store* st, const std::string& s) :
    store(st),
    name(s),
    enabled(false),
    currDetector(nullDetector)
{
}

std::vector<BaseDetector*> BaseTracker::getDetectors()
{
    return detectors;
}

void BaseTracker::enable()
{
    enabled = true;
}

void BaseTracker::disable()
{
    enabled = false;
}

bool BaseTracker::isEnabled()
{
    return enabled;
}

bool* BaseTracker::getEnabled()
{
    return &enabled;
}

const std::string BaseTracker::getName()
{
    return name;
}
