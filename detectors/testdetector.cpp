#include <cv.h>
#include "testdetector.h"

using namespace cv;

TestDetector::TestDetector(const int type) :
    BaseDetector(type, "Test"),
    enTestImg(false)
{
    params.push_back(new ImageModeParam("Test image mode", &enTestImg, &testImg));
}


bool TestDetector::locate(const Mat& srcImg, Rect& srcRoi)
{

    Sobel(srcImg, testImg, 3, 0, 1);
    convertScaleAbs(testImg, testImg);

    return true;
}
