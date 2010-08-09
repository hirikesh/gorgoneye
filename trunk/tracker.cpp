#include "store.h"
#include "tracker.h"
#include "detector.h"

Tracker::Tracker(Store* st) :
    store(st),
    detector(new Detector())
{
}

void Tracker::track()
{
    detector->locate(store->sceneImg, store->faceRoi);
    // add other stuff here
    // to qualify this function
    // as "tracking". ie. state
    // variables showing the
    // result of locate() etc.
}

void Tracker::setDetector(int type)
{
}
