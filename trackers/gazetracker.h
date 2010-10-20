#ifndef GAZETRACKER_H
#define GAZETRACKER_H

#include "basetracker.h"

class GazeTracker : public BaseTracker
{
public:
    GazeTracker(Store* st);

    void track();

private:
    int inputTotalCount;
    int inputPerPointCount;

    BaseFilter* cannyEdgeFilter;
    BaseFilter* grayscaleFilter;
    BaseFilter* ycbcrFilter;
    BaseFilter* harrisCornerFilter;
    BaseFilter* equaliseFilter;
    BaseFilter* erodeDilateFilter;

    BaseDetector* mLearningDetector;
};

#endif // GAZETRACKER_H
