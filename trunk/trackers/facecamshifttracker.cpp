#include <cv.h>
#include <QDebug>
#include "facecamshifttracker.h"
#include "filters/ycbcrfilter.h"
#include "filters/hsvfilter.h"
#include "filters/erodedilatefilter.h"
#include "detectors/camshiftdetector.h"
#include "store.h"

class Mat;

FaceCAMShiftTracker::FaceCAMShiftTracker(Store* st) : BaseTracker(st, "CAMShift Face")
{
    ycbcrFilter = new YCbCrFilter("YCbCr Filter", st, 0, 255, 133, 173, 77, 127);
    ycbcrFilter->enable();
    filters.push_back(ycbcrFilter);
    hsvFilter = new HSVFilter("HSV Filter", st, 0, 32, 40, 128, 24, 80);
    hsvFilter->enable();
    filters.push_back((BaseFilter*)hsvFilter);
    erodeDilateFilter = new ErodeDilateFilter("Erode-Dilate Filter", st, 2);
    erodeDilateFilter->enable();
    filters.push_back(erodeDilateFilter);

    camShiftDetector = new CAMShiftDetector(st, CAMS);
    detectors.push_back(camShiftDetector);

    BaseTracker::initImageModes();
}

void FaceCAMShiftTracker::track()
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

    // Filtering
    cv::Mat ignoreImg;
    ycbcrFilter->filter(tmpSceneImg, ignoreImg, ignoreImg, tmpSceneMsk);
    hsvFilter->filter(tmpSceneImg, ignoreImg, tmpSceneMsk, tmpSceneMsk);
    erodeDilateFilter->filter(ignoreImg, ignoreImg, tmpSceneMsk, tmpSceneMsk);

//    double t = (double)cv::getTickCount();
    bool located = currDetector->locate(hsvFilter->hueChannel, tmpSceneMsk, tmpFaceRoi);
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

cv::Mat* FaceCAMShiftTracker::getDispImg()
{
    return &store->sceneImg;
}
