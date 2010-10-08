#include <cv.h>
#include "hybriddetector.h"

HybridDetector::HybridDetector(const int type, BaseDetector* slowdet, BaseDetector* fastdet) :
    BaseDetector(type, slowdet->name() + "-" + fastdet->name()),
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
