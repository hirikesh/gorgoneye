#ifndef GAZETRACKER_H
#define GAZETRACKER_H

#include "basetracker.h"

class GazeTracker : public BaseTracker
{
public:
    GazeTracker(Store* st);

    void track();

private:
    BaseFilter* cannyEdgeFilter;
    BaseFilter* grayscaleFilter;
};

#endif // GAZETRACKER_H
