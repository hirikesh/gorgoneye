/* FaceTracker:
   The face tracking interface object.
   Encapsulates dynamically switchable
   face tracking algorithms according
   to the Strategy design pattern. */

#include "store.h"
#include "basetracker.h"
#include "detectors/basedetector.h"
#include "detectors/haardetector.h"
#include "detectors/featuredetector.h"

#ifndef FACE_H
#define FACE_H

class FaceTracker : public BaseTracker
{
public:
    FaceTracker(Store* st);

    void track();

    enum type { HAAR, FEAT };
    void setDetector(int type);
private:
    BaseDetector* haarDetector;
    BaseDetector* featureDetector;
};

#endif // FACE_H
