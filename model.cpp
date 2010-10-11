#include <cv.h>
#include <highgui.h>
#include <QDebug>
#include "model.h"
#include "filters/grayscalefilter.h"
#include "filters/hsvfilter.h"
#include "filters/ycbcrfilter.h"
#include "filters/erodedilatefilter.h"
#include "filters/equalisefilter.h"
#include "filters/cannycontourfilter.h"

using cv::VideoCapture;
using cv::Mat;
using std::vector;
using std::string;

Model::Model(int device) :
    capture(VideoCapture(device)),
    store(Store()),
    faceHaarTracker(new FaceHaarTracker(&store)),
    faceCAMShiftTracker(new FaceCAMShiftTracker(&store)),
    faceHaarCAMShiftTracker(new FaceHaarCAMShiftTracker(&store)),
    eyesHaarTracker(new EyesHaarTracker(&store))
//    faceTracker(new FaceTracker(&store)),
//    eyesTracker(new EyesTracker(&store))
{
#ifdef __linux__
//    capture.set(CV_CAP_PROP_BRIGHTNESS, 0.004);
//    capture.set(CV_CAP_PROP_CONTRAST, 0.12);
//    capture.set(CV_CAP_PROP_GAIN, 0.05);
//    capture.set(CV_CAP_PROP_EXPOSURE, 0.5);
#endif

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

    // Initialise face ROI
    capture >> store.sceneImg;
    store.faceRoi = cv::Rect(0, 0, store.sceneImg.cols, store.sceneImg.rows);

    // Add runtime filters
    filters.push_back(new GrayscaleFilter(&store));
    filters.push_back(new HSVFilter(&store));
    filters.push_back(new YCbCrFilter(&store));
    filters.push_back(new ErodeDilateFilter(&store));
    filters.push_back(new EqualiseFilter(&store));
    filters.push_back(new CannyContourFilter(&store));

    // Instantiate all trackers
    faceHaarTracker->disable();
    trackers.push_back(faceHaarTracker);

    faceCAMShiftTracker->disable();
    trackers.push_back(faceCAMShiftTracker);

    faceHaarCAMShiftTracker->enable();
    trackers.push_back(faceHaarCAMShiftTracker);

    eyesHaarTracker->enable();
    trackers.push_back(eyesHaarTracker);

//    faceTracker->setDetector(FaceTracker::HYBR);
//    faceTracker->disable();
//    trackers.push_back(faceTracker);

//    eyesTracker->setDetector(EyesTracker::HAAR);
//    eyesTracker->disable();
//    trackers.push_back(eyesTracker);

    // Initialisation of store vars
    store.dispImg = &store.sceneImg;
}

void Model::update()
{
    capture >> store.sceneImg;
    preProcess();

    // Track face
//    faceTracker->track();
    faceHaarTracker->track();
    faceCAMShiftTracker->track();
    faceHaarCAMShiftTracker->track();

    // Update face image even if face tracker
    // failed or was disabled.
    store.faceImg = store.sceneImg(store.faceRoi);

//    preProcess();

    // Attempt to track eyes even if face failed or
    // was disabled.
//    eyesTracker->track();
    eyesHaarTracker->track();

    // Update eyes image only if tracker succeeds.
    // A successful face track could render the old
    // eye ROI invalid.
    if(store.eyesLocated)
        store.eyesImg = store.faceImg(store.eyesRoi);

//    if(store.eyesLocated)
//        gazeTracker->track();

    postProcess();
}

Store* Model::getStore()
{
    return &store;
}

vector<BaseTracker*> Model::getTrackers()
{
    return trackers;
}

vector<BaseTracker*>* Model::getPtrTrackers()
{
    return &trackers;
}

vector<BaseFilter*>* Model::getPtrFilters()
{
    return &filters;
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
        filters[i]->filter(store.sceneImg, store.sceneImg, store.sceneMsk);
//        filters[i]->filter(store.faceImg, store.faceImg, store.faceMsk);
    }
//    t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
//    qDebug() << "Preproc. speed:" << 1000*t << "ms";
//        qDebug() << "filters running: " << filters.size();

    //    Colour Space Conversion: BGR -> YCbCr
    //    static Size srcImgSize = store.sceneImg.size();
    //    static Mat cYCrCbImg(srcImgSize, CV_8UC3);
    //    static Mat lumaImg(srcImgSize, CV_8UC1);
    //    static Mat chromaRedImg(srcImgSize, CV_8UC1);
    //    static Mat chromaBlueImg(srcImgSize, CV_8UC1);
    //    static Mat cYCrCbChannels[] = {lumaImg, chromaRedImg, chromaBlueImg};
    //    cvtColor(store.sceneImg, cYCrCbImg, CV_BGR2YCrCb);
    //    split(cYCrCbImg, cYCrCbChannels);

        //    --- Light Compensation ---
    //    Mat maskLumaImg = lumaImg >= 240;
    //    Scalar sumLuma = sum(maskLumaImg);
    //    if ((int)sumLuma[0]/255 > 100)
    //    {
    //        Scalar avgGray = mean(lumaImg, maskLumaImg);
    //        double scaleFactor = 255.0/avgGray[0];
    //        scaleF = Scalar(scaleFactor, scaleFactor, scaleFactor);
    //        multiply(store.sceneImg, scaleF, store.sceneImg);
    //        Scalar avgGrayChk = mean(store.sceneImg, maskLumaImg);
    //    }

    // --- Histogram Equalisation ---
    //    equalizeHist(lumaImg, lumaImg);
    //    merge(cYCrCbChannels, 3, cYCrCbImg);
    //    cvtColor(cYCrCbImg, store.sceneImg, CV_YCrCb2BGR);
}

void Model::postProcess()
{
    // Reset masks for next update
    store.sceneMsk = Mat();
    store.faceMsk = Mat();
    store.eyesMsk = Mat();
}
