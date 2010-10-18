#include "store.h"

Store::Store() :
    calibMode(false),
    calibX(0), calibY(0),
    faceLocated(false),
    eyesLocated(false),
    gazeLocated(false)
{
    // Initialise these variables to be safe
    sceneImg = cv::Mat(FRAME_WIDTH, FRAME_HEIGHT, CV_8UC3, cv::Scalar(0,0,0));
    faceImg = sceneImg.clone();
    eyesImg = sceneImg.clone();
    gazeImg = sceneImg.clone();

    sceneMsk = cv::Mat(FRAME_WIDTH, FRAME_HEIGHT, CV_8UC1, cv::Scalar(255));
    faceMsk = sceneMsk.clone();
    eyesMsk = sceneMsk.clone();

    faceRoi = cv::Rect(0, 0, FRAME_WIDTH, FRAME_HEIGHT);
    eyesRoi = faceRoi;
    gazeRoi = faceRoi;

    // Set overlay image to nothing
    dispImg = &ignore;
}
