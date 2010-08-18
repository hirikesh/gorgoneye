#include <cv.h>
#include <highgui.h>
using namespace cv;

#include "model.h"
#include "store.h"
#include <QtDebug>

Model::Model(int device) :
    store(Store()),
    capture(VideoCapture(device)),
    faceTracker(FaceTracker(&store))
{
    capture >> store.sceneImg; // initialisation required
    faceTracker.setDetector(FaceTracker::HAAR);
    faceTracker.setEnable(true);
    trackers.push_back(&faceTracker);
}

void Model::update()
{
    capture >> store.sceneImg;
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
