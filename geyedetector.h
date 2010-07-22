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
#define HAAR_CC_FACE_DEFAULT "c:\\opencv2.1\\data\\haarcascades\\haarcascade_frontalface_default.xml"
public:
    GEyeDetector();
    GEyeDetector(Mat* img, double sf, int mn, Size ms);

    void setScaleFactor(double);
    void setMinNeighbours(int);
    void setMinSize(Size);

    Rect detect();
    Size size();
    double scale();
    int minneighbours();
private:

    Mat image;
    Rect roi;
    CascadeClassifier cc;
    double scaleFactor;
    int minNeighbours;
    Size minSize;
};

#endif // GEYEDETECTOR_H
