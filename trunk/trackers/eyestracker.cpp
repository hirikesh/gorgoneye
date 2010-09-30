#include <cv.h>
#include "eyestracker.h"
#include "detectors/haardetector.h"
#include "store.h"

EyesTracker::EyesTracker(Store* st) : BaseTracker(st, "Eyes")
{
    haarDetector = new HaarDetector(HAAR, HAAR_CC_EYES, 1.1, 2, NULL, cv::Size(24,24));
    detectors.push_back(haarDetector);
}

void EyesTracker::track()
{
    if(enabled) {
        // reduce faceImg to top left/right quadrants
//        store->faceRoi - store->faceRoi.size()/2;
//        store->faceRoi.br()/2;
//        store->faceImg = store->sceneImg(store->faceRoi);
        if(currDetector->locate(store->faceImg, store->eyesRoi)) {
            store->eyesImg = store->faceImg(store->eyesRoi);
        }
    }
}

void EyesTracker::setDetector(int type)
{
    switch(type)
    {
    case HAAR:
        currDetector = haarDetector;
        break;
    default:
        currDetector = nullDetector;
        break;
    }
}

cv::Mat* EyesTracker::getDispImg()
{
    return &store->faceImg;
}
