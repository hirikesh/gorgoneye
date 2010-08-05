/* Detector:
   Polymorphic class to support multiple
   tracking algorithms under common interface.
   Follows the Strategy design pattern. */

#include "store.h"

#ifndef DETECTOR_H
#define DETECTOR_H

class Detector
{
public:
    Detector(Store* st);

    virtual void locate();

private:
    Store* store;
};

#endif // DETECTOR_H
