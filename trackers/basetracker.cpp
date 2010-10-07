#include "basetracker.h"

BaseTracker::BaseTracker(Store* st, const std::string& s) :
    store(st),
    name(s),
    enabled(false)
{
}

void BaseTracker::setDetector(int type)
{
    currDetector = detectors[type];
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

bool* BaseTracker::getEnabled()
{
    return &enabled;
}

const std::string BaseTracker::getName() const
{
    return name;
}
