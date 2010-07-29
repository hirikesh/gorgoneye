#include <geyedetector.h>
GEyeDetector::GEyeDetector()
{}

GEyeDetector::GEyeDetector(Mat* img, double sf = 1.1, int mn = 4, Size ms = Size(96,132)) :
    image(img),
    scaleFactor(sf),
    minNeighbours(mn),
    minSize(ms)
{}

Rect GEyeDetector::detect()
{
    vector<Rect> rois;

    haar->detectMultiScale(*image,
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
