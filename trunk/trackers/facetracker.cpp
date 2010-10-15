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
return;
#if(TIME_FACE_TRACKERS)
    double t = (double)cv::getTickCount();
#endif /* TIME_FACE_TRACKERS */
    located = someDetector->locate(store->sceneImg, store->sceneMsk, store->faceRoi);
#if(TIME_FACE_TRACKERS)
    t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
    qDebug() << someDetector->name().c_str() << "speed:" << 1000*t << "ms";
#endif /* TIME_FACE_TRACKERS */

    if(located) {
        // Postprocessing
    }
    // Updating store bool after attempting to ensures ROIs are valid
    store->faceLocated = located;
}
