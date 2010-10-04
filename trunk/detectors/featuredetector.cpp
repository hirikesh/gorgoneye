#include <cv.h>
#include "featuredetector.h"

using namespace cv;

FeatureDetector::FeatureDetector(const int type,
                                 int minh, int maxh,
                                 int mins, int maxs,
                                 int minv, int maxv,
                                 int mincb, int maxcb,
                                 int mincr, int maxcr) :
    BaseDetector(type, "CAMShift"),
    histCalibrate(true),
    minHue(minh),
    maxHue(maxh),
    minSaturation(mins),
    maxSaturation(maxs),
    minValue(minv),
    maxValue(maxv),
    minChromaBlue(mincb),
    maxChromaBlue(maxcb),
    minChromaRed(mincr),
    maxChromaRed(maxcr),
    enBackProjImg(false),
    enHueVisImg(false),
    enChromaRedVisImg(false),
    enChromaBlueVisImg(false)
{
    params.push_back(new RangeParam<int>("Min. Hue", Param::RANGE, &minHue, 0, 180, 5));
    params.push_back(new RangeParam<int>("Max. Hue", Param::RANGE, &maxHue, 0, 180, 5));
    params.push_back(new RangeParam<int>("Min. Saturation", Param::RANGE, &minSaturation, 0, 255, 5));
    params.push_back(new RangeParam<int>("Max. Saturation", Param::RANGE, &maxSaturation, 0, 255, 5));
    params.push_back(new RangeParam<int>("Min. Value", Param::RANGE, &minValue, 0, 255, 5));
    params.push_back(new RangeParam<int>("Max. Value", Param::RANGE, &maxValue, 0, 255, 5));
    params.push_back(new RangeParam<int>("Min. Cb", Param::RANGE, &minChromaBlue, 0, 255, 5));
    params.push_back(new RangeParam<int>("Max. Cb", Param::RANGE, &maxChromaBlue, 0, 255, 5));
    params.push_back(new RangeParam<int>("Min. Cr", Param::RANGE, &minChromaRed, 0, 255, 5));
    params.push_back(new RangeParam<int>("Max. Cr", Param::RANGE, &maxChromaRed, 0, 255, 5));
    params.push_back(new ImageModeParam("Back Projected Image", &enBackProjImg, &backProjGrayImg));
    params.push_back(new ImageModeParam("Hue Visualisation Image", &enHueVisImg, &hueVisImg));
    params.push_back(new ImageModeParam("Cr Component Image", &enChromaRedVisImg, &chromaRedVisImg));
    params.push_back(new ImageModeParam("Cb Component Image", &enChromaBlueVisImg, &chromaBlueVisImg));
}

