#include "facetracker.h"
#define HAAR_CC_FACE_DEFAULT "c:\\opencv2.1\\data\\haarcascades\\haarcascade_frontalface_default.xml"

FaceTracker::FaceTracker(Store* st) : BaseTracker(st, "Face")
{
    haarDetector = new HaarDetector(HAAR_CC_FACE_DEFAULT,
                                    1.1, 3, NULL, Size(96,96));
    featureDetector = new FeatureDetector();
    detectors.push_back(haarDetector);
    detectors.push_back(featureDetector);
}

void FaceTracker::track()
{
//    detector->locate(store->sceneImg, store->faceRoi);
    // hybrid haar/camshift with automatic fallback to haar when camshift fails
    if(currDetector->locate(store->sceneImg, store->faceRoi))
        setDetector(FEAT);
    else
        setDetector(HAAR);
}

void FaceTracker::setDetector(int type)
{
    switch(type)
    {
    case HAAR:
        currDetector = haarDetector;
        break;
    case FEAT:
        currDetector = featureDetector;
        break;
    default:
        currDetector = nullDetector;
        break;
    }
}
