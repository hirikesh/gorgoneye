#include <cv.h>
#include "eyestracker.h"
#include "detectors/haardetector.h"
#include "store.h"

EyesTracker::EyesTracker(Store* st) : BaseTracker(st, "Eyes")
{
    haarDetector = new HaarDetector(HAAR, HAAR_CC_EYES, 1.1, 3, NULL, cv::Size(24,16));
    detectors.push_back(haarDetector);
}

void EyesTracker::track()
{
    if(enabled) {
        // Preprocessing
        // Smooth and downsample sceneImg
        static cv::Mat tmpFaceImg;
        cv::pyrDown(store->faceImg, tmpFaceImg);
        // reduce faceImg to top left/right quadrants
        static cv::Rect quadFaceRoi;
        quadFaceRoi = cv::Rect(0, 0,
                               store->faceRoi.width / 4,
                               store->faceRoi.height / 4);
        static cv::Rect tmpEyesRoi;

        store->eyesLocated = currDetector->locate(tmpFaceImg(quadFaceRoi), tmpEyesRoi);
        if(store->eyesLocated) {
//        if(currDetector->locate(store->faceImg, store->eyesRoi)) {
            // Post processing
            // Shift eyes ROI by quadROI
            store->eyesRoi = cv::Rect(tmpEyesRoi.x * 2,
                                      tmpEyesRoi.y * 2,
                                      tmpEyesRoi.width * 2,
                                      tmpEyesRoi.height * 2);
//            store->eyesImg = store->faceImg(store->eyesRoi);
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
