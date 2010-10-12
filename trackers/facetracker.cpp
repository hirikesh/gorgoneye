#include <cv.h>
#include <QDebug>
#include "facetracker.h"
#include "store.h"

FaceTracker::FaceTracker(Store* st) : BaseTracker(st, "Face-test")
{
    BaseTracker::initImageModes();
}

void FaceTracker::track()
{
    return;
    if(!enabled) return;

    // Preprocessing

//    double t = (double)cv::getTickCount();
    bool located = someDetector->locate(store->sceneImg, store->sceneMsk, store->faceRoi);
//    t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
//    qDebug() << hybridDetector->name().c_str() << "speed:" << 1000*t << "ms";

    if(located) {
        // Postprocessing
    }
    // Updating store bool after attempting to ensures ROIs are valid
    store->faceLocated = located;
}
