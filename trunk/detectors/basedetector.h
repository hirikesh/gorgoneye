/* BaseDetector:
   Polymorphic class to support multiple
   tracking algorithms under common interface.
   Follows the Strategy design pattern. */

#include <cv.h>
#include "parameter.h"

using namespace cv;

#ifndef DETECTOR_H
#define DETECTOR_H

class BaseDetector
{
public:
    BaseDetector();

    virtual bool locate(const Mat& srcImg, Rect& srcRoi);
    vector<Param*> getParams();

protected:
    vector<Param*> params;
};

#endif // DETECTOR_H
