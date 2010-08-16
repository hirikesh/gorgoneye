#include "control.h"
#include "trackers/facetracker.h"

Control::Control()
{
    store = new Store();
    faceTracker = new FaceTracker(store);
}


void Control::start()
{

}
