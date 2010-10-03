#include <cv.h>
#include <highgui.h>
#include <QDebug>
#include "model.h"


using namespace cv;
using std::vector;

Model::Model(int device) :
    capture(VideoCapture(device)),
    store(Store()),
    faceTracker(FaceTracker(&store)),
    eyesTracker(EyesTracker(&store))
{
    // initialisation no longer required
    // but leave here anyway, doesn't hurt.

    capture >> store.sceneImg;

    eyesTracker.setDetector(EyesTracker::HAAR);
    eyesTracker.disable();
    trackers.push_back(&eyesTracker);

    faceTracker.setDetector(FaceTracker::HYBR);
    faceTracker.enable();
    trackers.push_back(&faceTracker);

    // current UI display
    store.dispImg = &store.sceneImg;
}

void Model::update()
{
    capture >> store.sceneImg;

//    Lighting Compensation
//    Scalar avgImg = mean(store.sceneImg);
//    double avgB = avgImg[0];
//    double avgG = avgImg[1];
//    double avgR = avgImg[2];
//    double avgGrey = (avgR + avgG + avgB)/3;
//    double aR = avgGrey/avgR;
//    double aG = avgGrey/avgG;
//    double aB = avgGrey/avgB;
//    static Mat scaleF(store.sceneImg.size(), CV_8UC3);
//    scaleF = Scalar(aB, aG, aR);
//    multiply(store.sceneImg, scaleF, store.sceneImg);

// Histogram Equalisation
//    static Size srcImgSize = store.sceneImg.size();
//    static Mat cYCrCbImg(srcImgSize, CV_8UC3);
//    static Mat lumaImg(srcImgSize, CV_8UC1);
//    static Mat chromaRedImg(srcImgSize, CV_8UC1);
//    static Mat chromaBlueImg(srcImgSize, CV_8UC1);
//    static Mat cYCrCbChannels[] = {lumaImg, chromaRedImg, chromaBlueImg};
//    cvtColor(store.sceneImg, cYCrCbImg, CV_BGR2YCrCb);
//    split(cYCrCbImg, cYCrCbChannels);
//    equalizeHist(lumaImg, lumaImg);
//    merge(cYCrCbChannels, 3, cYCrCbImg);
//    cvtColor(cYCrCbImg, store.sceneImg, CV_YCrCb2BGR);
    faceTracker.track();
    eyesTracker.track();
    //gazeTracker.track();
}

Store* Model::getStore()
{
    return &store;
}

vector<BaseTracker*> Model::getTrackers()
{
    return trackers;
}

Mat* Model::getDispImg()
{
    return store.dispImg;
}
