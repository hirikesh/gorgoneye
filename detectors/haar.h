/* Haar:
   Concreate Detector class using
   HAAR classification. */

#include "detector.h"

#ifndef HAAR_H
#define HAAR_H

class Haar : public Detector
{
public:
//    Haar();

    void locate();

private:
    // configuration parameters
    // as an object should go here.
};

#endif // HAAR_H
