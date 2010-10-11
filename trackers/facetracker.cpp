#include <cv.h>
#include <QDebug>
#include "facetracker.h"
#include "detectors/haardetector.h"
#include "detectors/featuredetector.h"
#include "detectors/hybriddetector.h"
#include "store.h"

FaceTracker::FaceTracker(Store* st) : BaseTracker(st, "Face")
{
    haarDetector = new HaarDetector(st, HAAR, HAAR_CC_FACE, 1.2, 3, NULL, cv::Size(64,72));
//    featureDetector = new FeatureDetector(st, FEAT, 0, 24, 0, 255, 108, 255, 77, 127, 132, 173);
    featureDetector = new FeatureDetector(st, FEAT, 0, 24, 0, 255, 0, 42, 77, 127, 132, 173);
    hybridDetector = new HybridDetector(st, HYBR, haarDetector, featureDetector);
    detectors.push_back(haarDetector);
    detectors.push_back(featureDetector);
    detectors.push_back(hybridDetector);

    BaseTracker::initImageModes();
}

void FaceTracker::track()
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
    qDebug() << tmpSceneMsk.data;
    bool located = currDetector->locate(tmpSceneImg, tmpSceneMsk, tmpFaceRoi);
//    t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
//    qDebug() << currDetector->getName().c_str() << "speed:" << 1000*t << "ms";

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

cv::Mat* FaceTracker::getDispImg()
{
    return &store->sceneImg;
}
