#include <cv.h>
#include <highgui.h>
#include "model.h"

using namespace cv;
using std::vector;

Model::Model(int device) :
    capture(VideoCapture(device)),
    store(Store()),
    faceTracker(FaceTracker(&store)),
    eyesTracker(EyesTracker(&store))
{
    // initialisation no longer required
    // but leave here anyway, doesn't hurt.
    capture >> store.sceneImg;

    eyesTracker.setDetector(EyesTracker::HAAR);
    eyesTracker.disable();
    trackers.push_back(&eyesTracker);

    faceTracker.setDetector(FaceTracker::HYBR);
    faceTracker.enable();
    trackers.push_back(&faceTracker);

    // current UI display
    store.dispImg = &store.sceneImg;
}

void Model::update()
{
    capture >> store.sceneImg;
    faceTracker.track();
    eyesTracker.track();
    //gazeTracker.track();
}

Store* Model::getStore()
{
    return &store;
}

vector<BaseTracker*> Model::getTrackers()
{
    return trackers;
}

Mat* Model::getDispImg()
{
    return store.dispImg;
}
