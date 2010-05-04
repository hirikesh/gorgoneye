#ifndef GEYE_H
#define GEYE_H

#include <cv.h>
#include <highgui.h>
using namespace cv;

// gorgoneye namespace
namespace geye
{
    class Tracker;
}

// Class which does the image processing
// for tracking & gaze estimation.
class Tracker
{
public:
    // constructor and destructor
    Tracker();
    ~Tracker();

    // image data to process
    Mat frame;

    // perform tracking & update results
    void track();

protected:
    // debugging properties
    bool show; // do imshow()
    bool draw_roi_face; // draw face rect
    bool draw_roi_eyes; // draw eyes rects

    // preprocessing properties


    // face tracking properties
#define HAAR_CC_FACE_DEFAULT "c:\\opencv2.0\\data\\haarcascades\\haarcascade_frontalface_default.xml"
#define HAAR_CC_FACE_ALT "c:\\opencv2.0\\data\\haarcascades\\haarcascade_frontalface_alt.xml"
#define HAAR_CC_FACE_ALT2 "c:\\opencv2.0\\data\\haarcascades\\haarcascade_frontalface_alt2.xml"
#define HAAR_CC_FACE_ALT_TREE "c:\\opencv2.0\\data\\haarcascades\\haarcascade_frontalface_alt_tree.xml"
#define HAAR_CC_PROFILE_FACE "c:\\opencv2.0\\data\\haarcascades\\haarcascade_profileface.xml"

    CascadeClassifier haar_cc; // HAAR cascade classification
    double haar_cc_scale; // downsamples image by haar_cc_scale each pass
    int haar_cc_minneighbours; // min. no. of nearby matches req. to give positive
    int haar_cc_minwidth; // min. size of faces in pixels
    int haar_cc_minheight; // min. size of faces in pixels

    // face-to-eyes processing properties


    // eye tracking properties


    // eye-to-gaze processing properties


    // gaze estimation properties


    // postprocessing properties


private:
    // tracking variables
    Rect  roi_face; // face region
    Rect  roi_leye; // left eye region
    Rect  roi_reye; // right eye region
    Point poi_gaze; // gaze point

    // debugging functions
    void drawroiface();
    void drawroieyes();

    // main tracking functions
    void trackface();
    void trackeyes();
    void trackgaze();

    // preprocessing & postprocessing
    void runpreproc(); // initial preprocessing
    void runf2eproc(); // face-to-eyes processing
    void rune2gproc(); // eyes-to-gaze processing
    void runposproc(); // final postprocessing
};

#endif // GEYE_H
