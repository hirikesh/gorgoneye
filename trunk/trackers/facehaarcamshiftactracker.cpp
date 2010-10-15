#include <cv.h>
#include <QDebug>
#include "facehaarcamshiftactracker.h"
#include "filters/hsvfilter.h"
#include "filters/ycbcrfilter.h"
#include "filters/erodedilatefilter.h"
#include "detectors/haardetector.h"
#include "detectors/camshiftdetector.h"
#include "store.h"

FaceHaarCAMShiftACTracker::FaceHaarCAMShiftACTracker(Store* st) : BaseTracker(st, "Haar-CAMShift Auto-Calib. Face")
{
    haarDetector = new HaarDetector(st, HAAR_CC_FACE, 1.2, 3, NULL, cv::Size(64,72));
    detectors.push_back(haarDetector);

    hsvFilter = new HSVFilter(st);
    hsvFilter->enable();
    filters.push_back(hsvFilter);

    ycbcrFilter = new YCbCrFilter(st);
    ycbcrFilter->enable();
    filters.push_back(ycbcrFilter);

    erodeDilateFilter = new ErodeDilateFilter(st, ERODE_DILATE_DEPTH);
    erodeDilateFilter->enable();
    filters.push_back(erodeDilateFilter);

    camShiftDetector = new CAMShiftDetector(st);
    detectors.push_back(camShiftDetector);

    BaseTracker::initImageModes();
}

void FaceHaarCAMShiftACTracker::track()
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

        // Very basic threshold detection to optimise pre-CAMShift thresholding
        // Simply masks facial region to smaller central region of the haar face,
        // ideally passing through skin-coloured regions only. Then the min. and
        // max thresholds for each channel in HSV and YCbCr are picked as the
        // thresholds for CAMShift to use.
        // TODO: threshold-detection needs to take into account noise; perhaps
        //       some sort of filtering should take place during CAMShift to try
        //       to update the values on the fly and attempt to converge to a
        //       more optimal value, rather than once-off static thresholds from
        //       a haar capture.
        if(located)
        {
            store->sceneMsk = cv::Mat(tmpSceneImg.size(), CV_8UC1, cv::Scalar(0));

            // Create rectangular mask for CAMShift calibration
            float widthPerc = 0.45;
            float heightPerc = 0.45;
            int x = tmpFaceRoi.x + tmpFaceRoi.width*(1-widthPerc)/2;
            int y = tmpFaceRoi.x + tmpFaceRoi.height*(1-heightPerc)/2;
            int w = widthPerc*tmpFaceRoi.width;
            int h = heightPerc*tmpFaceRoi.height;
            cv::Rect calibRoi(x, y, w, h);
            cv::rectangle(store->sceneMsk, calibRoi, cv::Scalar(255), -1);
            cv::Mat calibImg = tmpSceneImg(calibRoi);

            // Elliptic mask disabled due to problems finding min/max values
//            cv::Point center(tmpFaceRoi.width/2, tmpFaceRoi.height/2);
//            cv::ellipse(store->sceneMsk, tmpFaceRoi.tl() + center,
//                        cv::Size(0.3*tmpFaceRoi.width/2, 0.4*tmpFaceRoi.height/2),
//                        0, 0, 360, cv::Scalar(255), -1);

            // Produce individual channels for threshold-detection
            cv::GaussianBlur(calibImg, calibImg, cv::Size(3,3), 0); // reduce effect of noise
            hsvFilter->filter(calibImg, store->ignore, store->ignore);
            ycbcrFilter->filter(calibImg, store->ignore, store->ignore);

            // Shift hue values up by 180 degress for correct skin hue range detection
            cv::Mat myHueChannel = 255*((HSVFilter*)hsvFilter)->hueChannel/180; // scale to range of 8bit uchar
            cv::bitwise_xor(myHueChannel, cv::Scalar(127), myHueChannel); // modulo 2 arithmetic
            myHueChannel = 180*myHueChannel/255; // skin hue is now 180degs higher

            // Locate min. and max. channel values.
            double minhue, maxhue, minsat, maxsat, minval, maxval;
            cv::minMaxLoc(myHueChannel, &minhue, &maxhue);
            cv::minMaxLoc(((HSVFilter*)hsvFilter)->satChannel, &minsat, &maxsat);
            cv::minMaxLoc(((HSVFilter*)hsvFilter)->valChannel, &minval, &maxval);
            double miny, maxy, mincr, maxcr, mincb, maxcb;
            cv::minMaxLoc(((YCbCrFilter*)ycbcrFilter)->yChannel, &miny, &maxy);
            cv::minMaxLoc(((YCbCrFilter*)ycbcrFilter)->crChannel, &mincr, &maxcr);
            cv::minMaxLoc(((YCbCrFilter*)ycbcrFilter)->cbChannel, &mincb, &maxcb);

            minhue = minhue - 90 > 0 ? minhue - 90 : minhue + 90;
            maxhue = maxhue - 90 > 0 ? maxhue - 90 : maxhue + 90;
            qDebug() << "Auto-calibrating...";
            qDebug() << "Hue:" << minhue << maxhue;
            qDebug() << "Sat:" << minsat << maxsat;
            qDebug() << "Val:" << minval << maxval;
            qDebug() << "Lum:" << miny   << maxy;
            qDebug() << "C.r:" << mincr  << maxcr;
            qDebug() << "C.b:" << mincb  << maxcb;
            qDebug() << "____________";

            // Set min. and max. channel thresholds for CAMShift's next run
            ((HSVFilter*)hsvFilter)->setParams(maxhue, minhue+1, minsat, maxsat+1, minval, maxval+1);
            ((YCbCrFilter*)ycbcrFilter)->setParams(miny, maxy+1, mincr, maxcr+1, mincb, maxcb+1);
        }
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

        // Check for clibration mask
        if(store->sceneMsk.data)
        {
            bitwise_and(tmpSceneMsk, store->sceneMsk, tmpSceneMsk);
            store->sceneMsk = cv::Mat();
        }

        erodeDilateFilter->filter(tmpSceneMsk, tmpSceneMsk, store->ignore);

#if(TIME_FACE_TRACKERS)
        double t = (double)cv::getTickCount();
#endif /* TIME_FACE_TRACKERS */
        located = camShiftDetector->locate(hueImg, tmpSceneMsk, tmpFaceRoi);
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
