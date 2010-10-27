#ifndef GAZETRACKER_H
#define GAZETRACKER_H

#include "basetracker.h"
#include "config.h"

class GazeTracker : public BaseTracker
{
public:
    GazeTracker(Store* st);

    void track();

private:
    bool refineEyeRoi(const cv::Mat &eyeImg, cv::Rect &refinedROI);
    int findLeftCorner(const cv::Mat &image, int &offset);
    int findRightCorner(const cv::Mat &image, int &offset);

    int findThreshold(const cv::Mat &gray, const float &percentage);

    int inputTotalCount;
    int inputPerPointCount;
    int timesPerPoint;

    BaseFilter* cannyContourFilter;
    BaseFilter* cannyEdgeFilter;
    BaseFilter* grayscaleFilter;
    BaseFilter* ycbcrFilter;
    BaseFilter* harrisCornerFilter;
    BaseFilter* cornerFilter;

    BaseFilter* equaliseFilter;
    BaseFilter* erodeDilateFilter;

    BaseFilter* doGNormFilter;

    BaseDetector* mLearningDetector;
};

#endif // GAZETRACKER_H
