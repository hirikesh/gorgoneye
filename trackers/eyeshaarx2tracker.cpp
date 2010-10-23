#include <cv.h>
#include <QDebug>
#include "eyeshaarx2tracker.h"
#include "detectors/haardetector.h"
#include "store.h"

EyesHaarX2Tracker::EyesHaarX2Tracker(Store* st) : BaseTracker(st, "Haar x2 Eyes")
{
    haarDetector = new HaarDetector(st, HAAR_CC_EYESL, 1.2, 3, NULL, cv::Size(30,20));
    detectors.push_back(haarDetector);

    BaseTracker::initImageModes();
}

void EyesHaarX2Tracker::track()
{
    if(!enabled) return;

    // Preprocessing
    // Reduce tracking to top-left quadrant
    cv::Rect reducedFaceRoi;
    reducedFaceRoi = cv::Rect(store->faceRoi.width/2, 0,
                              store->faceRoi.width / 2,
                              store->faceRoi.height / 2);

#if(TIME_EYES_TRACKERS)
    double t = (double)cv::getTickCount();
#endif /* TIME_EYES_TRACKERS */
    located = haarDetector->locate(store->faceImg(reducedFaceRoi), store->ignore, store->eyesRoiL);
#if(TIME_EYES_TRACKERS)
    t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
    qDebug() << haarDetector->name().c_str() << "speed:" << 1000*t << "ms";
#endif /* TIME_EYES_TRACKERS */

    if(located) {
        // Postprocessing (none atm)
        store->eyesRoiL = cv::Rect(store->eyesRoiL.x + store->faceRoi.width/2,
                                   store->eyesRoiL.y,
                                   store->eyesRoiL.width,
                                   store->eyesRoiL.height);
    }
    // Updating store bool after attempting to ensures ROIs are valid
    store->eyesLocatedL = located;
}
