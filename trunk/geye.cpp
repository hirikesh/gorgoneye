#include <geye.h>

// --------------------------------------------
// constructor(s) & destructor
//
Tracker::Tracker()
{
    // initialise important variables
    frame = Mat();

    // debugging properties
    show = true;
    draw_roi_face = true;
    draw_roi_eyes = true;

    // face tracking properties
    haar_cc = CascadeClassifier(HAAR_CC_FACE_DEFAULT);
    haar_cc_scale = 1.1;
    haar_cc_minneighbours = 4;
    haar_cc_minsize = Size(96,132);

    // tracking variables
    roi_face = Rect(Point(-1,-1),Point(-1,-1));
    roi_leye = Rect(Point(-1,-1),Point(-1,-1));
    roi_reye = Rect(Point(-1,-1),Point(-1,-1));
    poi_gaze = Point(-1,-1);
}

Tracker::~Tracker()
{

}

// --------------------------------------------
// public functions (main)
//
void Tracker::track()
{
    // run all methods in sequence
    runpreproc();
    trackface();
    runf2eproc();
    trackeyes();
    rune2gproc();
    trackgaze();
    runposproc();

    // show in cv::namedWindow if set
    if(show)
    {
        // draw ROIs if set
        if(draw_roi_face)
        {
            drawroiface();
        }
        if(draw_roi_eyes)
        {
            drawroieyes();
        }
        // show in window now
        imshow("Capture", frame);
    }

}

// --------------------------------------------
// private functions (debugging)
//
void Tracker::drawroiface()
{
    if(roi_face.area()>0) rectangle(frame, roi_face.tl(), roi_face.br(), CV_RGB(0,200,0));
}

void Tracker::drawroieyes()
{
    if(roi_leye.area()>0) rectangle(frame, roi_leye.tl(), roi_leye.br(), CV_RGB(0,0,200));
    if(roi_reye.area()>0) rectangle(frame, roi_reye.tl(), roi_reye.br(), CV_RGB(0,0,200));
}

// --------------------------------------------
// private functions (tracking)
//
void Tracker::trackface()
{
    // temporary processing variables
    vector<Rect> faces;

    // haar cascade classifier face detection
    haar_cc.detectMultiScale(frame, faces, haar_cc_scale, haar_cc_minneighbours, NULL, haar_cc_minsize);

    // simply store first face found for now
    if(faces.size()>0) roi_face = faces[0];
    else roi_face = Rect(Point(-1,-1),Point(-1,-1));
}

void Tracker::trackeyes()
{

}

void Tracker::trackgaze()
{

}

// --------------------------------------------
// private functions (processing)
//
void Tracker::runpreproc()
{

}

void Tracker::runf2eproc()
{

}

void Tracker::rune2gproc()
{

}

void Tracker::runposproc()
{

}
