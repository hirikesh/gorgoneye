#include <cv.h>
#include <highgui.h>
#include <QDebug>
#include "model.h"

#include "filters/grayscalefilter.h"
#include "filters/hsvfilter.h"
#include "filters/ycbcrfilter.h"
#include "filters/erodedilatefilter.h"
#include "filters/equalisefilter.h"
#include "filters/cannyedgefilter.h"
#include "filters/sobelfilter.h"

#include "trackers/facehaartracker.h"
#include "trackers/facecamshifttracker.h"
#include "trackers/facehaarcamshifttracker.h"
#include "trackers/facehaarcamshiftactracker.h"
#include "trackers/eyeshaartracker.h"

#include "trackers/facetracker.h"
#include "trackers/eyestracker.h"
#include "trackers/gazetracker.h"

using cv::VideoCapture;
using cv::Mat;
using std::vector;
using std::string;

Model::Model(int device) :
    capture(VideoCapture(device)),
    store(Store()),

    // Completed trackers
    faceHaarTracker(new FaceHaarTracker(&store)),
    faceCAMShiftTracker(new FaceCAMShiftTracker(&store)),
    faceHaarCAMShiftTracker(new FaceHaarCAMShiftTracker(&store)),
    faceHaarCAMShiftACTracker(new FaceHaarCAMShiftACTracker(&store)),
    eyesHaarTracker(new EyesHaarTracker(&store)),

    // Work-in-progress trackers
    faceTracker(new FaceTracker(&store)),
    eyesTracker(new EyesTracker(&store)),
    gazeTracker(new GazeTracker(&store))

{
    string props[] = {"Millisecond", "Frames", "Ratio", "Width", "Height",
                      "FPS", "FOURCC Codec", "Frame Count", "Format",
                      "Mode", "Brightness", "Contrast", "Saturation",
                      "Hue", "Gain", "Exposure", "RGB colour", "White Balancing",
                      "Rectification"};
    for(int i=0; i<19; i++)
    {
        double prop = capture.get(i);
        if(prop > 0)
            qDebug() << "Property supported:" << props[i].c_str() << '=' << prop;
    }

    // Add runtime filters
    filters.push_back(new EqualiseFilter(&store));
    filters.push_back(new GrayscaleFilter(&store));
    filters.push_back(new HSVFilter(&store));
    filters.push_back(new YCbCrFilter(&store));
    filters.push_back(new ErodeDilateFilter(&store));
    filters.push_back(new CannyEdgeFilter(&store));
    filters.push_back(new SobelFilter(&store));

    // Instantiate completed trackers
    faceHaarTracker->disable();
    faceCAMShiftTracker->disable();
    faceHaarCAMShiftTracker->disable();
    faceHaarCAMShiftACTracker->enable();
    eyesHaarTracker->enable();

    trackers.push_back(faceHaarTracker);
    trackers.push_back(faceCAMShiftTracker);
    trackers.push_back(faceHaarCAMShiftTracker);
    trackers.push_back(faceHaarCAMShiftACTracker);
    trackers.push_back(eyesHaarTracker);

    // Instantiate work-in-progress trackers
    faceTracker->disable();
    eyesTracker->disable();
    gazeTracker->disable();

    trackers.push_back(faceTracker);
    trackers.push_back(eyesTracker);
    trackers.push_back(gazeTracker);

    // Initialisation of runtime vars
    store.dispImg = &store.sceneImg;
    store.faceRoi = cv::Rect(0, 0, 640, 480);
}

#if PREPROC_STAGE == 1
#define PREPROC_FACE
#elif PREPROC_STAGE == 2
#define PREPROC_EYES
#elif PREPROC_STAGE == 3
#define PREPROC_GAZE
#endif /* PREPROC_STAGE */

void Model::update()
{
    capture >> store.sceneImg;

#ifdef PREPROC_FACE
    preProcess();
#endif /* PREPROC_FACE */

    // Track face
    faceHaarTracker->track();
    faceCAMShiftTracker->track();
    faceHaarCAMShiftTracker->track();
    faceHaarCAMShiftACTracker->track();
    faceTracker->track();

    // Update face image even if face tracker
    // failed or was disabled.
    store.faceImg = store.sceneImg(store.faceRoi);

#ifdef PREPROC_EYES
    preProcess();
#endif /* PREPROC_EYES */

    // Attempt to track eyes even if face failed or
    // was disabled.
    eyesHaarTracker->track();
    eyesTracker->track();

    // Update eyes image only if tracker succeeds.
    // A successful face track could render the old
    // eye ROI invalid.
    if(store.eyesLocated)
    {
        store.eyesImg = store.faceImg(store.eyesRoi);
#ifdef PREPROC_GAZE
        preProcess();
#endif /* PREPROC_GAZE */
        gazeTracker->track();
    }

    postProcess();
}

Store* Model::getStore()
{
    return &store;
}

vector<BaseFilter*>* Model::getPtrFilters()
{
    return &filters;
}

vector<BaseTracker*>* Model::getPtrTrackers()
{
    return &trackers;
}

Mat* Model::getDispImg()
{
    return store.dispImg;
}

void Model::preProcess()
{

#if(TIME_FILTERS)
    double t = (double)cv::getTickCount();
#endif /* TIME_FILTERS */
    for (unsigned int i = 0; i < filters.size(); i++)
    {
#if defined(PREPROC_FACE)
        filters[i]->filter(store.sceneImg, store.sceneImg, store.ignore);
#elif defined(PREPROC_EYES)
        filters[i]->filter(store.faceImg, store.faceImg, store.ignore);
#elif defined(PREPROC_GAZE)
        filters[i]->filter(store.eyesImg, store.eyesImg, store.ignore);
#endif
    }
#if(TIME_FILTERS)
    t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
    qDebug() << "Preproc. speed:" << 1000*t << "ms";
#endif /* TIME_FILTERS */
}

void Model::postProcess()
{
}
