#include <cv.h>
#include "cannycontourfilter.h"
#include "parameter.h"
#include "store.h"

using cv::Mat;
using cv::Size;
using cv::Scalar;

CannyContourFilter::CannyContourFilter(Store* st, double tl, double th, int dt) :
    BaseFilter(st, "Canny-Contour-Fit"),
    threshLow(tl), threshHigh(th), distThresh(dt)
{
    _params.push_back(new RangeParam<double>("Canny threshold low", Param::RANGE_DBL, &threshLow, 0, 1200 , 5));
    _params.push_back(new RangeParam<double>("Canny threshold high", Param::RANGE_DBL, &threshHigh, 0, 1200, 5));
    _params.push_back(new RangeParam<int>("Distance threshold", Param::RANGE, &distThresh, 0, 255, 1));
}

void CannyContourFilter::setParams()
{
}

void CannyContourFilter::filter(const cv::Mat &srcImg, cv::Mat &dstImg, cv::Mat &dstMsk)
{
    // Stop here if disabled
    if(!enabled) return;

    // Canny edge, contour and fit polygon
    _filter(srcImg);

    // Store result
    _store(dstImg, dstMsk);

    // Visualise on request
    _visualise();
}

void CannyContourFilter::_filter(const cv::Mat &src)
{
    // Find Canny edges
    Mat tmpImg, edges;
    cvtColor(src, tmpImg, CV_BGR2GRAY);
    Canny(tmpImg, edges, threshLow, threshHigh, 3);

    // showme
    cvtColor(edges, testImg, CV_GRAY2BGR);

    // FIND CONTOURS BASED ON CANNY EDGES
    Mat tmpedges = edges;
    std::vector<std::vector<cv::Point> > contours;
    findContours(tmpedges, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

    // showme
    testImg2 = src.clone();
    drawContours(testImg2, contours, -1, Scalar(0,0,255), 1);
    return;

    // ATTEMPT TO FIT POLYGON TO FACIAL CONTOUR
    // depends on reliability of canny detector.
    // first attempt:
    //   grabbed contours, used approxPolyDP (dominant points) to
    //   try and find facial contour. Proved troublesome as there
    //   was no convenient method to find the outer-most contours
    //   which are likely to be the entire face/head. Even if we
    //   managed to find the facial profile, contours found typically
    //   did not loop the entire facial region, particularly since
    //   cuts off chin.
    // second attempt:
    //   used opencv's fitEllipse function in order to try and grab
    //   resonable facial region. More often than not ended up with
    //   an ellipse elongated in the x-axis, due to the imbalance
    //   in contour points found around the eyes and the mouth.
    // third attempt:
    //   merged all contour found points into one unordered set of
    //   points for opencv to approxPolyDP. This effectively reduced
    //   the total number of points, however the approxPolyDP function
    //   could not remove inner points caused by the inner contours
    //   (nose, eyes, mouth), resulting in polygon vertexes crossing
    //   each other.
    // fourth attempt:

//        vector<Point> curve;
//        for(unsigned int i = 0; i < curve.size(); i++)
//            circle(testImg3, curve[i], 1, Scalar(0,0,255), 2);
//        fillConvexPoly(testImg3, contours[0], contours[0].size(), Scalar(0,0,255), 8);
//        fillConvexPoly(testImg3, curve.data(), curve.size(), Scalar(0,0,255), 8);

//        approxPolyDP(Mat(merged), curve, 100000, true);
//        fillConvexPoly(testImg3, merged.data(), merged.size(), Scalar(0,0,255), 8);

    // note: merging all contour points
    std::vector<cv::Point2f> mergedCPs;
    for(unsigned int i = 0; i < contours.size(); i++)
        for(unsigned int j = 0; j < contours[i].size(); j++)
            mergedCPs.push_back(contours[i][j]);

    testImg3 = src.clone();
    cv::RotatedRect elip;
    elip = fitEllipse(cv::Mat(mergedCPs));

    // showme
    ellipse(testImg3, elip, cv::Scalar(0,0,255), 1);

    cv::Mat fastMaskImg(src.size(), CV_8UC1, cv::Scalar(0));
    ellipse(fastMaskImg, elip, cv::Scalar(255), -1);

    // ATTEMPT TO RECREATE OUTERMOST CONTOUR
    // note: using fit ellipse's center point, determine
    //       the euclidean distance of each point in the
    //       merged set of contour points. The fruther
    //       the distance, the greater the weight.
    //       normalise the distances found according to
    //       the outer-most point. the weights are
    //       represented by darker levels of gray.
    //       finally, applying thresholding at the 80-100%
    //       normalised level to retain only the
    //       outermost points.
    cv::Mat distImg(src.size(), CV_8UC1, cv::Scalar(0));
    cv::Rect tmpRoi; cv::Point2f diffP;
    double eucDist;
    for(unsigned int i = 0; i < mergedCPs.size(); i++) {
        diffP = mergedCPs[i] - elip.center; // calc distance vector
        eucDist = std::sqrt(std::pow(diffP.x,2) + std::pow(diffP.y,2)); // calc distance
        // set pixel intensity to euclidean distance
        distImg.ptr(mergedCPs[i].y)[(int)mergedCPs[i].x] = cv::saturate_cast<uchar>((int)eucDist);
    }

    // NORMALISE PIXEL INTENSITIES
    double maxEucDist;
    minMaxLoc(distImg, NULL, &maxEucDist);
    distImg = (255/maxEucDist)*distImg;

    // FILTER DISTANCE MATRIX BY THRESHOLDS
    cv::Mat maskImg;
    threshold(distImg, maskImg, (double)distThresh, 255, cv::THRESH_BINARY_INV);

    // showme
    cvtColor(maskImg, testImg4, CV_GRAY2BGR);

    // FIT FINAL ELLISPE USING CANNY AGAIN
    cv::Mat edges2;
    Sobel(maskImg, edges2, 1, 1, 1, 1);

    cv::Mat tmpedges2 = edges2;
    std::vector<std::vector<cv::Point> > contours2;
    findContours(tmpedges2, contours2, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    drawContours(testImg2, contours, -1, cv::Scalar(0,0,255), 1);


//        elip = fitEllipse(maskImg);
//        ellipse(testImg4, elip, Scalar(0, 0, 255), 2);
//        imshow("fail", testImg4);

//        // FILTER DISTANCE MATRIX BY THRESHOLDS
//        Mat maskImg(srcImg.size(), CV_8UC1, Scalar(0));
//        inRange(distImg, Scalar(distLow), Scalar(distHigh), maskImg); // filtering by threshold
//
//        // CREATE BINARY MASK FOR FEATURE DETECTION
//        Mat maskImgCombine[] = {maskImg, maskImg, maskImg};
//        Mat blankImg(srcImg.size(), CV_8UC1, Scalar(0));
//
//        testImg4 = blankImg;
//        elip = fitEllipse(maskImg);
//        ellipse(testImg4, elip, Scalar(255), 1);
//
//
////        ellipse(maskImg, elip, Scalar(255), 1, 8);
//
//        merge(maskImgCombine, 3, testImg4);
//        imshow("test2", testImg4);


//        float r; Point2f center;
//        minEnclosingCircle(Mat(merged), center, r);
//        circle(testImg3, center, r, Scalar(0,0,255), 2);

    // END TEST
}

void CannyContourFilter::_store(cv::Mat &dstImg, cv::Mat &dstMsk)
{
    testImg2.copyTo(dstImg);
}

void CannyContourFilter::_visualise()
{
}
