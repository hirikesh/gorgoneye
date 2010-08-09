/* Detector:
   Polymorphic class to support multiple
   tracking algorithms under common interface.
   Follows the Strategy design pattern. */

#include <cv.h>
#include "parameter.h"

using namespace cv;

#ifndef DETECTOR_H
#define DETECTOR_H

class Detector
{
public:
    Detector();

    virtual bool locate(const Mat& srcImg, Rect& srcRoi);
    vector<Param>* getParams();

private:
    vector<Param> params;
};

#endif // DETECTOR_H
