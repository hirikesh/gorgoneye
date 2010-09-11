#include <cv.h>
#include "haardetector.h"

using namespace cv;

HaarDetector::HaarDetector(string td, double sf, int mn, bool fg, Size ms) :
    BaseDetector("Haar"),
    trainingData(td),
    scaleFactor(sf),
    minNeighbours(mn),
    flags(fg),
    minSize(ms)
{
    // Create Parameters that will be used
    params.push_back(new RangeParam<double>("Scale Factor", Param::RANGE_DBL, &scaleFactor, 1, 2, 0.05));
    params.push_back(new RangeParam<int>("Min. Neighbours", Param::RANGE, &minNeighbours, 1, 5, 1));
    params.push_back(new RangeParam<int>("Min. Search Width", Param::RANGE, &minSize.width, 20, 200, 20));
    params.push_back(new RangeParam<int>("Min. Search Height", Param::RANGE, &minSize.height, 20, 200, 20));
    params.push_back(new ModeParam("Enable Canny Pruning", &flags, false));
}

bool HaarDetector::locate(const Mat& srcImg, Rect& srcRoi)
{
    static Mat haarImg(srcImg.rows, srcImg.cols, CV_8UC3);
    cvtColor(srcImg, haarImg, CV_BGR2RGB);
    static CascadeClassifier cc(trainingData);
    static vector<Rect> rois;

    cc.detectMultiScale(haarImg,
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
