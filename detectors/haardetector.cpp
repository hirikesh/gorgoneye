#include <cv.h>
#include "haardetector.h"

using namespace cv;

HaarDetector::HaarDetector(const int type, string td, double sf, int mn, bool fg, Size ms) :
    BaseDetector(type, "Haar"),
    trainingData(td),
    scaleFactor(sf),
    minNeighbours(mn),
    flags(fg),
    minSize(ms)
{
    // Create Parameters that will be used
    params.push_back(new RangeParam<double>("Scale Factor", Param::RANGE_DBL, &scaleFactor, 1.05, 2, 0.05));
    params.push_back(new RangeParam<int>("Min. Neighbours", Param::RANGE, &minNeighbours, 1, 7, 1));
    params.push_back(new RangeParam<int>("Min. Search Width", Param::RANGE, &minSize.width, 8, 200, 20));
    params.push_back(new RangeParam<int>("Min. Search Height", Param::RANGE, &minSize.height, 8, 200, 20));
    params.push_back(new ModeParam("Enable Canny Pruning", &flags, false));
}

bool HaarDetector::locate(const Mat& srcImg, Rect& srcRoi)
{
    static CascadeClassifier cc(trainingData);
    static vector<Rect> rois;

    cc.detectMultiScale(srcImg,
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