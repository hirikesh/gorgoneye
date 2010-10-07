#ifndef MODEL_H
#define MODEL_H

#include <cv.h>
#include <highgui.h>
#include <vector>
#include "trackers/facetracker.h"
#include "trackers/eyestracker.h"
#include "filters/basefilter.h"
#include "store.h"

class BaseTracker;

class Model
{
public:
    Model(int device);
    void preProcess();
    void postProcess();
    void update(); // instructs model to grab new frame and process it, thereby updating Store.
    Store* getStore();
    std::vector<BaseTracker*> getTrackers();
    std::vector<BaseFilter*>* getFilters();
    cv::Mat* getDispImg();
private:
    cv::VideoCapture capture;
    Store store;
    FaceTracker faceTracker;
    EyesTracker eyesTracker;
    std::vector<BaseFilter*> filters;
    std::vector<BaseTracker*> trackers;
    // Eye eyeTracker, Nose noseTracker, etc.
    // or possibly an array of pointers to Trackers?
    // eg. vector<Tracker*> trackers;
};

#endif // MODEL_H
