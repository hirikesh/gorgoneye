/* Control:
   Overall control object for gorgoneye.
   Used to manage image processing sequencing,
   exposure of the "store" object to external
   user interfaces, and control of all tracker
   parameters to external entities (such as
   the UI). */

#include "store.h"

#include <cv.h>
#include <highgui.h>
using namespace cv;

#ifndef CONTROL_H
#define CONTROL_H

// STUB: consider creating namespace

// Tracker control and resource management
class Control
{
public:
    Control();

private:
    Store* store;
};

#endif // CONTROL_H
