#include "basetracker.h"

BaseTracker::BaseTracker(Store* st) :
    store(st),
    detector(new BaseDetector())
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
