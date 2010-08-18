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

vector<BaseDetector*> Model::getTrackerParams()
{
    // should run through an array or each tracker individually
    // grabbing all the params for each tracker and then
    // creating the tracker param to send to view (when it requests)
    // however for now since we only have a single tracker, just return
    // its information
    vector<BaseDetector*> detectors;

    if (faceTracker.isEnabled()) {
        detectors.push_back(faceTracker.getDetector());
    }
    return detectors;
}
