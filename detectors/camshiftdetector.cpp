#include <cv.h>
#include "camshiftdetector.h"
#include "parameter.h"
#include "store.h"

using namespace cv;

CAMShiftDetector::CAMShiftDetector(Store* st) :
      BaseDetector(st, "CAMShift"),
      enBackProjImg(false),
      histCalibrate(true)
{
    _images.push_back(new ImageModeParam("Back Projected Image", &enBackProjImg, &backProjVisImg, &st->dispImg));
}

bool CAMShiftDetector::locate(const Mat& srcImg, const Mat& srcMsk, Rect& srcRoi)
{
    // Histogram properties ------------------
    const int hBins = 30; // no. of hue bins
    const int histSize[] = {hBins};
    const float hRanges[] = {0, 180}; // OpenCV implements hue values from 0 to 180
    const float* histRanges[] = {hRanges};
    const int channels[] = {0};

    if (histCalibrate) {
        // Calculate histogram if last camshift failed
        Mat srcImgRoi = srcImg(srcRoi);
        Mat maskImgRoi = srcMsk(srcRoi);

        // Calculate Histogram ------------------
        calcHist(&srcImgRoi,// array of source images
                 1,         // number of source images
                 channels,  // list of channels
                 maskImgRoi,// image mask
                 hist,      // output histogram
                 1,         // histogram dimensionality - just hue
                 histSize,  // array containing hist sizes
                 histRanges,// arrays containing bin boundaries
                 true,      // uniform histogram
                 false);    // clear histogram from beginning

        histCalibrate = false;
    }

    // Calculate Back Projection ------------------
    double maxVal = 0;
    minMaxLoc(hist, NULL, &maxVal);
    double scaleHist = maxVal ? 255.0/maxVal : 0;

    calcBackProject(&srcImg,                 // array of source images
                    1,                       // no. of source images
                    channels,                // list of channels
                    hist,                    // destination backproject array
                    backProjImg,             // output image of back projection
                    histRanges,              // array containing bin boundaries
                    scaleHist,               // scale factor to improve contrast
                    true);                   // uniform histogram

    bitwise_and(backProjImg, srcMsk, backProjImg, MatND());

    // show back projection for debugging / parameter tweaking
    if(enBackProjImg) {
        Mat backProjImg3[] = {backProjImg, backProjImg, backProjImg};
        merge(backProjImg3, 3, backProjVisImg);
    }

    // CAMShift Calculations ---------
    // Search Window begins at region of interest determined by srcRoi
    // The algorithm will auto increase search window
    Rect tmpSrcRoi = srcRoi;
    RotatedRect rotTemp;
    rotTemp = CamShift(backProjImg, // back projected image
                       tmpSrcRoi,   // initial search window
                       TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 5, 10));

    // Simple but less robust method for bounds.
    Rect tmpRoi = rotTemp.boundingRect();
    // Check on bounds. If ROI is invalid, don't update srcRoi.
    // camshift reports success as long as ROI is at most half the size
    // of the input image, and at least 1 20th the size of the input image.
    if (tmpRoi.area() >= srcImg.size().area()/768 && tmpRoi.area() <= srcImg.size().area()/2) {
        int newTLx = tmpRoi.x < 0 ? 0 : tmpRoi.x;
        int newTLy = tmpRoi.y < 0 ? 0 : tmpRoi.y;
        int newBRx = tmpRoi.br().x > srcImg.cols ? srcImg.cols - 1 : tmpRoi.br().x;
        int newBRy = tmpRoi.br().y > srcImg.rows ? srcImg.rows - 1 : tmpRoi.br().y;
        srcRoi = Rect(Point(newTLx, newTLy), Point(newBRx, newBRy));
        return true;
    } else {
        histCalibrate = true;
        return false;
    }
}
