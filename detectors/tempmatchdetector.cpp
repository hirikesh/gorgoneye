#include <cv.h>
#include "tempmatchdetector.h"
#include "parameter.h"

using namespace cv;

TempMatchDetector::TempMatchDetector(Store* st, int md) :
    BaseDetector(st, "Template Matching"),
    method(md)
{
    _params.push_back(new RangeParam<int>("Method: SQDIFF(_N) CCORR(_N) CCOEFF(_N)", Param::RANGE, &method, 0, 5, 1));
}

bool TempMatchDetector::locate(const cv::Mat& srcImg, const cv::Mat& srcTmp, cv::Rect& srcRoi)
{
    cv::Mat matchResult;
    matchTemplate(srcImg, srcTmp, matchResult, method);

    double a; cv::Point b;
    cv::Point centroid;
    if(method < 2)
        cv::minMaxLoc(matchResult, &a, &a, &centroid, &b);
    else
        cv::minMaxLoc(matchResult, &a, &a, &b, &centroid);


//    srcRoi = cv::Rect(centroid.x - srcTmp.cols/2,
//                      centroid.y - srcTmp.rows/2,
//                      srcTmp.cols, srcTmp.rows);
        srcRoi = cv::Rect(centroid.x, centroid.y,1,1);
    return true;
}
