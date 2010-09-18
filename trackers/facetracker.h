/* FaceTracker:
   The face tracking interface object.
   Encapsulates dynamically switchable
   face tracking algorithms according
   to the Strategy design pattern. */

#define HAAR_CC_FACE_DEFAULT "c:\\opencv2.1\\data\\haarcascades\\haarcascade_frontalface_default.xml"

#ifndef FACE_H
#define FACE_H

#include "basetracker.h"

class FaceTracker : public BaseTracker
{
public:
    FaceTracker(Store* st);

    void track();

    enum type { HAAR, FEAT, HYBR };
    void setDetector(int type);

private:
    BaseDetector* haarDetector;
    BaseDetector* featureDetector;
    BaseDetector* hybridDetector;
};

#endif // FACE_H
