#include <cv.h>
#include "hybriddetector.h"

HybridDetector::HybridDetector(Store* st, BaseDetector* slowdet, BaseDetector* fastdet) :
    BaseDetector(st, slowdet->name() + "-" + fastdet->name()),
    slowDetector(slowdet),
    fastDetector(fastdet),
    useFastDetector(false)
{
}

bool HybridDetector::locate(const cv::Mat &srcImg, const cv::Mat& srcMsk, cv::Rect &srcRoi)
{
    if(useFastDetector) {
        useFastDetector = fastDetector->locate(srcImg, srcMsk, srcRoi);
    } else {
        useFastDetector = slowDetector->locate(srcImg, srcMsk, srcRoi);
    }
    return useFastDetector;
}
