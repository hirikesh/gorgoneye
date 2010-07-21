#include <QtGui/QApplication>
#include "geyedetector.h"
#include "geyetracker.h"
#include "geyeview.h"
#include "geyeui.h"
#include "geye.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /* geyeview - gorgoneye tracking view */
    //geyeview w;

    /* geyeui - gorgoneye user interface */
    // geyeui w;

    GEyeTracker w;

    w.show();
    return a.exec();


    // create video-from-webcam capture object
    VideoCapture ge_capture(0);

    // check that opencv found a webcam
    if(!ge_capture.isOpened()) return 1;

    // create tracker object
    Tracker ge_tracker;

    // loop the loop processing
    while(waitKey(12) != 'q')
    {
        // grab frame and place in buffer
        ge_capture >> ge_tracker.frame;

        // do image processing stuff here
        ge_tracker.track();
    }

    return 0;
}
