#include <cv.h>
#include <QDebug>
#include "cannycontourfilter.h"
#include "parameter.h"
#include "store.h"

using std::vector;
using cv::Mat;
using cv::Scalar;
using cv::Point;
using cv::Point2f;

CannyContourFilter::CannyContourFilter(Store* st, double tl, double th, double mnd, double mxd) :
    BaseFilter(st, "Canny-Contour-Fit"),
    minGrad(tl), maxGrad(th),
    minDist(mnd), maxDist(mxd)
{
    _images.push_back(new ImageModeParam("Canny edges", &testImg, &st->dispImg));
    _images.push_back(new ImageModeParam("Canny contours", &testImg2, &st->dispImg));
    _images.push_back(new ImageModeParam("Ellipse fit", &testImg3, &st->dispImg));
    _images.push_back(new ImageModeParam("Canny-contour fit", &testImg4, &st->dispImg));
    _params.push_back(new RangeParam<double>("Canny threshold low", Param::RANGE_DBL, &minGrad, 0, 1200 , 5));
    _params.push_back(new RangeParam<double>("Canny threshold high", Param::RANGE_DBL, &maxGrad, 0, 1200, 5));
    _params.push_back(new RangeParam<double>("Min. distance %", Param::RANGE_DBL, &minDist, 0, 1, 0.01));
    _params.push_back(new RangeParam<double>("Max. distance %", Param::RANGE_DBL, &maxDist, 0, 1, 0.01));
}

void CannyContourFilter::setParams()
{
}

void CannyContourFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, cv::Mat& dstMsk)
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
    Canny(tmpImg, edges, minGrad, maxGrad, 3);

    // showme
    cvtColor(edges, testImg, CV_GRAY2BGR);

    // FIND CONTOURS BASED ON CANNY EDGES
    Mat tmpedges = edges;
    vector<vector<Point> > contours;
    findContours(tmpedges, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

    // showme
    testImg2 = src.clone();
    drawContours(testImg2, contours, -1, Scalar(255,0,0), 1);

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

    // Calculate euclidean distance of all contour points
    // and filter through thresholds.
//    Point2f centerP(src.cols/2., src.rows/2.);
    double maxEucDist = std::sqrt(std::pow(src.cols/2.,2) + std::pow(src.rows/2.,2));
//    vector<Point2f> desiredCPs; // for circle and ellipse fitting
    vector<Point> desiredCPs; // for polygons
//    vector<Point> desiredCPsTop; // for polygons
//    vector<Point> desiredCPsBot; // for polygons
    for(unsigned int i = 0; i < contours.size(); i++)
    {
        for(unsigned int j = 0; j < contours[i].size(); j++)
        {
            double x = contours[i][j].x - src.cols/2.;
            double y = contours[i][j].y - src.rows/2.;
            double d = std::sqrt(std::pow(x,2) + std::pow(y,2)) / maxEucDist;
            if(minDist <= d && d < maxDist)
            {
                desiredCPs.push_back(contours[i][j]);
//                if(y<0)
//                    desiredCPsTop.push_back(contours[i][j]);
//                else
//                    desiredCPsBot.push_back(contours[i][j]);
            }
        }
    }
//    std::sort(desiredCPsTop.begin(), desiredCPsTop.end(), _predicate);
//    std::sort(desiredCPsBot.rbegin(), desiredCPsBot.rend(), _predicate);
//    desiredCPsTop.insert(desiredCPsTop.end(), desiredCPsBot.begin(), desiredCPsBot.end());
//    vector<Point> desiredCPs = desiredCPsBot;

    // showme
    testImg3 = src.clone();

    // Visualise points
//    for(unsigned int i = 0; i < desiredCPs.size(); i++)
//        circle(testImg3, desiredCPs[i], 1, Scalar(0,0,255), 2);

//    return;

//     FIT ellipse to see results for now
//    cv::RotatedRect elip;
//    if(desiredCPs.size() > 5)
//    {
//        elip = fitEllipse(Mat(desiredCPs));
//        ellipse(testImg3, elip, Scalar(0,0,255), 2);
//    }

    // THIS IS WHAT WE WANT
    fillConvexPoly(testImg3, desiredCPs.data(), desiredCPs.size(), Scalar(0,0,255), 8);
    return;

    // note: merging all contour points
    vector<Point2f> mergedCPs;
    for(unsigned int i = 0; i < contours.size(); i++)
        for(unsigned int j = 0; j < contours[i].size(); j++)
            mergedCPs.push_back(contours[i][j]);

    // ELLIPSE FIT TO FIND CENTRAL POINT
//    testImg3 = src.clone();
//    cv::RotatedRect elip;
//    elip = fitEllipse(Mat(mergedCPs));

    // showme
//    ellipse(testImg3, elip, Scalar(0,0,255), 1);

    // ASSUME CENTRAL POINT IS CENTER OF SOURCE IMAGE
    Point2f centerP(src.cols/2, src.rows/2);
//    Mat fastMaskImg(src.size(), CV_8UC1, Scalar(0));
//    ellipse(fastMaskImg, elip, Scalar(255), -1);

    // ATTEMPT TO RECREATE OUTERMOST CONTOUR
    // note: using fit ellipse's center point, determine
    //       the euclidean distance of each point in the
    //       merged set of contour points. The further
    //       the distance, the greater the weight.
    //       normalise the distances found according to
    //       the outer-most point. the weights are
    //       represented by darker levels of gray.
    //       finally, applying thresholding at the 80-100%
    //       normalised level to retain only the
    //       outermost points.
    Mat distImg(src.size(), CV_32FC1, Scalar(0));
    Point2f diffP;
    double eucDist;
    for(unsigned int i = 0; i < mergedCPs.size(); i++) {
//        diffP = mergedCPs[i] - elip.center; // calc distance vector
//        diffP = mergedCPs[i] - centerP; // other method
        eucDist = std::sqrt(std::pow(diffP.x,2) + std::pow(diffP.y,2)); // calc distance
        // set pixel intensity to euclidean distance
        distImg.ptr(mergedCPs[i].y)[(int)mergedCPs[i].x] = cv::saturate_cast<uchar>((int)eucDist);
    }

    // NORMALISE PIXEL INTENSITIES
//    double maxEucDist = std::sqrt(std::pow(centerP.x,2) + std::pow(centerP.y,2));
//    minMaxLoc(distImg, NULL, &maxEucDist);
    distImg = (1./maxEucDist)*distImg;

    // FILTER DISTANCE MATRIX BY THRESHOLDS
    Mat maskImg;
    inRange(distImg, Scalar(minDist), Scalar(maxDist), maskImg);
//    threshold(distImg, maskImg, (double)distThresh, 255, cv::THRESH_BINARY_INV);

    // showme
    cvtColor(maskImg, testImg4, CV_GRAY2BGR);
return;
    // FIT FINAL ELLISPE USING CANNY AGAIN
    Mat edges2;
    Sobel(maskImg, edges2, 1, 1, 1, 1);

    Mat tmpedges2 = edges2;
    vector<vector<Point> > contours2;
    findContours(tmpedges2, contours2, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    drawContours(testImg2, contours, -1, Scalar(0,0,255), 1);


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
//    testImg.copyTo(dstImg);
}

void CannyContourFilter::_visualise()
{
}

bool _predicate(const Point& p1, const Point& p2)
{
    return p1.x < p2.x;
}
