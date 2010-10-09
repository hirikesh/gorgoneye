#include "basetracker.h"

BaseTracker::BaseTracker(Store* st, const std::string& s) :
    store(st),
    _name(s),
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

bool BaseTracker::isEnabled()
{
    return enabled;
}

const std::string BaseTracker::name() const
{
    return _name;
}

std::vector<Param*> BaseTracker::getImageModes()
{
    // TODO:
    // Add Normal/Tracking Environment mode available from store
    // Loop through detectors and insert items found
    // return imagemodes;
    return imageModes;
}
