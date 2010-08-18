#include "facetracker.h"

FaceTracker::FaceTracker(Store* st) : BaseTracker(st, "Face")
{
    #define HAAR_CC_FACE_DEFAULT "c:\\opencv2.1\\data\\haarcascades\\haarcascade_frontalface_default.xml"
    haarDetector = new HaarDetector(HAAR_CC_FACE_DEFAULT,
                            1.1,
                            3,
                            NULL,
                            Size(96,96));
    featureDetector = new FeatureDetector();
}

void FaceTracker::track()
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

void FaceTracker::setDetector(int type)
{
    switch(type)
    {
    case HAAR:
        detector = haarDetector;
        break;
    case FEAT:
        detector = featureDetector;
        break;
    default:
        detector = new BaseDetector("");
        break;
    }
}
