#include <cv.h>
#include <QDebug>
#include "eyestracker.h"
#include "filters/cannycontourfilter.h"
#include "store.h"

EyesTracker::EyesTracker(Store* st) : BaseTracker(st, "Eyes-test")
{
    cannyContourFilter = new CannyContourFilter(st);
    cannyContourFilter->enable();
    filters.push_back(cannyContourFilter);

    BaseTracker::initImageModes();
}

void EyesTracker::track()
{
    if(!enabled) return;

    // Preprocessing

    // Filtering
    cannyContourFilter->filter(store->faceImg, store->ignore, store->ignore);

//    double t = (double)cv::getTickCount();
//    bool located = someDetector->locate(store->faceImg, store->faceMsk, store->eyesRoi);
//    t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
//    qDebug() << haarDetector->name().c_str() << "speed:" << 1000*t << "ms";

//    if(located) {
        // Post processing
//    }
    // Updating store bool after attempting to ensures ROIs are valid
//    store->eyesLocated = located;
    store->eyesLocated = false;
}
