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
    static Size srcImgSize = srcImg.size();
    static Mat cHSVImg(srcImgSize, CV_8UC3);
    static Mat hueImg(srcImgSize, CV_8UC1);
    static Mat satImg(srcImgSize, CV_8UC1);
    static Mat valImg(srcImgSize, CV_8UC1);
    static Mat cHSVChannels[] = {hueImg, satImg, valImg};

    // Extract Hue Info
    cvtColor(srcImg, cHSVImg, CV_BGR2HSV);
    split(cHSVImg, cHSVChannels);

    // visualise Hue for debugging
    static Mat emptyImg(srcImgSize, CV_8UC1, Scalar(255));
    static Mat hueVis[] = {hueImg, emptyImg, emptyImg};
    merge(hueVis, 3, hueVisImg);
    // This colour conversion is extremely laggy.
    cvtColor(hueVisImg, hueVisImg, CV_HSV2RGB);

    // set mask ROI
    static Mat maskImg(srcImgSize, CV_8UC1);
    inRange(cHSVImg,
            Scalar(0, minSaturation, minValue),
            Scalar(180, maxSaturation, maxValue),
            maskImg);
    static Mat hueImgROI, maskROI;
    hueImgROI = hueImg(srcRoi);
    maskROI = maskImg(srcRoi);

    // Histogram properties ------------------
    static MatND hist;
    const int hBins = 30; // no. of hue bins
    const int histSize[] = {hBins};
    const float hRanges[] = {0, 180}; // OpenCV implements hue values from 0 to 180
    const float* histRanges[] = {hRanges};
    const int channels[] = {0};

    if (histCalibrate)
    {
        histCalibrate = false;
        // Calculate Histogram ------------------
        calcHist(&hueImgROI,// array of source images
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
    static double maxVal = 0;
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
    static Mat backProjImg3[] = {backProjImg, backProjImg, backProjImg};
    bitwise_and(backProjImg, maskImg, backProjImg, MatND());
    merge(backProjImg3, 3, backProjGrayImg);

    // CAMShift Calculations ---------
    // Search Window begins at region of interest determined using Haar
    // The algorithm will auto increase search window
    static RotatedRect rotTemp;
    rotTemp = CamShift(backProjImg, // back projected image
                       srcRoi,      // initial search window
                       TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 5, 10));

    // Simple but less robust method for bounds. FIXME soon.
    static Rect tmpRoi;
    tmpRoi = rotTemp.boundingRect();
    // Check on bounds. If ROI is invalid, don't update srcRoi.
//    static Rect boundRoi(0, 0, srcImg.cols, srcImg.rows);
//    if (tmpRoi.tl().inside(boundRoi) && tmpRoi.br().inside(boundRoi)) {
    // camshift reports success as long as ROI is at most half the size
    // of the input image, and at least 1 20th the size of the input image.
    if (tmpRoi.area() >= srcImgSize.area()/400 && tmpRoi.area() <= srcImgSize.area()/2) {
        int newTLx = tmpRoi.x < 0 ? 0 : tmpRoi.x;
        int newTLy = tmpRoi.y < 0 ? 0 : tmpRoi.y;
        int newBRx = tmpRoi.br().x > srcImg.cols ? srcImg.cols : tmpRoi.br().x;
        int newBRy = tmpRoi.br().y > srcImg.rows ? srcImg.rows : tmpRoi.br().y;
        srcRoi = Rect(Point(newTLx, newTLy), Point(newBRx, newBRy));
//        srcRoi = tmpRoi;
        return true;
    } else {
        histCalibrate = true;
        return false;
    }
}
