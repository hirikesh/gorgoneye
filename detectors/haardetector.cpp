#include <cv.h>
#include "haardetector.h"
#include "parameter.h"

using namespace cv;

HaarDetector::HaarDetector(Store *st, string td, double sf, int mn, bool fg, Size ms) :
    BaseDetector(st, "Haar"),
    scaleFactor(sf),
    minNeighbours(mn),
    flags(fg),
    minSize(ms),
    cClassifier(td)
{
    // Create Parameters that will be used
    _params.push_back(new RangeParam<double>("Scale Factor", Param::RANGE_DBL, &scaleFactor, 1.05, 2, 0.05));
    _params.push_back(new RangeParam<int>("Min. Neighbours", Param::RANGE, &minNeighbours, 1, 7, 1));
    _params.push_back(new RangeParam<int>("Min. Search Width", Param::RANGE, &minSize.width, 8, 200, 20));
    _params.push_back(new RangeParam<int>("Min. Search Height", Param::RANGE, &minSize.height, 8, 200, 20));
    _params.push_back(new ModeParam("Enable Canny Pruning", &flags, false));
}

bool HaarDetector::locate(const Mat& srcImg, const Mat& srcMsk, Rect& srcRoi)
{
    vector<Rect> rois;
    cClassifier.detectMultiScale(srcImg,
                                 rois,
                                 scaleFactor,
                                 minNeighbours,
                                 flags,
                                 minSize);

    if(rois.size()) {
        srcRoi = rois[0];
        for(int i = 1; i < rois.size(); i++)
            if(rois[i].area() > srcRoi.area()) srcRoi = rois[i];
        return true;
    } else {
        return false;
    }
}
