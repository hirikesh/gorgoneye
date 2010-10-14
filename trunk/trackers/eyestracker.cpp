#include <cv.h>
#include <QDebug>
#include "eyestracker.h"
#include "filters/ycbcrfilter.h"
#include "store.h"

EyesTracker::EyesTracker(Store* st) : BaseTracker(st, "Eyes-test")
{
    ycbcrFilter = new YCbCrFilter(st);
    ycbcrFilter->enable();
    filters.push_back(ycbcrFilter);

    BaseTracker::initImageModes();
}

void EyesTracker::track()
{
    if(!enabled) return;

    // Preprocessing

    // Filtering
    ycbcrFilter->filter(store->faceImg, store->ignore, store->ignore);

//    double t = (double)cv::getTickCount();
//    located = someDetector->locate(store->faceImg, store->faceMsk, store->eyesRoi);
//    t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
//    qDebug() << haarDetector->name().c_str() << "speed:" << 1000*t << "ms";

//    if(located) {
        // Post processing
//    }
    // Updating store bool after attempting to ensures ROIs are valid
//    store->eyesLocated = located;
    store->eyesLocated = false;
}