bool FeatureDetector::locate(const Mat& srcImg, Rect& srcRoi)
{
    Size srcImgSize = srcImg.size();

    // Extract Hue Info
    Mat cHSVImg(srcImgSize, CV_8UC3);
    Mat hueImg(srcImgSize, CV_8UC1);
    Mat satImg(srcImgSize, CV_8UC1);
    Mat valImg(srcImgSize, CV_8UC1);
    Mat cHSVChannels[] = {hueImg, satImg, valImg};

    cvtColor(srcImg, cHSVImg, CV_BGR2HSV);
    split(cHSVImg, cHSVChannels);

    // Extract Chrominance info
    Mat cYCrCbImg(srcImgSize, CV_8UC3);
    Mat lumaImg(srcImgSize, CV_8UC1);
    Mat chromaRedImg(srcImgSize, CV_8UC1);
    Mat chromaBlueImg(srcImgSize, CV_8UC1);
    Mat cYCrCbChannels[] = {lumaImg, chromaRedImg, chromaBlueImg};

    cvtColor(srcImg, cYCrCbImg, CV_BGR2YCrCb);
    split(cYCrCbImg, cYCrCbChannels);

    // visualise Hue for debugging
    if(enHueVisImg) {
        Mat emptyImg(srcImgSize, CV_8UC1, Scalar(255));
        Mat hueVis[] = {hueImg, emptyImg, emptyImg};
        merge(hueVis, 3, hueVisImg);
        cvtColor(hueVisImg, hueVisImg, CV_HSV2RGB);
    }

    // visualise chrominance for debugging
    if(enChromaRedVisImg || enChromaBlueVisImg) {
        Mat neutralImg(srcImgSize, CV_8UC1, Scalar(128));
        if(enChromaRedVisImg) {
            Mat chromaRedVis[] = {neutralImg, chromaRedImg, neutralImg};
            merge(chromaRedVis, 3, chromaRedVisImg);
            cvtColor(chromaRedVisImg, chromaRedVisImg, CV_YCrCb2BGR);
        }
        if(enChromaBlueVisImg) {
            Mat chromaBlueVis[] = {neutralImg, neutralImg, chromaBlueImg};
            merge(chromaBlueVis, 3, chromaBlueVisImg);
            cvtColor(chromaBlueVisImg, chromaBlueVisImg, CV_YCrCb2BGR);
        }
    }

    // set mask ROI (HSV)
    Mat maskImg(srcImgSize, CV_8UC1);
    inRange(cHSVImg,
            Scalar(minHue, minSaturation, minValue),
            Scalar(maxHue, maxSaturation, maxValue),
            maskImg);

    // set Mask ROI (YCbCr)
    Mat maskChromaImg(srcImgSize, CV_8UC1);
    inRange(cYCrCbImg,
            Scalar(0, minChromaRed, minChromaBlue),
            Scalar(255, maxChromaRed, maxChromaBlue),
            maskChromaImg);

    //erode(maskChromaImg, maskChromaImg, Mat(), Point(-1, -1), 2);
    //dilate(maskChromaImg, maskChromaImg, Mat(), Point(-1, -1), 2);

    morphologyEx(maskChromaImg, maskChromaImg, MORPH_CLOSE, Mat());
    morphologyEx(maskChromaImg, maskChromaImg, MORPH_CLOSE, Mat());
    morphologyEx(maskChromaImg, maskChromaImg, MORPH_OPEN, Mat());
    morphologyEx(maskChromaImg, maskChromaImg, MORPH_OPEN, Mat());

    // merge mask images and prepare histogram
    bitwise_and(maskImg, maskChromaImg, maskImg, MatND());

//    Mat testImg = maskImg.clone();
//    vector<vector<Point> > contours;
//    vector<Vec4i> hierarchy;
//    findContours( testImg, contours, hierarchy,
//         CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
//
//    Mat ellipsedImg = Mat::zeros(testImg.rows, testImg.cols, CV_8UC3);
//    Mat* contourPath;
//    for(unsigned int i = 0; i < contours.size(); i++)
//    {
//        if (contours[i].size() > 10)
//        {
//            contourPath = new Mat(contours[i]);
//            RotatedRect ellipsed = fitEllipse(*contourPath);
//            ellipse(ellipsedImg, ellipsed, Scalar(128,0,0));
//            delete contourPath;
//        }
//    }
//    imshow("Ellipse", ellipsedImg);
    // Mat dst = Mat::zeros(maskImg.rows, maskImg.cols, CV_8UC3);
    // iterate through all the top-level contours,
    // draw each connected component with its own random color
    //int idx = 0;
//    for( ; idx >= 0; idx = hierarchy[idx][0] )
//    {

//        //Scalar color( rand()&255, rand()&255, rand()&255 );
//        //drawContours( dst, contours, idx, color, 1, 8, hierarchy );
//    }
    //( "Components", dst );

    // Histogram properties ------------------
    const int hBins = 30; // no. of hue bins
    const int histSize[] = {hBins};
    const float hRanges[] = {0, 180}; // OpenCV implements hue values from 0 to 180
    const float* histRanges[] = {hRanges};
    const int channels[] = {0};

    if (histCalibrate) {
        // Calculate histogram if last camshift failed
        Mat hueImgRoi = hueImg(srcRoi);
        Mat maskImgRoi = maskImg(srcRoi);

        // Calculate Histogram ------------------
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

        histCalibrate = false;
    }

    // Calculate Back Projection ------------------
    double maxVal = 0;
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
    if(enBackProjImg) {
//        Mat backProjImg3[] = {backProjImg, backProjImg, backProjImg};
        Mat backProjImg3[] = {maskImg, maskImg, maskImg};
//        Mat backProjImg3[] = {maskChromaImg, maskChromaImg, maskChromaImg};
        merge(backProjImg3, 3, backProjGrayImg);
    }

    // CAMShift Calculations ---------
    // Search Window begins at region of interest determined using Haar
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
    if (tmpRoi.area() >= srcImgSize.area()/768 && tmpRoi.area() <= srcImgSize.area()/2) {
        int newTLx = tmpRoi.x < 0 ? 0 : tmpRoi.x;
        int newTLy = tmpRoi.y < 0 ? 0 : tmpRoi.y;
        int newBRx = tmpRoi.br().x > srcImg.cols ? srcImg.cols - 1 : tmpRoi.br().x;
        int newBRy = tmpRoi.br().y > srcImg.rows ? srcImg.rows - 1: tmpRoi.br().y;
        srcRoi = Rect(Point(newTLx, newTLy), Point(newBRx, newBRy));
        return true;
    } else {
        histCalibrate = true;
        return false;
    }
}
