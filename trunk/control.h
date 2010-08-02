/* Control:
   Overall control object for gorgoneye.
   Used to manage image processing sequence,
   exposure of the store object, and control
   of tracker parameters to external entities
   (such as the UI). */

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
};

#endif // CONTROL_H
