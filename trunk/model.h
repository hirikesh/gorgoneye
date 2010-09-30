#ifndef MODEL_H
#define MODEL_H

#include <cv.h>
#include <highgui.h>
#include <vector>
#include "trackers/facetracker.h"
#include "trackers/eyestracker.h"
#include "store.h"

class BaseTracker;

class Model
{
public:
    Model(int device);
    void update(); // instructs model to grab new frame and process it, thereby updating Store.
    Store* getStore();
    std::vector<BaseTracker*> getTrackers();
    cv::Mat* getDispImg();
private:
    cv::VideoCapture capture;
    Store store;
    FaceTracker faceTracker;
    EyesTracker eyesTracker;
    std::vector<BaseTracker*> trackers;
    // Eye eyeTracker, Nose noseTracker, etc.
    // or possibly an array of pointers to Trackers?
    // eg. vector<Tracker*> trackers;
};

#endif // MODEL_H
