#include <cv.h>
#include <highgui.h>
#include <QDebug>
#include "model.h"
#include "filters/grayscalefilter.h"
#include "filters/hsvfilter.h"
#include "filters/ycbcrfilter.h"
#include "filters/erodedilatefilter.h"

using cv::VideoCapture;
using cv::Mat;
using std::vector;
using std::string;

Model::Model(int device) :
    capture(VideoCapture(device)),
    store(Store()),
    faceTracker(FaceTracker(&store)),
    eyesTracker(EyesTracker(&store))
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
    for(int i=0; i<19; i++) {
        double prop = capture.get(i);
        if(prop > 0)
            qDebug() << "Property supported:" << props[i].c_str() << '=' << prop;
    }
    //
    filters.push_back(new GrayscaleFilter("Grayscale Filter", &store));
    filters.push_back(new HSVFilter("HSV Filter", &store));
    filters.push_back(new YCbCrFilter("YCrCb Filter", &store));
    filters.push_back(new ErodeDilateFilter("Erode-Dilate Filter", &store));

    // Instantiate all trackers
    faceTracker.setDetector(FaceTracker::HYBR);
    faceTracker.enable();
    trackers.push_back(&faceTracker);

    eyesTracker.setDetector(EyesTracker::HAAR);
    eyesTracker.disable();
    trackers.push_back(&eyesTracker);

    // Initialisation of store vars
    store.dispImg = &store.sceneImg;
}

void Model::update()
{
    capture >> store.sceneImg;

    preProcess();
    // Track face
    faceTracker.track();

    // Update face ROI even if tracker failed
    store.faceImg = store.sceneImg(store.faceRoi).clone();

    // Track eyes only if face succeeded
    if(store.faceLocated)
        eyesTracker.track();

    // Update eyes ROI even if tracker failed
    store.eyesImg = store.faceImg(store.eyesRoi).clone();

//    if(store.eyesLocated)
//        gazeTracker.track();
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
    for (unsigned int i = 0; i < filters.size(); i++)
    {
        BaseFilter* currFilter = filters[i];
        currFilter->filter(store.sceneImg, store.sceneImg, store.sceneMsk, store.sceneMsk);
    }
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
}
