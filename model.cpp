#include <cv.h>
#include <highgui.h>

#include "model.h"

using namespace cv;
using std::vector;

Model::Model(int device) :
    capture(VideoCapture(device)),
    store(Store()),
    faceTracker(FaceTracker(&store))
{
    capture >> store.sceneImg; // initialisation required
    faceTracker.setDetector(FaceTracker::HAAR);
    faceTracker.setEnable(true);
    trackers.push_back(&faceTracker);
    store.faceImg = &store.sceneImg; // current UI display
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

Mat* Model::getFaceDispImg()
{
    return store.faceImg;
}
