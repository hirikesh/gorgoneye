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

void Model::update()
{
    capture >> store.sceneImg;
    preProcess();

    // Track face
    faceHaarTracker->track();
    faceCAMShiftTracker->track();
    faceHaarCAMShiftTracker->track();
    faceHaarCAMShiftACTracker->track();
    faceTracker->track();

    // Update face image even if face tracker
    // failed or was disabled.
    store.faceImg = store.sceneImg(store.faceRoi);

//    preProcess();

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
//        preProcess();
        gazeTracker->track();
    }

    postProcess();
}

Store* Model::getStore()
{
    return &store;
}

vector<BaseFilter*> Model::getFilters()
{
    return filters;
}

vector<BaseFilter*>* Model::getPtrFilters()
{
    return &filters;
}

vector<BaseTracker*> Model::getTrackers()
{
    return trackers;
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
    // run each filter in turn
//    double t = (double)cv::getTickCount();
    for (unsigned int i = 0; i < filters.size(); i++)
    {
        filters[i]->filter(store.sceneImg, store.sceneImg, store.ignore);
//        filters[i]->filter(store.faceImg, store.faceImg, store.ignore);
//        filters[i]->filter(store.eyesImg, store.eyesImg, store.ignore);
    }
//    t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
//    qDebug() << "Preproc. speed:" << 1000*t << "ms";
}

void Model::postProcess()
{
}
