#include "control.h"
#include "store.h"
#include "tracker.h"
#include "trackers/face.h"

Control::Control() :
{
    store = &Store();
    faceTracker = &Face(store);
}


void Control::start()
{

}
