#include <cv.h>
#include "testdetector.h"

using namespace cv;

TestDetector::TestDetector(const int type) :
    BaseDetector(type, "Test")
{
    params.push_back(new ImageModeParam("Test image mode", &testImg));
}


bool TestDetector::locate(const Mat& srcImg, Rect& srcRoi)
{



//    if(rois.size()) {
//        srcRoi = rois[0];
//        return true;
//    } else {
//        return false;
//    }
}
