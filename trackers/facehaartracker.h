#ifndef FACEHAARTRACKER_H
#define FACEHAARTRACKER_H

#include "basetracker.h"

class FaceHaarTracker : public BaseTracker
{
public:
    FaceHaarTracker(Store* st);

    void track();

    enum type { HAAR };

    cv::Mat* getDispImg();

private:
    BaseDetector* haarDetector;
};

#endif // FACEHAARTRACKER_H
