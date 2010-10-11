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

//void BaseTracker::setDetector(int type)
//{
//    currDetector = detectors[type];
//}

//int BaseTracker::getCurrDetectorType()
//{
//    return currDetector->type();
//}

std::vector<BaseFilter*> BaseTracker::getFilters()
{
    return filters;
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

//bool* BaseTracker::getEnabled()
//{
//    return &enabled;
//}

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
    imageModes.push_back(new ImageModeParam("Scene Image", &store->sceneImg, &store->dispImg));

    for (unsigned int i = 0; i < filters.size(); i++)
    {
        std::vector<Param*> vec = filters[i]->images();
        imageModes.insert(imageModes.end(), vec.begin(), vec.end());
    }

    for (unsigned int i = 0; i < detectors.size(); i++)
    {
        std::vector<Param*> vec = detectors[i]->images();
        imageModes.insert(imageModes.end(), vec.begin(), vec.end());
    }
}
