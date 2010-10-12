#ifndef EYES_H
#define EYES_H

#include "basetracker.h"

class EyesTracker : public BaseTracker
{
public:
    EyesTracker(Store* st);

    void track();

//    enum type { HAAR, FEAT, HYBR, TEST };

//    cv::Mat* getDispImg();

private:
    BaseDetector* haarDetector;
    BaseDetector* featureDetector;
    BaseDetector* hybridDetector;
    BaseDetector* testDetector;
};

#endif // EYES_H
