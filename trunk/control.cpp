#include "control.h"
#include "store.h"
#include "tracker.h"
#include "trackers/face.h"

Control::Control()
{
    store = new Store();
    faceTracker = new Face(store);
}


void Control::start()
{

}
