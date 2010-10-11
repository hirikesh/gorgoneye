/* HybridDetector:
   A hybrid detector class using 2
   detectors. The first (more intensive)
   detector is assumed to give an initial
   ROI bound for the second (less intensive)
   detector to do its job properly. */

#ifndef HYBRID_H
#define HYBRID_H

#include <cv.h>
#include "basedetector.h"

class HybridDetector : public BaseDetector
{
public:
    HybridDetector(Store* st, const int type, BaseDetector* slowdet, BaseDetector* fastdet);

    bool locate(const cv::Mat &srcImg, const cv::Mat& srcMsk, cv::Rect &srcRoi);

private:
    BaseDetector* slowDetector;
    BaseDetector* fastDetector;

    bool useFastDetector;
};

#endif // HYBRID_H
