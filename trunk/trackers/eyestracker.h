#ifndef EYES_H
#define EYES_H

#include "basetracker.h"

class EyesTracker : public BaseTracker
{
public:
    EyesTracker(Store* st);

    void track();
    cv::Mat* getDispImg();

    enum type { HAAR };
    void setDetector(int type);

private:
    BaseDetector* haarDetector;
};

#endif // EYES_H
