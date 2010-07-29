#ifndef GEYEDETECTOR_H
#define GEYEDETECTOR_H


#include <cv.h>
#include <highgui.h>
using namespace cv;

namespace geye {
    class GEyeDetector;
}

class GEyeDetector
{
public:
    GEyeDetector();
    GEyeDetector(Mat*, double, int, Size);

    void setScaleFactor(double);
    void setMinNeighbours(int);
    void setMinSize(Size);
    void setCC(CascadeClassifier*);

    Rect detect();

    bool isTracking;
    double getScale();
    int getMinNeighbours();
    Size getSize();

private:
    Rect featureDetect();
    Rect haarDetect();

    Mat* image;
    Rect currROI;

    // used for haarDetect
    double scaleFactor;
    int minNeighbours;
    Size minSize;
    CascadeClassifier* haar;

    // used for featureDetect
    Mat cHSVImg;
    //Mat hueImg;
    //Mat maskImg;
    Mat probImg;
    MatND hist;

};

#endif // GEYEDETECTOR_H
