#include <cv.h>
#include "detector.h"
#include "haar.h"

using namespace cv;

Haar::Haar(char* td, double sf, int mn, int fg, Size ms) : Detector(),
    trainingData(td),
    scaleFactor(sf),
    minNeighbours(mn),
    flags(fg),
    minSize(ms)
{
    cc = new CascadeClassifier(trainingData);

    // create vector of params here
}

bool Haar::locate(const Mat& srcImg, Rect& srcRoi)
{
    vector<Rect> rois;
    cc->detectMultiScale(srcImg,
                           rois,
                           scaleFactor,
                           minNeighbours,
                           flags,
                           minSize);

    if(rois.size())
    {
        srcRoi = rois[0];
        return true;
    }
    else
    {
        return false;
    }
}
