#ifndef MODEL_H
#define MODEL_H
#include "trackers/facetracker.h"
#include "parameter.h"
class Model
{
public:
    Model(Store*);
    vector<Param*> getTrackerParams(); // needs to be an array of this in final
private:
    FaceTracker faceTracker;
    // Eye eyeTracker, Nose noseTracker, etc.
    // or possibly an array of pointers to Trackers?
    // eg. vector<Tracker*> trackers;
};

#endif // MODEL_H
