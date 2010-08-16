#include <cv.h>
#include "detector.h"
#include "haar.h"
#include <QDebug>
using namespace cv;

Haar::Haar(char* td, double sf, int mn, int fg, Size ms) : Detector(),
    trainingData(td),
    scaleFactor(sf),
    minNeighbours(mn),
    flags(fg),
    minSize(ms)
{
    cc = new CascadeClassifier(trainingData);

    // Create Parameters that will be used
    //params[0] = new RangeParam(sf1, &scaleFactor, 0, 2, 1); needs to be double
    params.push_back(new RangeParam("minNeighbours", &minNeighbours, 1, 5, 1));
    params.push_back(new RangeParam("minWidth", &minSize.width, 20, 200, 20));
    params.push_back(new RangeParam("minHeight", &minSize.height, 20, 200, 20));
    //params[4] = new ModeParam("Canny Pruning", , false);

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
