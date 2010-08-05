/* Haar:
   Concreate Detector class using
   HAAR classification. */

#include "store.h"
#include "detector.h"

#ifndef HAAR_H
#define HAAR_H

class Haar : public Detector
{
public:
    Haar(Store* st);

    void locate();

private:
    // configuration parameters
    // as an object should go here.
};

#endif // HAAR_H
