#include "basetracker.h"
#include "store.h"
#include "detectors/basedetector.h"
#include "parameter.h"

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
    return currDetector->type();
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
    return imageModes;
}

void BaseTracker::initImageModes()
{
    for (unsigned int i = 0; i < detectors.size(); i++)
    {
        BaseDetector* currDetector = detectors[i];
        std::vector<Param*> vec = currDetector->getImageModes();
        imageModes.insert(imageModes.begin(), vec.begin(), vec.end());
    }
    imageModes.insert(imageModes.begin(), new ImageModeParam("Scene Image", &store->sceneImg, &store->dispImg));}
