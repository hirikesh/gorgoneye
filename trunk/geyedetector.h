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
    GEyeDetector(Mat*);
    GEyeDetector(Mat*, double, int, Size);

    void setScaleFactor(const double&);
    void setMinNeighbours(const int&);
    void setWidth(const int&);
    void setHeight(const int&);
    void setCC(CascadeClassifier*);

    Rect detect();
    double getScale();
    int getMinNeighbours();
    int getWidth();
    int getHeight();

private:
    Rect featureDetect();
    Rect haarDetect();

    Mat* image;
    Rect currROI;

    // used for haarDetect
    bool isTracking;
    bool beginTrack;
    double scaleFactor;
    int minNeighbours;
    Size minSize;
    CascadeClassifier* haar;

    // used for featureDetect
    Mat cHSVImg;
    Mat hueImg;
    Mat maskImg;
    Mat backProjImg;
    MatND hist;

};

#endif // GEYEDETECTOR_H
