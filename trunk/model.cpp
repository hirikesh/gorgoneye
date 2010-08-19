#include <cv.h>
#include <highgui.h>
using namespace cv;

#include "model.h"
#include "store.h"

Model::Model(int device) :
    store(Store()),
    capture(VideoCapture(device)),
    faceTracker(FaceTracker(&store))
{
    capture >> store.sceneImg; // initialisation required
    cvtColor(store.sceneImg, store.sceneImg, CV_BGR2RGB);

    faceTracker.setDetector(FaceTracker::HAAR);
    faceTracker.setEnable(true);
    trackers.push_back(&faceTracker);
}

void Model::update()
{
    capture >> store.sceneImg;
    cvtColor(store.sceneImg, store.sceneImg, CV_BGR2RGB);
    if(faceTracker.isEnabled()) faceTracker.track();
}

Store* Model::getStore()
{
    return &store;
}

vector<BaseTracker*> Model::getTrackers()
{
    return trackers;
}
