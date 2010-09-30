#include "basetracker.h"

BaseDetector* BaseTracker::nullDetector = new BaseDetector(-1, "Null Detector");

BaseTracker::BaseTracker(Store* st, const std::string& s) :
    store(st),
    name(s),
    enabled(false),
    currDetector(nullDetector)
{
}

int BaseTracker::getCurrDetectorType()
{
    return currDetector->getType();
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
