#include <cv.h>
#include "hybriddetector.h"

HybridDetector::HybridDetector(BaseDetector* slowdet, BaseDetector* fastdet) :
    BaseDetector(slowdet->getName() + " + " + fastdet->getName()),
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
