#include <cv.h>
#include "facetracker.h"
#include "detectors/haardetector.h"
#include "detectors/featuredetector.h"
#include "detectors/hybriddetector.h"
#include "store.h"

FaceTracker::FaceTracker(Store* st) : BaseTracker(st, "Face")
{
    haarDetector = new HaarDetector(HAAR, HAAR_CC_FACE, 1.2, 2, NULL, cv::Size(64,72));
    featureDetector = new FeatureDetector(FEAT, 96, 180, 0, 96);
    hybridDetector = new HybridDetector(HYBR, haarDetector, featureDetector);
    detectors.push_back(haarDetector);
    detectors.push_back(featureDetector);
    detectors.push_back(hybridDetector);
}

void FaceTracker::track()
{
    if(enabled) {
        if(currDetector->locate(store->sceneImg, store->faceRoi)) {
            store->faceImg = store->sceneImg(store->faceRoi);
        }
    }
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

cv::Mat* FaceTracker::getDispImg()
{
    return &store->sceneImg;
}
