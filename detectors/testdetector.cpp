#include <cv.h>
#include <highgui.h>
#include "parameter.h"
#include "testdetector.h"
#include "store.h"

using namespace cv;

TestDetector::TestDetector(Store *st, const int type) :
    BaseDetector(st, type, "Test")
{
    _params.push_back(new RangeParam<int>("Min. Sobel", Param::RANGE, &minSobel, 0, 255, 1));
    _params.push_back(new RangeParam<int>("Min. Sobel", Param::RANGE, &maxSobel, 0, 255, 1));
    imageModes.push_back(new ImageModeParam("Sobel image mode", &sobelImg, &st->dispImg));
    imageModes.push_back(new ImageModeParam("Test image mode", &testImg, &st->dispImg));
    minSobel = 45;
    maxSobel = 255;
}


bool TestDetector::locate(const Mat& srcImg, Rect& srcRoi)
{
//    Mat yImg(srcImg.size(), CV_8UC1);
//    Mat yccImg[] = {yImg, Mat(), Mat()};
//
//    Mat tmpImg;
//    cvtColor(srcImg, tmpImg, CV_BGR2YCrCb);
//    split(tmpImg, yccImg);

    // Sobel test
//    Mat nyImg(srcImg.size(), CV_8UC1);
    Mat tmpImg, tmpImg2, tmpImg3;
    Sobel(srcImg, tmpImg, 3, 0, 1, 3);
    convertScaleAbs(tmpImg, tmpImg);
    cvtColor(tmpImg, tmpImg2, CV_BGR2GRAY);
    inRange(tmpImg2, Scalar(minSobel), Scalar(maxSobel), tmpImg3);

//    imshow("masked sobel", tmpImg3);
    cvtColor(tmpImg3, sobelImg, CV_GRAY2BGR);

//    Mat tmpImg4[] = {tmpImg3, tmpImg3, tmpImg3};
//    merge(tmpImg4, 3, sobelImg);

//    Mat neutImg(srcImg.size(), CV_8UC1, Scalar(128));
//    Mat bgrImg[] = {nyImg, neutImg, neutImg};
//    merge(bgrImg, 3, testImg);
//    cvtColor(testImg, testImg, CV_YCrCb2BGR);



    // Harris test
    Mat gray;
    cvtColor(srcImg(srcRoi), gray, CV_BGR2GRAY);
//    gray = tmpImg3;

    std::vector<Point2f> corners;
    goodFeaturesToTrack(gray, // input grayscale image
                        corners, // output points found
                        2, // max number of corners to return
                        0.01, // min % quality level for a positive
                        10, // min distance between any 2 points
                        Mat(), // mask image
                        7, // Sobel kernel dimensions
                        false, // use Harris detection
                        0.10); // free parameter of Harris detector

    cvtColor(gray, testImg, CV_GRAY2BGR);
    for(unsigned int i = 0; i < corners.size(); i++)
        circle(testImg, corners[i], 2, Scalar(0, 0, 255), 2);

    return false;
}
