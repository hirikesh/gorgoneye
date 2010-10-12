#include <cv.h>
#include <QDebug>
#include "eyestracker.h"
#include "store.h"

EyesTracker::EyesTracker(Store* st) : BaseTracker(st, "Eyes-test")
{
    BaseTracker::initImageModes();
}

void EyesTracker::track()
{
    return;
    if(!enabled) return;

    // Preprocessing

//    double t = (double)cv::getTickCount();
    bool located = someDetector->locate(store->faceImg, store->faceMsk, store->eyesRoi);
//    t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
//    qDebug() << haarDetector->name().c_str() << "speed:" << 1000*t << "ms";

    if(located) {
        // Post processing
    }
    // Updating store bool after attempting to ensures ROIs are valid
    store->eyesLocated = located;
}
