#include <cv.h>
#include <highgui.h>
#include "trackers/facetracker.h"
#include "parameter.h"
#include "store.h"

#ifndef MODEL_H
#define MODEL_H

class Model
{
public:
    Model(int device);
    void update(); // instructs model to grab new frame and process it, thereby updating Store.
    Store* getStore();
    vector<BaseDetector*> getTrackerParams(); // needs to be an array of this in final
private:
    Store store;
    VideoCapture capture;
    FaceTracker faceTracker;
    // Eye eyeTracker, Nose noseTracker, etc.
    // or possibly an array of pointers to Trackers?
    // eg. vector<Tracker*> trackers;
};

#endif // MODEL_H
