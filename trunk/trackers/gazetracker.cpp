#include <cv.h>
#include <QDebug>
#include "gazetracker.h"
#include "filters/cannyedgefilter.h"
#include "filters/grayscalefilter.h"
#include "filters/ycbcrfilter.h"
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

    ycbcrFilter = new YCbCrFilter(st);
    ycbcrFilter->enable();
    filters.push_back(ycbcrFilter);

    BaseTracker::initImageModes();
}

void GazeTracker::track()
{

    if(!enabled) return;

    // Preprocessing

    // Filtering
    cannyEdgeFilter->filter(store->eyesImg, store->ignore, store->ignore);
    grayscaleFilter->filter(store->eyesImg, store->gazeImg, store->ignore);
    ycbcrFilter->filter(store->eyesImg, store->ignore, store->ignore);

    store->gazeImg.convertTo(store->gazeImg, CV_32FC1, 1./255);
return;
#if(TIME_GAZE_TRACKERS)
        double t = (double)cv::getTickCount();
#endif /* TIME_GAZE_TRACKERS */
    located = someDetector->locate(store->eyesImg, store->eyesMsk, store->gazeRoi);
#if(TIME_GAZE_TRACKERS)
    t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
    qDebug() << someDetector->name().c_str() << "speed:" << 1000*t << "ms";
#endif /* TIME_GAZE_TRACKERS */

    if(located) {
        // Postprocessing (none atm)
    }
//     Updating store bool after attempting to ensures ROIs are valid
    store->gazeLocated = located;
}
