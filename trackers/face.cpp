#include "store.h"
#include "detector.h"
#include "detectors/haar.h"
#include "detectors/feature.h"
#include "face.h"

Face::Face(Store* st) : Tracker(st)
{
}


void Face::track()
{
    detector->locate();

    // Now add other stuff here
    // to fulfill the function of
    // "tracking". ie. state vars
    // representing the result of
    // locate(), post processing,
    // callbacks, etc.
}


void Face::setDetector(int type)
{
    switch(type)
    {
    case HAAR:
        detector = new Haar(store);
    case FEAT:
        detector = new Feature(store);
    default:
        detector = new Detector(store);
    }
}
