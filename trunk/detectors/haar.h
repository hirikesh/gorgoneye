/* Haar:
   Concreate Detector class using
   HAAR classification. */

#include <cv.h>
#include "detector.h"

#ifndef HAAR_H
#define HAAR_H

class Haar : public Detector
{
public:
    Haar(char* td, double sf, int mn, int fg, Size ms);

    bool locate(const Mat& srcImg, Rect& srcRoi);

private:
    CascadeClassifier* cc;

    char* trainingData;
    double scaleFactor;
    int minNeighbours;
    int flags;
    Size minSize;
};

#endif // HAAR_H
