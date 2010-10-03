#include <cv.h>
#include "featuredetector.h"
#include <QDebug>

using namespace cv;

FeatureDetector::FeatureDetector(const int type, int mins, int maxs, int minv, int maxv,
                                                 int mincb, int maxcb, int mincr, int maxcr) :
    BaseDetector(type, "CAMShift"),
    histCalibrate(true),
    minSaturation(mins),
    maxSaturation(maxs),
    minValue(minv),
    maxValue(maxv),
    minChromaBlue(mincb),
    maxChromaBlue(maxcb),
    minChromaRed(mincr),
    maxChromaRed(maxcr)
{
    params.push_back(new RangeParam<int>("Min. Saturation", Param::RANGE, &minSaturation, 0, 255, 5));
    params.push_back(new RangeParam<int>("Max. Saturation", Param::RANGE, &maxSaturation, 0, 255, 5));
    params.push_back(new RangeParam<int>("Min. Value", Param::RANGE, &minValue, 0, 255, 5));
    params.push_back(new RangeParam<int>("Max. Value", Param::RANGE, &maxValue, 0, 255, 5));
    params.push_back(new RangeParam<int>("Min. Cb", Param::RANGE, &minChromaBlue, 0, 255, 5));
    params.push_back(new RangeParam<int>("Max. Cb", Param::RANGE, &maxChromaBlue, 0, 255, 5));
    params.push_back(new RangeParam<int>("Min. Cr", Param::RANGE, &minChromaRed, 0, 255, 5));
    params.push_back(new RangeParam<int>("Max. Cr", Param::RANGE, &maxChromaRed, 0, 255, 5));
    params.push_back(new ImageModeParam("Back Projected Image", &backProjGrayImg));
    params.push_back(new ImageModeParam("Hue Visualisation Image", &hueVisImg));
    params.push_back(new ImageModeParam("Cb Component Image", &chromaBlueVisImg));
    params.push_back(new ImageModeParam("Cr Component Image", &chromaRedVisImg));
}

