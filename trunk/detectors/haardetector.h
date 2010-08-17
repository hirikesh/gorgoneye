/* HaarDetector:
   Concreate BaseDetector class using
   HAAR classification. */

#include <cv.h>
#include "basedetector.h"
#include "parameter.h"

#ifndef HAAR_H
#define HAAR_H

class HaarDetector : public BaseDetector
{
public:
    HaarDetector(string td, double sf, int mn, bool fg, Size ms);

    bool locate(const Mat& srcImg, Rect& srcRoi);
    int getMinN();
private:
    CascadeClassifier* cc;

    string trainingData;
    double scaleFactor;
    int minNeighbours;
    int flags;
    Size minSize;
};

#endif // HAAR_H
