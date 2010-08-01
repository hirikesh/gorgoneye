#include <geyedetector.h>
GEyeDetector::GEyeDetector()
{}

GEyeDetector::GEyeDetector(Mat* img, double sf, int mn, Size ms) :
    image(img),
    scaleFactor(sf),
    minNeighbours(mn),
    minSize(ms)
{
   // init Camshift
    hueImg = Mat(image->rows, image->cols, CV_8UC1);
    maskImg = Mat(image->rows, image->cols, CV_8UC1, Scalar(0));
    isTracking = false;
    beginTrack= false;
}

Rect GEyeDetector::detect()
{
    // helper function for detection
    //return haarDetect();
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
    // TO DO: filter HSV image
    cvtColor(*image, cHSVImg, CV_BGR2HSV);
    cvtColor(*image, *image, CV_BGR2RGB);
    // Extracts hue channel from hsv multi-channel image.
    // mixChannels requires pre-allocated matrices.
    int fromTo[] = {0,0};
    mixChannels(&cHSVImg, 1, &hueImg, 1, fromTo, 1);

    // visualise Hue for debugging
    Mat hueVisImg(hueImg.rows, hueImg.cols, CV_8UC3);
    Mat satImg(hueImg.rows, hueImg.cols, CV_8UC1, Scalar(255));
    Mat valImg(hueImg.rows, hueImg.cols, CV_8UC1, Scalar(255));

    Mat hueVis[] = {hueImg, satImg, valImg};
    merge(hueVis, 3, hueVisImg);
    cvtColor(hueVisImg, hueVisImg, CV_HSV2RGB);
    imshow("Hue Visualisation", hueVisImg);


    // set mask ROI
    Mat maskImg(cHSVImg.rows, cHSVImg.cols, CV_8UC1, Scalar(0));
    Mat maskROI(maskImg, currROI);
    maskROI = Scalar(255); // alters original image data
    imshow("Mask", maskImg);

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
        calcHist(&cHSVImg,  // array of source images
                 1,         // number of source images
                 channels,  // list of channels
                 maskImg,   // image mask
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
    calcBackProject(&cHSVImg,                // array of source images
                    1,                       // no. of source images
                    channels,                // list of channels
                    hist,                    // destination backproject array
                    backProjImg,             // output image of back projection
                    histRanges,              // array containing bin boundaries
                    maxVal? 255.0/maxVal : 0,// scale factor
                    true);                   // uniform histogram
    maskImg = Scalar(255); // reset mask
    bitwise_and(backProjImg, maskImg, backProjImg, Mat());

    // show back projection - debugging
    imshow("Back Projected Image", backProjImg);

    // CAMShift Calculations ---------
    // Search Window begins at region of interest determined using Haar
    // The algorithm will auto increase search window
/*
    RotatedRect rotTemp;
    rotTemp = CamShift(backProjImg, // back projected image
                       currROI,     // initial search window
                       TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 10, 1));

    // TODO: Update face location and angle (if required).
    currROI = rotTemp.boundingRect();

    // Modify bounds on camshift rectangle
    if (currROI.x + currROI.width > image->cols)
        currROI.width = image->cols - currROI.x;
    if (currROI.y + currROI.height > image->rows)
        currROI.height = image->rows - currROI.y;
    if (currROI.x < 0)
        currROI.x = 0;
    if (currROI.x > image->cols)
        currROI.x = image->cols;
    if (currROI.y < 0)
        currROI.y = 0;
    if (currROI.y > image->rows)
        currROI.y = image->rows;
    return currROI;
*/
    Rect temp(-1, -1, 0, 0);
    return temp;
}

void GEyeDetector::setScaleFactor(double sf)
{
    scaleFactor = sf;
}
void GEyeDetector::setMinNeighbours(int mn)
{
    minNeighbours = mn;
}
void GEyeDetector::setWidth(int w)
{
    minSize.width = w;
}
void GEyeDetector::setHeight(int h)
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
