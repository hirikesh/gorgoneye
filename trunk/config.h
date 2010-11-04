/* Use this place to put system, platform and
   per-use dependent constants, etc.

   Any class which requires these constants
   will need to import this, either directly
   or indirectly. */

#ifndef CONFIG_H
#define CONFIG_H

/* START: gazetracker.cpp & mlearningdetector.cpp */
#define IGNORED_SAMPLES_PER_POINT 10
#define SAMPLES_PER_POINT 10
#define XVALID_SAMPLES_PER_POINT 2
#define CALIBRATION_PASSES 1

/* START: glgaze.{h,cpp} */
// Scale factor for gaze resolution:
//     DPI_SCALE * detected_screen_dpi = X and Y distance between adjacent gaze points
#define DPI_SCALE 2.0 // large number to start off easy
/* END:   glgaze.{h,cpp} */


/* START: glview.{h,cpp} */
// Frame size for various uses in glview
#define FRAME_WIDTH 640
#define FRAME_HEIGHT 480
/* END:   glview.{h,cpp} */


/* START: control.{h,cpp} */
// camera capture & gl update rate in milliseconds
#define CAP_TIMER_MS 33
/* END:   control.{h,cpp} */


/* START: model.{h,cpp} */
// Set stage for preprocessing to run:
//     1 - face, 2 - eyes, 3 - gaze, else - none
#define PREPROC_STAGE 3
// Output processing-time information to qDebug():
//     0 - no timing info, 1 - timing via opencv ticks()
#define TIME_FILTERS 0
/* END:   model.{h,cpp} */


/* START: basetracker.{h,cpp} */
// Output detector/tracker processing-time information to qDebug():
//     0 - no timing info, 1 - timing via opencv ticks()
#define TIME_FACE_TRACKERS 0 // 0 -
#define TIME_EYES_TRACKERS 0
#define TIME_GAZE_TRACKERS 0
// Global CAMShift filter parameter defaults
#define MIN_HUE 0
#define MAX_HUE 180
#define MIN_SAT 30
#define MAX_SAT 256
#define MIN_VAL 10
#define MAX_VAL 234
#define MIN_LUM 0
#define MAX_LUM 256
#define MIN_CR 125
#define MAX_CR 174
#define MIN_CB 77
#define MAX_CB 128
#define ERODE_DILATE_DEPTH 2
/* END:   basetracker.{h,cpp} */


/* START: basedetector.{h,cpp} */
// Set the opencv data directory
#ifdef _WIN32
#define HAAR_CC_DIR "c:\\opencv2.1\\data\\haarcascades\\"
#else
#define HAAR_CC_DIR "/usr/share/opencv/haarcascades/"
#endif

// Set the desired haar classifier data to use
#define HAAR_CC_FACE HAAR_CC_DIR"haarcascade_frontalface_default.xml"
#define HAAR_CC_EYES HAAR_CC_DIR"haarcascade_righteye_2splits.xml"
#define HAAR_CC_EYESL HAAR_CC_DIR"haarcascade_lefteye_2splits.xml"
#define HAAR_CC_EYES2 HAAR_CC_DIR"haarcascade_mcs_righteye.xml"
/* END:   basedetector.{h,cpp} */


#endif // CONFIG_H
