#include <cv.h>
#include <QDebug>
#include "facetracker.h"
#include "filters/cannycontourfilter.h"
#include "store.h"

FaceTracker::FaceTracker(Store* st) : BaseTracker(st, "Face-test")
{
    cannyContourFilter = new CannyContourFilter(st);
    cannyContourFilter->enable();
    filters.push_back(cannyContourFilter);

    BaseTracker::initImageModes();
}

void FaceTracker::track()
{
    if(!enabled) return;

    // Preprocessing

    // Filtering
    cannyContourFilter->filter(store->sceneImg(store->faceRoi), store->ignore, store->ignore);

//    double t = (double)cv::getTickCount();
//    located = someDetector->locate(store->sceneImg, store->sceneMsk, store->faceRoi);
//    t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
//    qDebug() << hybridDetector->name().c_str() << "speed:" << 1000*t << "ms";

//    if(located) {
        // Postprocessing
//    }
    // Updating store bool after attempting to ensures ROIs are valid
//    store->faceLocated = located;
}
