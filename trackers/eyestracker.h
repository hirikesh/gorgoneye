#ifndef EYES_H
#define EYES_H

#include "basetracker.h"

class EyesTracker : public BaseTracker
{
public:
    EyesTracker(Store* st);

    void track();

private:
    BaseDetector* someDetector;
};

#endif // EYES_H
