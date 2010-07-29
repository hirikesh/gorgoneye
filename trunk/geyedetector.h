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
    GEyeDetector(Mat* img, double sf, int mn, Size ms);

    void setScaleFactor(double);
    void setMinNeighbours(int);
    void setMinSize(Size);
    void setCC(CascadeClassifier*);

    Rect detect();
    Size getSize();
    double getScale();
    int getMinNeighbours();
private:

    Rect roi;
    CascadeClassifier* haar;
    Mat* image;
    double scaleFactor;
    int minNeighbours;
    Size minSize;
};

#endif // GEYEDETECTOR_H
