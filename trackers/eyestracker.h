#ifndef EYES_H
#define EYES_H

#include "basetracker.h"

class EyesTracker : public BaseTracker
{
public:
    EyesTracker(Store* st);

    void track();

private:
    BaseFilter* ycbcrFilter;

    BaseDetector* someDetector;
};

#endif // EYES_H
