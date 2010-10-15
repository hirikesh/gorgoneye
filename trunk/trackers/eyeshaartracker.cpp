#include <cv.h>
#include <QDebug>
#include "eyeshaartracker.h"
#include "detectors/haardetector.h"
#include "store.h"

EyesHaarTracker::EyesHaarTracker(Store* st) : BaseTracker(st, "Haar Eyes")
{
    haarDetector = new HaarDetector(st, HAAR_CC_EYES, 1.2, 3, NULL, cv::Size(30,20));
    detectors.push_back(haarDetector);

    BaseTracker::initImageModes();
}

void EyesHaarTracker::track()
{
    if(!enabled) return;

    // Preprocessing
    // Reduce tracking to top-left quadrant
    cv::Rect reducedFaceRoi;
    reducedFaceRoi = cv::Rect(0, 0,
                              store->faceRoi.width / 2,
                              store->faceRoi.height / 2);

#if(TIME_EYES_TRACKERS)
    double t = (double)cv::getTickCount();
#endif /* TIME_EYES_TRACKERS */
    located = haarDetector->locate(store->faceImg(reducedFaceRoi), store->ignore, store->eyesRoi);
#if(TIME_EYES_TRACKERS)
    t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
    qDebug() << haarDetector->name().c_str() << "speed:" << 1000*t << "ms";
#endif /* TIME_EYES_TRACKERS */

    if(located) {
        // Postprocessing (none atm)
    }
    // Updating store bool after attempting to ensures ROIs are valid
    store->eyesLocated = located;
}
