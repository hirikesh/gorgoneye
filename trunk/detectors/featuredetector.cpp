#include <cv.h>
#include "featuredetector.h"

using namespace cv;

FeatureDetector::FeatureDetector(const int type, int mins, int maxs, int minv, int maxv) :
    BaseDetector(type, "CAMShift"),
    histCalibrate(true),
    minSaturation(mins), // default
    maxSaturation(maxs), // default
    minValue(minv), // default
    maxValue(maxv) // default
{
    params.push_back(new RangeParam<int>("Min. Saturation", Param::RANGE, &minSaturation, 0, 255, 5));
    params.push_back(new RangeParam<int>("Max. Saturation", Param::RANGE, &maxSaturation, 0, 255, 5));
    params.push_back(new RangeParam<int>("Min. Value", Param::RANGE, &minValue, 0, 255, 5));
    params.push_back(new RangeParam<int>("Max. Value", Param::RANGE, &maxValue, 0, 255, 5));
    params.push_back(new ImageModeParam("Back Projected Image", &backProjGrayImg));
    params.push_back(new ImageModeParam("Hue Visualisation Image", &hueVisImg));
}

bool FeatureDetector::locate(const Mat& srcImg, Rect& srcRoi)
{
    // Prepare CV Mats
    static Mat cHSVImg(srcImg.rows, srcImg.cols, CV_8UC3);
    static Mat satImg(srcImg.rows, srcImg.cols, CV_8UC1);
    static Mat valImg(srcImg.rows, srcImg.cols, CV_8UC1);
    static Mat hueImg(srcImg.rows, srcImg.cols, CV_8UC1);
    static Mat maskImg(srcImg.rows, srcImg.cols, CV_8UC1);

    // Extract Hue Info
    cvtColor(srcImg, cHSVImg, CV_BGR2HSV);
    Mat cHSVChannels[] = {hueImg, satImg, valImg};
    split(cHSVImg, cHSVChannels);

    // visualise Hue for debugging
//    Mat hueVisImg(srcImg.rows, srcImg.cols, CV_8UC3);
//    satImg = Scalar(255);
//    valImg = Scalar(255);
//    Mat hueVis[] = {hueImg, satImg, valImg};
//    merge(hueVis, 3, hueVisImg);
    // This colour conversion is extremely laggy.
//   cvtColor(hueVisImg, hueVisImg, CV_HSV2RGB);
//    imshow("Hue Visualisation", hueVisImg);

    // set mask ROI
    inRange(cHSVImg,
            Scalar(0, minSaturation, minValue),
            Scalar(180, maxSaturation, maxValue),
            maskImg);
    //imshow("Masked Image", maskImg);
    Mat hueImgROI(hueImg, srcRoi);
    Mat maskROI(maskImg, srcRoi);

    // Histogram properties ------------------
    //static Mat backProjImg;
    static MatND hist;
    int hBins = 30; // no. of hue bins
    int histSize[] = {hBins};
    float hRanges[] = {0, 180}; // OpenCV implements hue values from 0 to 180
    const float* histRanges[] = {hRanges};
    int channels[] = {0};

    if (histCalibrate)
    {
        histCalibrate = false;
        // Calculate Histogram ------------------
        calcHist(&hueImgROI,  // array of source images
                 1,         // number of source images
                 channels,  // list of channels
                 maskROI,   // image mask
                 hist,      // output histogram
                 1,         // histogram dimensionality - just hue
                 histSize,  // array containing hist sizes
                 histRanges,// arrays containing bin boundaries
                 true,      // uniform histogram
                 false);    // clear histogram from beginning
    }

    // Calculate Back Projection ------------------
    double maxVal = 0;
    minMaxLoc(hist, 0, &maxVal, 0, 0);
    double scaleHist = maxVal? 255.0/maxVal : 0;

    calcBackProject(&hueImg,                 // array of source images
                    1,                       // no. of source images
                    channels,                // list of channels
                    hist,                    // destination backproject array
                    backProjImg,             // output image of back projection
                    histRanges,              // array containing bin boundaries
                    scaleHist,               // scale factor to improve contrast
                    true);                   // uniform histogram

    // show back projection for debugging / parameter tweaking
    bitwise_and(backProjImg, maskImg, backProjImg, MatND());
    // imshow("Filtered Back Projected Image", backProjImg);
    Mat backProjImg3[] = {backProjImg, backProjImg, backProjImg};
    merge(backProjImg3, 3, backProjGrayImg);

    // CAMShift Calculations ---------
    // Search Window begins at region of interest determined using Haar
    // The algorithm will auto increase search window
    RotatedRect rotTemp;
    rotTemp = CamShift(backProjImg, // back projected image
                       srcRoi,      // initial search window
                       TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 5, 10));

    // Simple but less robust method for bounds. FIXME soon.
    static Rect boundRoi(0, 0, srcImg.cols, srcImg.rows);
    static Rect tmpSrcRoi;
    tmpSrcRoi = rotTemp.boundingRect();
    // Check on bounds. If ROI is invalid, don't update srcRoi.
    if (tmpSrcRoi.tl().inside(boundRoi) && tmpSrcRoi.br().inside(boundRoi)) {
        srcRoi = tmpSrcRoi;
        return true;
    } else {
        histCalibrate = true;
        return false;
    }

    // Modify bounds on camshift rectangle
    // TODO: A more elegant range check function
//    if (srcRoi.x <= 0)
//    {srcRoi.x = 0;}
//    if (srcRoi.width <= 0)
//    {srcRoi.width = 0;}
//    if (srcRoi.y <= 0)
//    {srcRoi.y = 0;}
//    if (srcRoi.height <= 0)
//    {srcRoi.height = 0;}
//    if (srcRoi.width >= srcImg.cols)
//    {srcRoi.width = srcImg.cols;}
//    if (srcRoi.x >= srcImg.cols)
//    {srcRoi.x = srcImg.cols;}
//    if (srcRoi.y >= srcImg.rows)
//    {srcRoi.y = srcImg.rows;}
//    if (srcRoi.height >= srcImg.rows)
//    {srcRoi.height = srcImg.rows;}
//    if (srcRoi.x + srcRoi.width >= srcImg.cols)
//    {srcRoi.width = srcImg.cols - srcRoi.x;}
//    if (srcRoi.y + srcRoi.height >= srcImg.rows)
//    {srcRoi.height = srcImg.rows - srcRoi.y;}

//    return true;
}
