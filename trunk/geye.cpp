#include <geye.h>

// --------------------------------------------
// constructor(s) & destructor
//
Tracker::Tracker()
{
    // image data to process
    frame = Mat();

    // debugging properties
    show = true;
    draw_roi_face = true;
    draw_roi_eyes = true;
    roi_colour_face = Scalar(0,128,0);
    roi_colour_eyes = Scalar(0,0,128);

    // face tracking properties
    haar_cc_face = CascadeClassifier(HAAR_CC_FACE_DEFAULT);
    haar_cc_scale_face = 1.1;
    haar_cc_minneighbours_face = 4;
    haar_cc_minwidth_face = 96;
    haar_cc_minheight_face = 132;

    // eye tracking properties
    haar_cc_leye = CascadeClassifier(HAAR_CC_LEYE_MSC);
    haar_cc_reye = CascadeClassifier(HAAR_CC_REYE_MSC);
    haar_cc_scale_eyes = 1.1;
    haar_cc_minneighbours_eyes = 3;
    haar_cc_minwidth_eyes = 20;
    haar_cc_minheight_eyes = 20;

    // initialise tracking variables
    roi_ignore = Rect(-1,-1,0,0);
    poi_ignore = Point(-1,-1);
    roi_face = roi_ignore;
    roi_leye = roi_ignore;
    roi_reye = roi_ignore;
    poi_gaze = poi_ignore;
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
    if(roi_face.x>0) rectangle(frame, roi_face.tl(), roi_face.br(), roi_colour_face);
}

void Tracker::drawroieyes()
{
    if(roi_face.x>0 && roi_leye.x>0) rectangle(frame_roi_face_left, roi_leye.tl(), roi_leye.br(), roi_colour_eyes);
    if(roi_face.x>0 && roi_reye.x>0) rectangle(frame_roi_face_right, roi_reye.tl(), roi_reye.br(), roi_colour_eyes);
}

// --------------------------------------------
// private functions (tracking)
//
void Tracker::trackface()
{
    // temporary processing variables
    vector<Rect> faces;

    // haar cascade classifier for face detection
    haar_cc_face.detectMultiScale(frame, // process entire frame
                                  faces,
                                  haar_cc_scale_face,
                                  haar_cc_minneighbours_face,
                                  NULL,
                                  Size(haar_cc_minwidth_face,
                                       haar_cc_minheight_face));

    // simply store first face found for now
    if(faces.size()>0) roi_face = faces[0];
    else roi_face = roi_ignore;
}

void Tracker::trackeyes()
{
    // make sure roi is valid
    if(roi_face.x<0) return;

    // temporary processing variables
    vector<Rect> leyes, reyes;


    // haar cascade classifier for left eye detection
    haar_cc_leye.detectMultiScale(frame_roi_face_left, // process left half of face
                                  leyes,
                                  haar_cc_scale_eyes,
                                  haar_cc_minneighbours_eyes,
                                  NULL,
                                  Size(haar_cc_minwidth_eyes,
                                       haar_cc_minheight_eyes));

    // haar cascade classifier for right eye detection
    haar_cc_reye.detectMultiScale(frame_roi_face_right, // process right half of face
                                  reyes,
                                  haar_cc_scale_eyes,
                                  haar_cc_minneighbours_eyes,
                                  NULL,
                                  Size(haar_cc_minwidth_eyes,
                                       haar_cc_minheight_eyes));

    // simply store first two eyes found for now
    if(leyes.size()>0) roi_leye = leyes[0];
    else roi_leye = roi_ignore;
    if(reyes.size()>0) roi_reye = reyes[0];
    else roi_reye = roi_ignore;
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
    // set image roi to face for eye tracking (if valid)
    if(roi_face.x>0)
    {
        frame_roi_face = Mat(frame, roi_face);
        frame_roi_face_left = Mat(frame_roi_face, Rect(0, 0, roi_face.width/2, roi_face.height/2));
        frame_roi_face_right  = Mat(frame_roi_face, Rect(roi_face.width/2, 0, roi_face.width/2, roi_face.height/2));
    }
}

void Tracker::rune2gproc()
{

}

void Tracker::runposproc()
{

}
