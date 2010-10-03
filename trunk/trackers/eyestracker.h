#ifndef EYES_H
#define EYES_H

#include "basetracker.h"

class EyesTracker : public BaseTracker
{
public:
    EyesTracker(Store* st);

    void track();

    enum type { HAAR, TEST };
    void setDetector(int type);

    cv::Mat* getDispImg();

private:
    BaseDetector* haarDetector;
    BaseDetector* testDetector;
};

#endif // EYES_H
