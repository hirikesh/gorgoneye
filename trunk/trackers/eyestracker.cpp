#include <cv.h>
#include "eyestracker.h"
#include "detectors/haardetector.h"
#include "store.h"

EyesTracker::EyesTracker(Store* st) : BaseTracker(st, "Eyes")
{
    haarDetector = new HaarDetector(HAAR, HAAR_CC_EYES, 1.1, 3, NULL, cv::Size(16,8));
    detectors.push_back(haarDetector);
}

void EyesTracker::track()
{
    if(enabled) {
        // Preprocessing
        // reduce faceImg to top left/right quadrants
        static cv::Rect quadFaceRoi;
        quadFaceRoi = cv::Rect(store->faceRoi.x + store->faceRoi.width / 2,
                               store->faceRoi.y,
                               store->faceRoi.width / 2,
                               store->faceRoi.height / 2);
        static cv::Rect tmpEyesRoi;

        if(currDetector->locate(store->sceneImg(quadFaceRoi), tmpEyesRoi)) {
//        if(currDetector->locate(store->faceImg, store->eyesRoi)) {
            // Post processing
            // Shift eyes ROI by quadROI
            store->eyesRoi = tmpEyesRoi + quadFaceRoi.tl();
            store->eyesImg = store->faceImg(store->eyesRoi);
        }
//        store->eyesRoi = quadFaceRoi - store->faceRoi.tl();
    }
}

void EyesTracker::setDetector(int type)
{
    switch(type)
    {
    case HAAR:
        currDetector = haarDetector;
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
