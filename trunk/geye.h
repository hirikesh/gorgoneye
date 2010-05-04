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

    // entry for image data
    Mat frame;

    // perform tracking & update results
    void track();

protected:
    // debugging properties
    bool show; // do imshow()
    bool draw_roi_face; // draw face rect
    bool draw_roi_eyes; // draw eyes rects
    Scalar roi_colour_face; // face rect colour
    Scalar roi_colour_eyes; // eyes rect colour

    // preprocessing properties


    // face tracking properties
    #define HAAR_CC_FACE_DEFAULT "c:\\opencv2.0\\data\\haarcascades\\haarcascade_frontalface_default.xml"
    #define HAAR_CC_FACE_ALT "c:\\opencv2.0\\data\\haarcascades\\haarcascade_frontalface_alt.xml"
    #define HAAR_CC_FACE_ALT2 "c:\\opencv2.0\\data\\haarcascades\\haarcascade_frontalface_alt2.xml"
    #define HAAR_CC_FACE_ALT_TREE "c:\\opencv2.0\\data\\haarcascades\\haarcascade_frontalface_alt_tree.xml"
    #define HAAR_CC_FACE_PROFILE "c:\\opencv2.0\\data\\haarcascades\\haarcascade_profileface.xml"

    CascadeClassifier haar_cc_face; // HAAR cascade classification
    double haar_cc_scale_face; // downsamples image by haar_cc_scale each pass
    int haar_cc_minneighbours_face; // min. no. of nearby matches req. to give positive
    int haar_cc_minwidth_face; // min. size of faces in pixels
    int haar_cc_minheight_face; // min. size of faces in pixels

    // face-to-eyes processing properties


    // eye tracking properties
    #define HAAR_CC_EYE "c:\\opencv2.0\\data\\haarcascades\\haarcascade_eye.xml"
    #define HAAR_CC_LEYE_2SPLITS "c:\\opencv2.0\\data\\haarcascades\\haarcascade_lefteye_2splits.xml"
    #define HAAR_CC_REYE_2SPLITS "c:\\opencv2.0\\data\\haarcascades\\haarcascade_righteye_2splits.xml"
    #define HAAR_CC_LEYE_MSC "c:\\opencv2.0\\data\\haarcascades\\haarcascade_mcs_lefteye.xml"
    #define HAAR_CC_REYE_MSC "c:\\opencv2.0\\data\\haarcascades\\haarcascade_mcs_righteye.xml"

    CascadeClassifier haar_cc_leye; // HAAR cascade classification
    CascadeClassifier haar_cc_reye; // HAAR cascade classification
    double haar_cc_scale_eyes; // downsamples image by haar_cc_scale each pass
    int haar_cc_minneighbours_eyes; // min. no. of nearby matches req. to give positive
    int haar_cc_minwidth_eyes; // min. size of faces in pixels
    int haar_cc_minheight_eyes; // min. size of faces in pixels

    // eye-to-gaze processing properties


    // gaze estimation properties


    // postprocessing properties


private:
    // tracking variables
    Rect  roi_ignore; // ignored region
    Point poi_ignore; // ignored point

    Rect  roi_face; // face region
    Mat   frame_roi_face;
    Mat   frame_roi_face_left;
    Mat   frame_roi_face_right;

    Rect  roi_leye; // left eye region
    Mat   frame_roi_leye;
    Rect  roi_reye; // right eye region
    Mat   frame_roi_reye;

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
