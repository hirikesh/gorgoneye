#include "store.h"
#include "detector.h"
#include "detectors/haar.h"
#include "detectors/feature.h"
#include "face.h"

Face::Face(Store* st) : Tracker(st)
{
    #define HAAR_CC_FACE_DEFAULT "c:\\opencv2.1\\data\\haarcascades\\haarcascade_frontalface_default.xml"
    haarDetector = new Haar(HAAR_CC_FACE_DEFAULT,
                            1.1,
                            3,
                            NULL,
                            Size(96,96));
    featureDetector = new Feature();
}

void Face::track()
{
    //    cvtColor(store->sceneImg, store->sceneImg, CV_BGR2RGB);
    detector->locate(store->sceneImg, store->faceRoi);

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
        detector = haarDetector; // new Haar(store);
        break;
    case FEAT:
        detector = featureDetector; // new Feature(store);
        break;
    default:
        detector = new Detector();
        break;
    }
}