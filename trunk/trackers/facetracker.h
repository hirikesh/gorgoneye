/* FaceTracker:
   The face tracking interface object.
   Encapsulates dynamically switchable
   face tracking algorithms according
   to the Strategy design pattern. */

#ifndef FACE_H
#define FACE_H

#include "basetracker.h"

class FaceTracker : public BaseTracker
{
public:
    FaceTracker(Store* st);

    void track();

private:
    BaseFilter* cannyContourFilter;

    BaseDetector* someDetector;
};

#endif // FACE_H
