/* Store:
   State-like object which houses the data
   processed by gorgoneye throughout all stages. */
#ifndef STORE_H
#define STORE_H

#include <cv.h>

class Store
{
public:
    Store();
    // Images to process / already processed
    cv::Mat sceneImg;
    cv::Mat* faceImg;
    cv::Mat* eyesImg;

    // Tracked region of interests
    cv::Rect faceRoi; // relative to sceneImg
    cv::Rect eyesRoi;  // relative to faceImg
    cv::Rect gazeRoi; // relative to screen

    // Status flags
    bool faceLocated;
    bool eyesLocated;
    bool gazeLocated;
};

#endif // STORE_H
