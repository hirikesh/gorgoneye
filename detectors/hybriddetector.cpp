#include <cv.h>
#include "hybriddetector.h"

HybridDetector::HybridDetector(Store* st, const int type, BaseDetector* slowdet, BaseDetector* fastdet) :
    BaseDetector(st, type, slowdet->name() + "-" + fastdet->name()),
    slowDetector(slowdet),
    fastDetector(fastdet),
    useFastDetector(false)
{
}

bool HybridDetector::locate(const cv::Mat &srcImg, cv::Rect &srcRoi)
{
    if(useFastDetector) {
        useFastDetector = fastDetector->locate(srcImg, srcRoi);
    } else {
        useFastDetector = slowDetector->locate(srcImg, srcRoi);
    }
    return useFastDetector;
}
