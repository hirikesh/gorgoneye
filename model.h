#ifndef MODEL_H
#define MODEL_H

#include <cv.h>
#include <highgui.h>
#include <vector>
#include "store.h"

// PREPROC_STAGE: 1 - face, 2 - eyes, 3 - gaze, else - none
#define PREPROC_STAGE 1
// TIME_FILTERS: 0 - no ticks, 1 - ticks via QDebug
#define TIME_FILTERS 0

class BaseTracker;
class BaseFilter;

class Model
{
public:
    Model(int device);

    void update(); // instructs model to grab new frame and process it, thereby updating Store.
    void preProcess();
    void postProcess();

    Store* getStore();
    cv::Mat* getDispImg();

    std::vector<BaseFilter*>* getPtrFilters();
    std::vector<BaseTracker*>* getPtrTrackers();

private:
    cv::VideoCapture capture;
    Store store;

    BaseTracker* faceHaarTracker;
    BaseTracker* faceCAMShiftTracker;
    BaseTracker* faceHaarCAMShiftTracker;
    BaseTracker* faceHaarCAMShiftACTracker;

    BaseTracker* eyesHaarTracker;

    BaseTracker* faceTracker;
    BaseTracker* eyesTracker;
    BaseTracker* gazeTracker;

    std::vector<BaseFilter*> filters;
    std::vector<BaseTracker*> trackers;
};

#endif // MODEL_H
