/* Store:
   State-like object which houses the data
   processed by gorgoneye throughout all stages. */
#ifndef STORE_H
#define STORE_H

#include <cv.h>
#include "config.h"

class Store
{
public:
    Store();
    // Images to process / already processed
    cv::Mat sceneImg;
    cv::Mat faceImg;
    cv::Mat eyesImg;
    cv::Mat gazeImg;

    // Mask images (binary)
    cv::Mat sceneMsk;
    cv::Mat faceMsk;
    cv::Mat eyesMsk;

    // Tracked regions
    cv::Rect faceRoi; // relative to sceneImg
    cv::Rect eyesRoi; // relative to faceImg
    cv::Rect gazeRoi; // relative to screen

    // Gaze environment
//    int gazeWidth;
//    int gazeHeight;

    // Gaze calibration
    bool calibMode;
    bool calibNow;
    int calibX;
    int calibY;
    int calibW;
    int calibH;
    cv::Mat gazeFeatures;
    cv::Mat gazeCoords;

    // ML Gaze estimation
    cv::Mat gazePoint;
    float gazeX;
    float gazeY;

    // Status flags
    bool faceLocated;
    bool eyesLocated;
    bool gazeLocated;

    // Image to overlay
    cv::Mat* dispImg;

    // Multipurpose unitialised image
    // MUST NOT BE MODIFIED
    cv::Mat ignore;
};

#endif // STORE_H
