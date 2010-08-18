#include "model.h"

Model::Model(Store* st) :
        faceTracker(FaceTracker(st))
{
    faceTracker.setDetector(FaceTracker::HAAR);
    faceTracker.setEnable(true);
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
