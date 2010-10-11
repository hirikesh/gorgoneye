#ifndef MODEL_H
#define MODEL_H

#include <cv.h>
#include <highgui.h>
#include <vector>
#include "trackers/facehaartracker.h"
#include "trackers/facecamshifttracker.h"
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
    std::vector<BaseTracker*>* getPtrTrackers();
    std::vector<BaseFilter*>* getPtrFilters();
    cv::Mat* getDispImg();

private:
    cv::VideoCapture capture;
    Store store;

    BaseTracker* faceHaarTracker;
    BaseTracker* faceCAMShiftTracker;
    BaseTracker* faceTracker;
    BaseTracker* eyesTracker;

    std::vector<BaseFilter*> filters;
    std::vector<BaseTracker*> trackers;
};

#endif // MODEL_H
