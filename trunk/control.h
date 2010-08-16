/* Control:
   Overall control object for gorgoneye.
   Used to manage image processing sequencing,
   exposure of the "store" object to external
   user interfaces, and control of all tracker
   parameters to external entities (such as
   the UI). */

#include "store.h"
#include "trackers/basetracker.h"

using namespace cv;

#ifndef CONTROL_H
#define CONTROL_H

// STUB: consider creating namespace

// Tracker control and resource management
class Control
{
public:
    Control();

    void start();

private:
    Store* store;

    BaseTracker* faceTracker;
    BaseTracker* eyesTracker;
    BaseTracker* gazeTracker;
};

#endif // CONTROL_H
