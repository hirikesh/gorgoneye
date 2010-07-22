#include <geyedetector.h>
GEyeDetector::GEyeDetector()
{
    cc = CascadeClassifier(HAAR_CC_FACE_DEFAULT);
}

GEyeDetector::GEyeDetector(Mat* img, double sf = 1.1, int mn = 4, Size ms = Size(96,132))
{
    cc = CascadeClassifier(HAAR_CC_FACE_DEFAULT);
    image = *img;
    scaleFactor = sf;
    minNeighbours = mn;
    minSize = ms;
}

Rect GEyeDetector::detect()
{
    vector<Rect> rois;

    cc.detectMultiScale(image,
                        rois,
                        scaleFactor,
                        minNeighbours,
                        NULL,
                        minSize);
    if(rois.size() > 0)
    {
        return rois[0];
    }
    else
    {
        return Rect(-1, -1, 0, 0);
    }
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

Size GEyeDetector::size()
{
    return minSize;
}

double GEyeDetector::scale()
{
    return scaleFactor;
}

int GEyeDetector::minneighbours()
{
    return minNeighbours;
}
