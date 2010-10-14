#ifndef FACEHAARCAMSHIFTTRACKER_H
#define FACEHAARCAMSHIFTTRACKER_H

#include "basetracker.h"

#define MIN_HUE 160
#define MAX_HUE 20
#define MIN_SAT 32
#define MAX_SAT 256
#define MIN_VAL 32
#define MAX_VAL 224

#define MIN_LUM 0
#define MAX_LUM 256
#define MIN_CR 133
#define MAX_CR 174
#define MIN_CB 77
#define MAX_CB 128

#define ERODE_DILATE_DEPTH 2

class FaceHaarCAMShiftTracker : public BaseTracker
{
public:
    FaceHaarCAMShiftTracker(Store* st);

    void track();

private:
    BaseDetector* haarDetector;
    BaseFilter* ycbcrFilter;
    BaseFilter* hsvFilter;
    BaseFilter* erodeDilateFilter;
    BaseDetector* camShiftDetector;
};

#endif // FACEHAARCAMSHIFTTRACKER_H
