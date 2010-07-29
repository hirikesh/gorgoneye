#include <geyedetector.h>
GEyeDetector::GEyeDetector()
{}

GEyeDetector::GEyeDetector(Mat* img, double sf, int mn, Size ms) :
    image(img),
    scaleFactor(sf),
    minNeighbours(mn),
    minSize(ms)
{
    isTracking = false;
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
                            NULL,
                            minSize);
    if(rois.size() > 0)
    {
        //isTracking = true;
        currROI = rois[0];
        return currROI;
    }
    else

    {
        //isTracking = false;
        return Rect(-1, -1, 0, 0);
    }
}
Rect GEyeDetector::featureDetect()
{
    // Extract Hue Info
    // TO DO: filter HSV image
    cvtColor(*image, cHSVImg, CV_BGR2HSV);
    // Extracts hue channel from hsv multi-channel image.
    // mixChannels requires pre-allocated matrices.
    Mat hueImg(image->rows, image->cols, CV_8UC1);
    int fromTo[] = {0,0};
    mixChannels(&cHSVImg, 1, &hueImg, 1, fromTo, 1);

    // set mask ROI
    Mat maskImg(image->rows, image->cols, CV_8UC1, Scalar(0));
    Mat maskROI(maskImg, currROI);

    // Histogram Properties ------------------

    int hBins = 15; // no. of hue bins
    int histSize[] = {hBins};
    float hRanges[] = {0, 180}; // OpenCV implements hue values from 0 to 180
    const float* histRanges[] = {hRanges};
    int channels[] = {0};

    // Calculate Histogram ------------------

    calcHist(&hueImg,   // array of source images
             1,         // number of source images
             channels,  // list of channels
             maskROI,   // image mask
             hist,      // output histogram
             1,         // histogram dimensionality - just hue
             histSize,  // array containing hist sizes
             histRanges,// arrays containing bin boundaries
             true,      // uniform histogram
             false);    // clear histogram from beginning

    // Plot Histogram (Incomplete) ----------------
    // COMMENT: Unnecessary for now
    // double maxVal = 0;
    // minMaxLoc(hist, 0, &maxVal, 0, 0);

    // Calculate Back Projection ------------------

    calcBackProject(&hueImg,   // array of source images
                    1,         // no. of source images
                    channels,  // list of channels
                    hist,      // destination backproject array
                    probImg,   // output image of back projection
                    histRanges,// array containing bin boundaries
                    1,         // optional scale factor
                    true);     // uniform histogram
    bitwise_and(probImg, maskROI, probImg, Mat());

    // CAMShift Calculations ---------
    // Search Window initially starts at region of interest determined initially
    // using Haar Classifiers.
    // The algorithm should automatically increase the search window
    RotatedRect rotTemp;
    rotTemp = CamShift(probImg,
                       currROI,
                       TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 10, 1));

    // TODO: Update face location and angle.
    currROI = rotTemp.boundingRect();
    return currROI;
}

void GEyeDetector::setScaleFactor(double sf)
{
    scaleFactor = sf;
}
void GEyeDetector::setMinNeighbours(int mn)
{
    minNeighbours = mn;
}
void GEyeDetector::setMinSize(Size ms)
{
    minSize = ms;
}
void GEyeDetector::setCC(CascadeClassifier* cc)
{
    haar = cc;
}

Size GEyeDetector::getSize()
{
    return minSize;
}
double GEyeDetector::getScale()
{
    return scaleFactor;
}
int GEyeDetector::getMinNeighbours()
{
    return minNeighbours;
}
