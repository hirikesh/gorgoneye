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
    hsvFilter = new HSVFilter(st, MIN_HUE, MAX_HUE, MIN_SAT, MAX_SAT, MIN_VAL, MAX_VAL);
    hsvFilter->enable();
    filters.push_back(hsvFilter);

    ycbcrFilter = new YCbCrFilter(st, MIN_LUM, MAX_LUM, MIN_CR, MAX_CR, MIN_CB, MAX_CB);
    ycbcrFilter->enable();
    filters.push_back(ycbcrFilter);

    erodeDilateFilter = new ErodeDilateFilter(st, ERODE_DILATE_DEPTH);
    erodeDilateFilter->enable();
    filters.push_back(erodeDilateFilter);

    camShiftDetector = new CAMShiftDetector(st);
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
    cv::Mat hueImg(tmpSceneImg.size(), CV_8UC1);
    cv::Mat hsvMsk(tmpSceneImg.size(), CV_8UC1);
    cv::Mat yccMsk(tmpSceneImg.size(), CV_8UC1);

    hsvFilter->filter(tmpSceneImg, hueImg, hsvMsk);
    ycbcrFilter->filter(tmpSceneImg, store->ignore, yccMsk);
    bitwise_and(hsvMsk, yccMsk, tmpSceneMsk);

    erodeDilateFilter->filter(tmpSceneMsk, tmpSceneMsk, store->ignore);

//    double t = (double)cv::getTickCount();
    located = camShiftDetector->locate(hueImg, tmpSceneMsk, tmpFaceRoi);
//    t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
//    qDebug() << camShiftDetector->name().c_str() << "speed:" << 1000*t << "ms";

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
