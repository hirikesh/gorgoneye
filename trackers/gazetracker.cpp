#include <cv.h>
#include <QDebug>
#include "gazetracker.h"
#include "filters/cannyedgefilter.h"
// insert gaze detection algorithm header here!
#include "store.h"

GazeTracker::GazeTracker(Store* st) :
    BaseTracker(st, "Gaze-test")
{
    cannyEdgeFilter = new CannyEdgeFilter(st, 20, 40);
    cannyEdgeFilter->enable();
    filters.push_back(cannyEdgeFilter);

    BaseTracker::initImageModes();
}

void GazeTracker::track()
{

    if(!enabled) return;

    // Preprocessing
    cv::Mat gazeImg(1, 1, CV_8UC3), cleanGazeImg;
    cannyEdgeFilter->filter(store->eyesImg, gazeImg, store->ignore);

//    double t = (double)cv::getTickCount();
//    bool located = someDetector->locate(store->faceImg(reducedFaceRoi), store->ignore, store->eyesRoi);
//    t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
//    qDebug() << haarDetector->name().c_str() << "speed:" << 1000*t << "ms";

//    if(located) {
        // Postprocessing (none atm)
//    }
    // Updating store bool after attempting to ensures ROIs are valid
//    store->gazeLocated = located;
}
