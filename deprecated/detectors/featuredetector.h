/* FeatureDetector:
   Concrete BaseDetector class using
   CAMshift tracking. */

#ifndef FEATURE_H
#define FEATURE_H

#include <cv.h>
#include "basedetector.h"

class FeatureDetector : public BaseDetector
{
public:
    FeatureDetector(Store *st,
                    int minh, int maxh,
                    int mins, int maxs,
                    int minv, int maxv,
                    int mincb, int maxcb,
                    int mincr, int maxcr);

    bool locate(const cv::Mat& srcImg, const cv::Mat& srcMsk, cv::Rect& srcRoi);

private:
    bool histCalibrate;
    int minHue;
    int maxHue;
    int minSaturation;
    int maxSaturation;
    int minValue;
    int maxValue;

    int minChromaBlue;
    int maxChromaBlue;
    int minChromaRed;
    int maxChromaRed;

    cv::MatND hist;
    cv::Mat backProjImg;

    bool enBackProjImg;
    bool enHueVisImg;
    bool enChromaRedVisImg;
    bool enChromaBlueVisImg;

    cv::Mat backProjVisImg;
    cv::Mat hueVisImg;
    cv::Mat chromaRedVisImg;
    cv::Mat chromaBlueVisImg;
};

#endif // FEATURE_H
