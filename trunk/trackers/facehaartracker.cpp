#include <cv.h>
#include <QDebug>
#include "facehaartracker.h"
#include "detectors/haardetector.h"
#include "store.h"

FaceHaarTracker::FaceHaarTracker(Store* st) : BaseTracker(st, "Haar Face")
{
    haarDetector = new HaarDetector(st, HAAR_CC_FACE, 1.2, 3, NULL, cv::Size(64,72));
    detectors.push_back(haarDetector);

    BaseTracker::initImageModes();
}

void FaceHaarTracker::track()
{
    if(!enabled) return;

    // Preprocessing
    // Smooth and downsample sceneImg
    cv::Mat tmpSceneImg, tmpSceneMsk;
    cv::pyrDown(store->sceneImg, tmpSceneImg);

    cv::Rect tmpFaceRoi;
    tmpFaceRoi = cv::Rect(store->faceRoi.x / 2,
                          store->faceRoi.y / 2,
                          store->faceRoi.width / 2,
                          store->faceRoi.height / 2);

//    double t = (double)cv::getTickCount();
    bool located = haarDetector->locate(tmpSceneImg, tmpSceneMsk, tmpFaceRoi);
//    t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
//    qDebug() << haarDetector->name().c_str() << "speed:" << 1000*t << "ms";

    if(located) {
        // Postprocessing
        // Upsample tracked ROI
        store->faceRoi = cv::Rect(tmpFaceRoi.x * 2,
                                  tmpFaceRoi.y * 2,
                                  tmpFaceRoi.width * 2,
                                  tmpFaceRoi.height * 2);
    }
    // Updating store bool after attempting to ensures ROIs are valid
    store->faceLocated = located;
}
