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

    // Images to process / already processed
    Mat sceneImg;
    Mat faceImg;
    Mat eyesImg;

    // Tracked region of interests
    Rect faceRoi; // relative to sceneImg
    Rect eyesRoi;  // relative to faceImg
    Rect gazeRoi; // relative to screen

    // Status flags
    bool faceLocated;
    bool eyesLocated;
    bool gazeLocated;
};

#endif // STORE_H