bool FeatureDetector::locate(const Mat& srcImg, Rect& srcRoi)
{
    static Size srcImgSize = srcImg.size();

    // Extract Hue Info
    static Mat cHSVImg(srcImgSize, CV_8UC3);
    static Mat hueImg(srcImgSize, CV_8UC1);
    static Mat satImg(srcImgSize, CV_8UC1);
    static Mat valImg(srcImgSize, CV_8UC1);
    static Mat cHSVChannels[] = {hueImg, satImg, valImg};
    cvtColor(srcImg, cHSVImg, CV_BGR2HSV);
    split(cHSVImg, cHSVChannels);

    // Extract Chrominance info
    static Mat cYCrCbImg(srcImgSize, CV_8UC3);
    static Mat lumaImg(srcImgSize, CV_8UC1);
    static Mat chromaRedImg(srcImgSize, CV_8UC1);
    static Mat chromaBlueImg(srcImgSize, CV_8UC1);
    static Mat cYCrCbChannels[] = {lumaImg, chromaRedImg, chromaBlueImg};
    cvtColor(srcImg, cYCrCbImg, CV_BGR2YCrCb);
    split(cYCrCbImg, cYCrCbChannels);

    // visualise Hue for debugging
    static Mat emptyImg(srcImgSize, CV_8UC1, Scalar(255));
    static Mat hueVis[] = {hueImg, emptyImg, emptyImg};

    merge(hueVis, 3, hueVisImg);
    cvtColor(hueVisImg, hueVisImg, CV_HSV2RGB);

    // visualise chrominance for debugging
    static Mat neutralImg(srcImgSize, CV_8UC1, Scalar(128));
    static Mat chromaRedVis[] = {neutralImg, chromaRedImg, neutralImg};
    static Mat chromaBlueVis[] = {neutralImg, neutralImg, chromaBlueImg};

    merge(chromaRedVis, 3, chromaRedVisImg);
    cvtColor(chromaRedVisImg, chromaRedVisImg, CV_YCrCb2BGR);
    merge(chromaBlueVis, 3, chromaBlueVisImg);
    cvtColor(chromaBlueVisImg, chromaBlueVisImg, CV_YCrCb2BGR);

    // set mask ROI (HSV)
    static Mat maskImg(srcImgSize, CV_8UC1);
    inRange(cHSVImg,
            Scalar(0, minSaturation, minValue),
            Scalar(180, maxSaturation, maxValue),
            maskImg);

    // set Mask ROI (YCbCr)
    static Mat maskChromaImg(srcImgSize, CV_8UC1);
    inRange(cYCrCbImg,
            Scalar(0, minChromaRed, minChromaBlue),
            Scalar(255, maxChromaRed, maxChromaBlue),
            maskChromaImg);
    //erode(maskChromaImg, maskChromaImg, Mat(), Point(-1, -1), 2);
    //dilate(maskChromaImg, maskChromaImg, Mat(), Point(-1, -1), 2);

    morphologyEx(maskChromaImg, maskChromaImg, MORPH_CLOSE, Mat());
    morphologyEx(maskChromaImg, maskChromaImg, MORPH_OPEN, Mat());

    // merge mask images and prepare histogram
    bitwise_and(maskImg, maskChromaImg, maskImg, MatND());

    // Histogram properties ------------------
    static MatND hist;
    const int hBins = 30; // no. of hue bins
    const int histSize[] = {hBins};
    const float hRanges[] = {0, 180}; // OpenCV implements hue values from 0 to 180
    const float* histRanges[] = {hRanges};
    const int channels[] = {0};

    // Calculate histogram if last camshift failed
    qDebug() << "got here";
    static Mat hueImgRoi, maskImgRoi;
    hueImgRoi = hueImg(srcRoi);
    maskImgRoi = maskImg(srcRoi);
    qDebug() << "woohoo";
    if (histCalibrate) {
        histCalibrate = false;
        // Calculate Histogram ------------------
        qDebug() << "about to calc hist";
        calcHist(&hueImgRoi,// array of source images
                 1,         // number of source images
                 channels,  // list of channels
                 maskImgRoi,// image mask
                 hist,      // output histogram
                 1,         // histogram dimensionality - just hue
                 histSize,  // array containing hist sizes
                 histRanges,// arrays containing bin boundaries
                 true,      // uniform histogram
                 false);    // clear histogram from beginning
        qDebug() << "it was him!";
    }

    // Calculate Back Projection ------------------
    static double maxVal = 0;
    minMaxLoc(hist, 0, &maxVal, 0, 0);
    double scaleHist = maxVal ? 255.0/maxVal : 0;

    calcBackProject(&hueImg,                 // array of source images
                    1,                       // no. of source images
                    channels,                // list of channels
                    hist,                    // destination backproject array
                    backProjImg,             // output image of back projection
                    histRanges,              // array containing bin boundaries
                    scaleHist,               // scale factor to improve contrast
                    true);                   // uniform histogram

    bitwise_and(backProjImg, maskImg, backProjImg, MatND());

    // show back projection for debugging / parameter tweaking
//    static Mat backProjImg3[] = {backProjImg, backProjImg, backProjImg};
//    merge(backProjImg3, 3, backProjGrayImg);
    static Mat backProjImg3[] = {maskImg, maskImg, maskImg};
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
    // camshift reports success as long as ROI is at most half the size
    // of the input image, and at least 1 20th the size of the input image.
    if (tmpRoi.area() >= srcImgSize.area()/768 && tmpRoi.area() <= srcImgSize.area()/2) {
        int newTLx = tmpRoi.x < 0 ? 0 : tmpRoi.x;
        int newTLy = tmpRoi.y < 0 ? 0 : tmpRoi.y;
        int newBRx = tmpRoi.br().x > srcImg.cols ? srcImg.cols : tmpRoi.br().x;
        int newBRy = tmpRoi.br().y > srcImg.rows ? srcImg.rows : tmpRoi.br().y;
        srcRoi = Rect(Point(newTLx, newTLy), Point(newBRx, newBRy));
        return true;
    } else {
        histCalibrate = true;
        return false;
    }
}
