#include <cv.h>
#include <QDebug>
#include "eyestracker.h"
#include "detectors/haardetector.h"
#include "detectors/featuredetector.h"
#include "detectors/hybriddetector.h"
#include "detectors/testdetector.h"
#include "store.h"

EyesTracker::EyesTracker(Store* st) : BaseTracker(st, "Eyes")
{
    haarDetector = new HaarDetector(st, HAAR_CC_EYES, 1.2, 1, NULL, cv::Size(32,24));
    featureDetector = new FeatureDetector(st, 0, 180, 0, 255, 0, 255, 0, 255, 0, 255);
    hybridDetector = new HybridDetector(st, haarDetector, featureDetector);
    testDetector = new TestDetector(st);
    detectors.push_back(haarDetector);
    detectors.push_back(featureDetector);
    detectors.push_back(hybridDetector);
    detectors.push_back(testDetector);

    BaseTracker::initImageModes();
}

void EyesTracker::track()
{
    if(!enabled) return;

    // Preprocessing
    // Smooth and downsample faceImg
//    cv::Mat tmpFaceImg;
//    cv::pyrDown(store->faceImg, tmpFaceImg);
        // reduce faceImg to top left/right quadrants
    cv::Rect reducedFaceRoi;
    reducedFaceRoi = cv::Rect(0, 0,
                              store->faceRoi.width / 2, // div by 4 because pyrDown
                              store->faceRoi.height / 2); // downsampled by 2
//    cv::Rect tmpEyesRoi;
//    tmpEyesRoi = cv::Rect(store->eyesRoi.x / 2,
//                          store->eyesRoi.y / 2,
//                          store->eyesRoi.width / 2,
//                          store->eyesRoi.height / 2);
//    tmpEyesRoi = store->eyesRoi;

//    double t = (double)cv::getTickCount();
    bool located = haarDetector->locate(store->faceImg(reducedFaceRoi), store->faceMsk(reducedFaceRoi), store->eyesRoi);
//    t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
//    qDebug() << haarDetector->name().c_str() << "speed:" << 1000*t << "ms";

    if(located) {
        testDetector->locate(store->faceImg(reducedFaceRoi), store->faceMsk(reducedFaceRoi), store->eyesRoi);

        // Post processing
        // Shift eyes ROI by quadROI
//        store->eyesRoi = cv::Rect(tmpEyesRoi.x * 2,
//                                  tmpEyesRoi.y * 2,
//                                  tmpEyesRoi.width * 2,
//                                  tmpEyesRoi.height * 2);
//        store->eyesRoi = tmpEyesRoi;
    }
    // Updating store bool after attempting to ensures ROIs are valid
    store->eyesLocated = located;
}

//cv::Mat* EyesTracker::getDispImg()
//{
//    return &store->faceImg;
//}
