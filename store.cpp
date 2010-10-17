#include "store.h"

Store::Store() :
    faceLocated(false),
    eyesLocated(false),
    gazeLocated(false)
{
    // Initialisation of immediate-use variables
    sceneImg = cv::Mat(FRAME_WIDTH, FRAME_HEIGHT, CV_8UC3, cv::Scalar(0,0,0));
    faceRoi = cv::Rect(0, 0, FRAME_WIDTH, FRAME_HEIGHT);
    dispImg = &ignore;
}
