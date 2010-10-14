#include <cv.h>
#include <QDebug>
#include "gazetracker.h"
#include "filters/cannyedgefilter.h"
#include "filters/grayscalefilter.h"
// insert gaze detection algorithm header here!
#include "store.h"

GazeTracker::GazeTracker(Store* st) :
    BaseTracker(st, "Gaze-test")
{
    cannyEdgeFilter = new CannyEdgeFilter(st, 20, 40);
    cannyEdgeFilter->enable();
    filters.push_back(cannyEdgeFilter);

    grayscaleFilter = new GrayscaleFilter(st);
    grayscaleFilter->enable();
    filters.push_back(grayscaleFilter);

    BaseTracker::initImageModes();
}

void GazeTracker::track()
{

    if(!enabled) return;

    // Preprocessing

    // Filtering
    cv::Mat gazeImg;
    cannyEdgeFilter->filter(store->eyesImg, store->ignore, store->ignore);
    grayscaleFilter->filter(store->eyesImg, gazeImg, store->ignore);

    gazeImg.convertTo(gazeImg, CV_32FC1, 1./255);

//    double t = (double)cv::getTickCount();
//    located = someDetector->locate(store->faceImg(reducedFaceRoi), store->ignore, store->eyesRoi);
//    t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
//    qDebug() << haarDetector->name().c_str() << "speed:" << 1000*t << "ms";

//    if(located) {
        // Postprocessing (none atm)
//    }
    // Updating store bool after attempting to ensures ROIs are valid
//    store->gazeLocated = located;
}
