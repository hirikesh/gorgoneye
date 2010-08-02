#include <geyedetector.h>
GEyeDetector::GEyeDetector()
{}

GEyeDetector::GEyeDetector(Mat* img) :
        image(img),
        isTracking(false),
        beginTrack(false),
        scaleFactor(1.1),
        minNeighbours(3),
        minSize(Size(80,80))
{
    int w = img->cols;
    int h = img->rows;
    maskImg = Mat(h, w, CV_8UC1);
    hueImg = Mat(h, w, CV_8UC1);

}

GEyeDetector::GEyeDetector(Mat* img, double sf, int mn, Size ms) :
    image(img),
    isTracking(false),
    beginTrack(false),
    scaleFactor(sf),
    minNeighbours(mn),
    minSize(ms)
{
    int w = img->cols;
    int h = img->rows;
    maskImg = Mat(h, w, CV_8UC1);
    hueImg = Mat(h, w, CV_8UC1);

}

Rect GEyeDetector::detect()
{
    // helper function for detection
    if(isTracking)
    {
        return featureDetect();
    }
    else
    {
        return haarDetect();
    }
}
Rect GEyeDetector::haarDetect()
{
    vector<Rect> rois;

    cvtColor(*image, *image, CV_BGR2RGB);

    haar->detectMultiScale(*image,
                            rois,
                            scaleFactor,
                            minNeighbours,
                            CV_HAAR_DO_CANNY_PRUNING,
                            minSize);
    if(rois.size() > 0)
    {
        isTracking = true;
        beginTrack = true;
        currROI = rois[0];
        return currROI;
    }
    else

    {
        isTracking = false;
        return Rect(-1, -1, 0, 0);
    }
}
Rect GEyeDetector::featureDetect()
{
    // Extract Hue Info
    cvtColor(*image, cHSVImg, CV_BGR2HSV);
    cvtColor(*image, *image, CV_BGR2RGB);
    Mat satImg(cHSVImg.rows, cHSVImg.cols, CV_8UC1);
    Mat valImg(cHSVImg.rows, cHSVImg.cols, CV_8UC1);
    Mat cHSVChannels[] = {hueImg, satImg, valImg};
    split(cHSVImg, cHSVChannels);

//    // visualise Hue for debugging
//    Mat hueVisImg(cHSVImg.rows, cHSVImg.cols, CV_8UC3);
//    satImg = Scalar(255);
//    valImg = Scalar(255);
//    Mat hueVis[] = {hueImg, satImg, valImg};
//    merge(hueVis, 3, hueVisImg);
//    cvtColor(hueVisImg, hueVisImg, CV_HSV2RGB);
//    imshow("Hue Visualisation", hueVisImg);

    // set mask ROI
    inRange(cHSVImg,
            Scalar(0, 45, 45),
            Scalar(180, 256, 256),
            maskImg);
    Mat maskROI(maskImg, currROI);
    Mat hueImgROI(hueImg, currROI);

    // Histogram properties ------------------
    int hBins = 30; // no. of hue bins
    int histSize[] = {hBins};
    float hRanges[] = {0, 180}; // OpenCV implements hue values from 0 to 180
    const float* histRanges[] = {hRanges};
    int channels[] = {0};

    if (beginTrack)
    {
        beginTrack = false;
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
    //imshow("Filtered Back Projected Image", backProjImg);

    // CAMShift Calculations ---------
    // Search Window begins at region of interest determined using Haar
    // The algorithm will auto increase search window

    RotatedRect rotTemp;
    rotTemp = CamShift(backProjImg, // back projected image
                       currROI,     // initial search window
                       TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 20, 1));

    currROI = rotTemp.boundingRect();

    // Modify bounds on camshift rectangle
    // TODO: A more elegant range check function
    if (currROI.x <= 0)
    {currROI.x = 0;}
    if (currROI.width <= 0)
    {currROI.width = 0;}
    if (currROI.y <= 0)
    {currROI.y = 0;}
    if (currROI.height <= 0)
    {currROI.height = 0;}
    if (currROI.width >= image->cols)
    {currROI.width = image->cols;}
    if (currROI.x >= image->cols)
    {currROI.x = image->cols;}
    if (currROI.y >= image->rows)
    {currROI.y = image->rows;}
    if (currROI.height >= image->rows)
    {currROI.height = image->rows;}
    if (currROI.x + currROI.width >= image->cols)
    {currROI.width = image->cols - currROI.x;}
    if (currROI.y + currROI.height >= image->rows)
    {currROI.height = image->rows - currROI.y;}
    return currROI;
}

void GEyeDetector::setScaleFactor(const double& sf)
{
    scaleFactor = sf;
}
void GEyeDetector::setMinNeighbours(const int& mn)
{
    minNeighbours = mn;
}
void GEyeDetector::setWidth(const int& w)
{
    minSize.width = w;
}
void GEyeDetector::setHeight(const int& h)
{
    minSize.height = h;
}
void GEyeDetector::setCC(CascadeClassifier* cc)
{
    haar = cc;
}

int GEyeDetector::getWidth()
{
    return minSize.width;
}
int GEyeDetector::getHeight()
{
    return minSize.height;
}
double GEyeDetector::getScale()
{
    return scaleFactor;
}
int GEyeDetector::getMinNeighbours()
{
    return minNeighbours;
}
