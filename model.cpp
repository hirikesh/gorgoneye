#include "model.h"

Model::Model(Store* st) :
        faceTracker(Face(st))
{
    faceTracker.setDetector(Face::HAAR);
}

vector<Param*> Model::getTrackerParams()
{
    // should run through an array or each tracker individually
    // grabbing all the params for each tracker and then
    // creating the tracker param to send to view (when it requests)
    // however for now since we only have a single tracker, just return
    // its information
    return faceTracker.getDetector()->getParams();
}
