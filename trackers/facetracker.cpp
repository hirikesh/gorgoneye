#include <cv.h>
#include "facetracker.h"
#include "detectors/haardetector.h"
#include "detectors/featuredetector.h"
#include "detectors/hybriddetector.h"
#include "store.h"

FaceTracker::FaceTracker(Store* st) : BaseTracker(st, "Face")
{
    haarDetector = new HaarDetector(HAAR, HAAR_CC_FACE, 1.2, 2, NULL, cv::Size(64,72));
    featureDetector = new FeatureDetector(FEAT, 96, 172, 24, 196, 77, 127, 133, 173);
    hybridDetector = new HybridDetector(HYBR, haarDetector, featureDetector);
    detectors.push_back(haarDetector);
    detectors.push_back(featureDetector);
    detectors.push_back(hybridDetector);
}

void FaceTracker::track()
{
    if(enabled) {
        // Preprocessing
        // Smooth and downsample sceneImg
        static cv::Mat tmpSceneImg;
        cv::pyrDown(store->sceneImg, tmpSceneImg);
        static cv::Rect tmpFaceRoi;
        tmpFaceRoi = cv::Rect(store->faceRoi.x / 2,
                              store->faceRoi.y / 2,
                              store->faceRoi.width / 2,
                              store->faceRoi.height / 2);

        store->faceLocated = currDetector->locate(tmpSceneImg, tmpFaceRoi);
        if(store->faceLocated) {
            // Postprocessing
            // Upsample tracked ROI
            store->faceRoi = cv::Rect(tmpFaceRoi.x * 2,
                                      tmpFaceRoi.y * 2,
                                      tmpFaceRoi.width * 2,
                                      tmpFaceRoi.height * 2);
            store->faceImg = store->sceneImg(store->faceRoi);
        }
    }
}

void FaceTracker::setDetector(int type)
{
    switch(type)
    {
    case HAAR:
        currDetector = haarDetector;
        break;
    case FEAT:
        currDetector = featureDetector;
        break;
    case HYBR:
        currDetector = hybridDetector;
        break;
    default:
        currDetector = nullDetector;
        break;
    }
}

cv::Mat* FaceTracker::getDispImg()
{
    return &store->sceneImg;
}
