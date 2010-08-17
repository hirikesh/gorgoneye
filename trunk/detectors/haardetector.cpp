#include <cv.h>
#include "haardetector.h"

using namespace cv;

HaarDetector::HaarDetector(string td, double sf, int mn, bool fg, Size ms) : BaseDetector(),
    trainingData(td),
    scaleFactor(sf),
    minNeighbours(mn),
    flags(fg),
    minSize(ms)
{
    cc = new CascadeClassifier(trainingData);

    // Create Parameters that will be used
    params.push_back(new RangeParam<double>("scaleFactor", Param::RANGE_DBL, &scaleFactor, 0, 2, 1));
    params.push_back(new RangeParam<int>("minNeighbours", Param::RANGE, &minNeighbours, 1, 5, 1));
    params.push_back(new RangeParam<int>("minWidth", Param::RANGE, &minSize.width, 20, 200, 20));
    params.push_back(new RangeParam<int>("minHeight", Param::RANGE, &minSize.height, 20, 200, 20));
    params.push_back(new ModeParam("Canny Pruning",&fg, false));

}

bool HaarDetector::locate(const Mat& srcImg, Rect& srcRoi)
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