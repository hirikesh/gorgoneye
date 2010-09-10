#ifndef MODEL_H
#define MODEL_H

#include <cv.h>
#include <highgui.h>
#include <vector>
#include "trackers/facetracker.h"
#include "store.h"

class BaseTracker;

class Model
{
public:
    Model(int device);
    void update(); // instructs model to grab new frame and process it, thereby updating Store.
    Store* getStore();
    std::vector<BaseTracker*> getTrackers();
    cv::Mat* getFaceDispImg();
private:
    cv::VideoCapture capture;
    Store store;
    FaceTracker faceTracker;
    std::vector<BaseTracker*> trackers;
    // Eye eyeTracker, Nose noseTracker, etc.
    // or possibly an array of pointers to Trackers?
    // eg. vector<Tracker*> trackers;
};

#endif // MODEL_H
