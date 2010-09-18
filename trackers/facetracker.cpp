#include <cv.h>
#include "facetracker.h"
#include "detectors/haardetector.h"
#include "detectors/featuredetector.h"
#include "detectors/hybriddetector.h"
#include "store.h"

FaceTracker::FaceTracker(Store* st) : BaseTracker(st, "Face")
{
    haarDetector = new HaarDetector(HAAR_CC_FACE_DEFAULT, 1.1, 3, NULL, cv::Size(96,96));
    featureDetector = new FeatureDetector(40, 240, 50, 255);
    hybridDetector = new HybridDetector(haarDetector, featureDetector);
    detectors.push_back(haarDetector);
    detectors.push_back(featureDetector);
    detectors.push_back(hybridDetector);
}

void FaceTracker::track()
{
    if(enabled) currDetector->locate(store->sceneImg, store->faceRoi);
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
    case HYBR:
        currDetector = hybridDetector;
        break;
    default:
        currDetector = nullDetector;
        break;
    }
}
