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
    cv::Mat faceImg;
    cv::Mat eyesImg;

    // Mask images (binary)
    cv::Mat sceneMsk;
    cv::Mat faceMsk;
    cv::Mat eyesMsk;

    // Tracked region of interests
    cv::Rect faceRoi; // relative to sceneImg
    cv::Rect eyesRoi; // relative to faceImg
    cv::Rect gazeRoi; // relative to screen

    // Image to overlay
    cv::Mat* dispImg;

    // Multipurpose unitialised image
    // MUST NOT BE ALTERED
    cv::Mat ignore;

    // Gaze training and estimation
    bool training_mode;
    cv::Mat calibImg;
    cv::Point calibPoint;
    cv::Mat gazeImg;
    cv::Point gazePoint;

    // Status flags
    bool faceLocated;
    bool eyesLocated;
    bool gazeLocated;
};

#endif // STORE_H
