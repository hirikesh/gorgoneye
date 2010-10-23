#include <cv.h>
#include <QDebug>
#include "facemtracker.h"
#include "filters/hsvfilter.h"
#include "filters/ycbcrfilter.h"
#include "filters/erodedilatefilter.h"
#include "detectors/haardetector.h"
#include "detectors/meanshiftdetector.h"
#include "store.h"


FaceMTracker::FaceMTracker(Store* st) : BaseTracker(st, "Haar-MeanShift Face")
{
    haarDetector = new HaarDetector(st, HAAR_CC_FACE, 1.2, 3, NULL, cv::Size(64,72));
    detectors.push_back(haarDetector);

    hsvFilter = new HSVFilter(st, MIN_HUE, MAX_HUE, MIN_SAT, MAX_SAT, MIN_VAL, MAX_VAL);
    hsvFilter->enable();
    filters.push_back(hsvFilter);

    ycbcrFilter = new YCbCrFilter(st, MIN_LUM, MAX_LUM, MIN_CR, MAX_CR, MIN_CB, MAX_CB);
    ycbcrFilter->enable();
    filters.push_back(ycbcrFilter);

    erodeDilateFilter = new ErodeDilateFilter(st, ERODE_DILATE_DEPTH);
    erodeDilateFilter->enable();
    filters.push_back(erodeDilateFilter);

    meanShiftDetector = new MeanShiftDetector(st);
    detectors.push_back(meanShiftDetector);

    BaseTracker::initImageModes();
}

void FaceMTracker::track()
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

    if(!store->faceLocated)
    {
#if(TIME_FACE_TRACKERS)
        double t = (double)cv::getTickCount();
#endif /* TIME_FACE_TRACKERS */
        located = haarDetector->locate(tmpSceneImg, store->ignore, tmpFaceRoi);
#if(TIME_FACE_TRACKERS)
        t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
        qDebug() << haarDetector->name().c_str() << "speed:" << 1000*t << "ms";
#endif /* TIME_FACE_TRACKERS */
    }
    else
    {
        // Filtering
        cv::Mat hueImg(tmpSceneImg.size(), CV_8UC1);
        cv::Mat hsvMsk(tmpSceneImg.size(), CV_8UC1);
        cv::Mat yccMsk(tmpSceneImg.size(), CV_8UC1);

        hsvFilter->filter(tmpSceneImg, hueImg, hsvMsk);
        ycbcrFilter->filter(tmpSceneImg, store->ignore, yccMsk);
        bitwise_and(hsvMsk, yccMsk, tmpSceneMsk);

        erodeDilateFilter->filter(tmpSceneMsk, tmpSceneMsk, store->ignore);

#if(TIME_FACE_TRACKERS)
        double t = (double)cv::getTickCount();
#endif /* TIME_FACE_TRACKERS */
        located = meanShiftDetector->locate(hueImg, tmpSceneMsk, tmpFaceRoi);
#if(TIME_FACE_TRACKERS)
        t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
        qDebug() << camShiftDetector->name().c_str() << "speed:" << 1000*t << "ms";
#endif /* TIME_FACE_TRACKERS */
    }

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
