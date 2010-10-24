#include "store.h"

Store::Store() :
    calibMode(false),
    calibNow(false),
    faceLocated(false),
    eyesLocated(false),
    eyesLocatedL(false),
    gazeLocated(false)
{
    // Initialise these variables to be safe
    sceneImg = cv::Mat(FRAME_WIDTH, FRAME_HEIGHT, CV_8UC3, cv::Scalar(0,0,0));
    faceImg = sceneImg.clone();
    eyesImg = sceneImg.clone();
    eyesImgL = sceneImg.clone();
    gazeImg = cv::Mat(FRAME_WIDTH, FRAME_HEIGHT, CV_8UC1, cv::Scalar(0));

    sceneMsk = cv::Mat(FRAME_WIDTH, FRAME_HEIGHT, CV_8UC1, cv::Scalar(255));
    faceMsk = sceneMsk.clone();
    eyesMsk = sceneMsk.clone();

    faceRoi = cv::Rect(0, 0, FRAME_WIDTH, FRAME_HEIGHT);
    eyesRoi = faceRoi;
    eyesRoiL = faceRoi;
    gazeRoi = faceRoi;

    // Initialise calibration/trianing matrices
    gazeXvalFeatures = cv::Mat(2048, 4096, CV_32FC1); // cross validation set
    gazeXvalCoords = cv::Mat(2048, 2, CV_32FC1); // cross validation output coordinates
    gazeFeatures = cv::Mat(4096, 4096, CV_32FC1); // training set
    gazeCoords = cv::Mat(4096, 2, CV_32FC1); // training output coordinates
    gazePoint = cv::Mat(1, 2, CV_32FC1); // gaze estimation result store

    // Set overlay image to nothing
    dispImg = &ignore;
}
