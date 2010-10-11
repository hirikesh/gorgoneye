#ifndef FACECAMSHIFTTRACKER_H
#define FACECAMSHIFTTRACKER_H

#include "basetracker.h"
#include "filters/hsvfilter.h"

class FaceCAMShiftTracker : public BaseTracker
{
public:
    FaceCAMShiftTracker(Store* st);

    void track();

    enum type { CAMS };

    cv::Mat* getDispImg();

private:
    BaseFilter* ycbcrFilter;
    HSVFilter* hsvFilter; // need access to hue channel
    BaseFilter* erodeDilateFilter;
    BaseDetector* camShiftDetector;
};

#endif // FACECAMSHIFTTRACKER_H
