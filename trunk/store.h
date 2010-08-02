/* Store:
   State-like object which houses the data
   processed by gorgoneye throughout all stages. */

#include <cv.h>
#include <highgui.h>
using namespace cv;

#ifndef STORE_H
#define STORE_H

class Store
{
public:
    Store();

protected:
    // may need to move private members here
    // later on, depending on the needs of
    // external entities inheriting this object.

private:
    Mat sceneImg;
    Mat faceImg;
    Mat eyeImg;

    Rect faceRoi; // relative to sceneImg
    Rect eyeRoi;  // relative to faceImg
    Rect gazeRoi; // relative to screen
};

#endif // STORE_H
