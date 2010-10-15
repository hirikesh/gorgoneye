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
return;
#if(TIME_EYES_TRACKERS)
    double t = (double)cv::getTickCount();
#endif /* TIME_EYES_TRACKERS */
    located = someDetector->locate(store->faceImg, store->faceMsk, store->eyesRoi);
#if(TIME_EYES_TRACKERS)
    t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
    qDebug() << someDetector->name().c_str() << "speed:" << 1000*t << "ms";
#endif /* TIME_EYES_TRACKERS */

    if(located) {
        // Post processing
    }
    // Updating store bool after attempting to ensures ROIs are valid
    store->eyesLocated = located;
}
