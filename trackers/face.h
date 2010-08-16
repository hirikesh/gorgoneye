/* Face:
   The face tracking interface object.
   Encapsulates dynamically switchable
   face tracking algorithms according
   to the Strategy design pattern. */

#include "store.h"
#include "tracker.h"
#include "detector.h"
#include "detectors/haar.h"
#include "detectors/feature.h"

#ifndef FACE_H
#define FACE_H

class Face : public Tracker
{
public:
    Face(Store* st);

    void track();

    enum type { HAAR, FEAT };
    void setDetector(int type);
private:
    Detector* haarDetector;
    Detector* featureDetector;
};

#endif // FACE_H
