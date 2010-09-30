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
    cv::Mat* getDispImg();

    enum type { HAAR, FEAT, HYBR };
    void setDetector(int type);

private:
    BaseDetector* haarDetector;
    BaseDetector* featureDetector;
    BaseDetector* hybridDetector;
};

#endif // FACE_H
