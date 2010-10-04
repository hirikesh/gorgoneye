#include <cv.h>
#include "eyestracker.h"
#include "detectors/haardetector.h"
#include "detectors/featuredetector.h"
#include "detectors/testdetector.h"
#include "store.h"

EyesTracker::EyesTracker(Store* st) : BaseTracker(st, "Eyes")
{
    haarDetector = new HaarDetector(HAAR, HAAR_CC_EYES, 1.1, 3, NULL, cv::Size(24,16));
    featureDetector = new FeatureDetector(FEAT, 0, 180, 0, 255, 0, 255, 0, 255, 0, 255);
    testDetector = new TestDetector(TEST);
    detectors.push_back(haarDetector);
    detectors.push_back(featureDetector);
    detectors.push_back(testDetector);
}

void EyesTracker::track()
{
    if(enabled) {
        // Preprocessing
        // Smooth and downsample sceneImg
        cv::Mat tmpFaceImg;
        cv::pyrDown(store->faceImg, tmpFaceImg);
        // reduce faceImg to top left/right quadrants
        cv::Rect reducedFaceRoi;
        reducedFaceRoi = cv::Rect(0, 0,
                               store->faceRoi.width / 4, // div by 4 because pyrDown
                               store->faceRoi.height / 4); // downsampled by 2
        cv::Rect tmpEyesRoi;
        tmpEyesRoi = cv::Rect(store->eyesRoi.x / 2,
                              store->eyesRoi.y / 2,
                              store->eyesRoi.width / 2,
                              store->eyesRoi.height / 2);

        bool located = currDetector->locate(tmpFaceImg(reducedFaceRoi), tmpEyesRoi);
        if(located) {
            // Post processing
            // Shift eyes ROI by quadROI
            store->eyesRoi = cv::Rect(tmpEyesRoi.x * 2,
                                      tmpEyesRoi.y * 2,
                                      tmpEyesRoi.width * 2,
                                      tmpEyesRoi.height * 2);
            store->eyesLocated = located;
            store->eyesImg = store->faceImg(store->eyesRoi).clone();
        }
    }
}

void EyesTracker::setDetector(int type)
{
    switch(type)
    {
    case HAAR:
        currDetector = haarDetector;
        break;
    case FEAT:
        currDetector = featureDetector;
        break;
    case TEST:
        currDetector = testDetector;
        break;
    default:
        currDetector = nullDetector;
        break;
    }
}

cv::Mat* EyesTracker::getDispImg()
{
    return &store->faceImg;
}
