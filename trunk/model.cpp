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
