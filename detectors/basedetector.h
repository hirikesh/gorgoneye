/* BaseDetector:
   Polymorphic class to support multiple
   tracking algorithms under common interface.
   Follows the Strategy design pattern. */

#include <cv.h>
#include "parameter.h"
#include "store.h"
using namespace cv;

#ifndef DETECTOR_H
#define DETECTOR_H

class BaseDetector
{
public:
    BaseDetector(const string&);

    virtual bool locate(const Mat& srcImg, Rect& srcRoi);
    vector<Param*> getParams();
    const string getName() const;

protected:
    const string name;
    vector<Param*> params;
};

#endif // DETECTOR_H
