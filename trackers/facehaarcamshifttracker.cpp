#include <cv.h>
#include <QDebug>
#include "facehaarcamshifttracker.h"
#include "filters/ycbcrfilter.h"
#include "filters/hsvfilter.h"
#include "filters/erodedilatefilter.h"
#include "detectors/haardetector.h"
#include "detectors/camshiftdetector.h"
#include "store.h"

FaceHaarCAMShiftTracker::FaceHaarCAMShiftTracker(Store* st) : BaseTracker(st, "Haar-CAMShift Face")
{
    haarDetector = new HaarDetector(st, HAAR_CC_FACE, 1.2, 3, NULL, cv::Size(64,72));
    detectors.push_back(haarDetector);

    hsvFilter = new HSVFilter(st, 0, 32, 40, 128, 24, 80);
    hsvFilter->enable();
    filters.push_back(hsvFilter);

    ycbcrFilter = new YCbCrFilter(st, 0, 256, 133, 174, 77, 128);
    ycbcrFilter->enable();
    filters.push_back(ycbcrFilter);

    erodeDilateFilter = new ErodeDilateFilter(st, 2);
    erodeDilateFilter->enable();
    filters.push_back(erodeDilateFilter);

    camShiftDetector = new CAMShiftDetector(st);
    detectors.push_back(camShiftDetector);

    BaseTracker::initImageModes();
}

void FaceHaarCAMShiftTracker::track()
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

    bool located;
    if(!store->faceLocated)
    {
//        double t = (double)cv::getTickCount();
        located = haarDetector->locate(tmpSceneImg, store->ignore, tmpFaceRoi);
//        t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
//        qDebug() << haarDetector->name().c_str() << "speed:" << 1000*t << "ms";
    }
    else
    {
        // Filtering
        cv::Mat hueImg(tmpSceneImg.size(), CV_8UC1);
        hsvFilter->filter(tmpSceneImg, hueImg, tmpSceneMsk);
        ycbcrFilter->filter(tmpSceneImg, store->ignore, tmpSceneMsk);
        erodeDilateFilter->filter(tmpSceneMsk, tmpSceneMsk, store->ignore);

//        double t = (double)cv::getTickCount();
        located = camShiftDetector->locate(hueImg, tmpSceneMsk, tmpFaceRoi);
//        t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
//        qDebug() << camShiftDetector->name().c_str() << "speed:" << 1000*t << "ms";
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
