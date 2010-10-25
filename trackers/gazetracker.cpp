#include <cv.h>
#include <highgui.h>
#include <cvblob.h>
#include <QDebug>
#include "gazetracker.h"
#include "filters/cannyedgefilter.h"
#include "filters/grayscalefilter.h"
#include "filters/ycbcrfilter.h"
#include "filters/harriscornerfilter.h"
#include "filters/equalisefilter.h"
#include "filters/erodedilatefilter.h"
#include "filters/cornerfilter.h"
#include "detectors/mlearningdetector.h"
#include "store.h"

using cvb::CvLabel;

GazeTracker::GazeTracker(Store* st) :
    BaseTracker(st, "Gaze-test"),
    inputTotalCount(0), inputPerPointCount(0), timesPerPoint(1)
{
    cannyEdgeFilter = new CannyEdgeFilter(st, 13500, 13500, 7);
    cannyEdgeFilter->enable();
    filters.push_back(cannyEdgeFilter);

    grayscaleFilter = new GrayscaleFilter(st, 18, 256);
    grayscaleFilter->enable();
    filters.push_back(grayscaleFilter);

    ycbcrFilter = new YCbCrFilter(st);
    ycbcrFilter->enable();
    filters.push_back(ycbcrFilter);

    harrisCornerFilter = new HarrisCornerFilter(st);
    harrisCornerFilter->enable();
    filters.push_back(harrisCornerFilter);

    equaliseFilter = new EqualiseFilter(st);
    equaliseFilter->enable();
    filters.push_back(equaliseFilter);

    erodeDilateFilter = new ErodeDilateFilter(st);
    erodeDilateFilter->enable();

    cornerFilter = new CornerFilter(st, 2, 0.20, 20, 3, false);
    cornerFilter->enable();
    filters.push_back(cornerFilter);

    mLearningDetector = new MLearningDetector(st, 3, false, false,
                                              false, 2, 50, 0.001);
    detectors.push_back(mLearningDetector);

    BaseTracker::initImageModes();
}

void GazeTracker::track()
{

    if(!enabled) return;

    // Preprocessing

    // Filtering
//    cannyEdgeFilter->filter(store->eyesImg, store->ignore, store->ignore);
//    ycbcrFilter->filter(store->eyesImg, store->ignore, store->ignore);
//    harrisCornerFilter->filter(store->eyesImg, store->ignore, store->ignore);
//    equaliseFilter->filter(store->eyesImg, store->gazeImg, store->ignore);
//    grayscaleFilter->filter(store->gazeImg, store->ignore, store->ignore);

#if 1
    // CROPPING
    cv::Mat clonedImage;
    cvtColor(store->eyesImg, clonedImage, CV_BGR2GRAY);
    equalizeHist(clonedImage, clonedImage);
    medianBlur(clonedImage, clonedImage, 7);

    cv::Mat clonedMask;
    cv::inRange(clonedImage, cv::Scalar(0), cv::Scalar(60), clonedMask);
    erodeDilateFilter->filter(clonedMask, clonedMask, store->ignore);

    IplImage binary = IplImage(clonedMask);
    IplImage *labelImg = cvCreateImage(clonedMask.size(), IPL_DEPTH_LABEL, 1);
    cvb::CvBlobs blobs;
    cvb::cvLabel(&binary, labelImg, blobs);
    std::vector<cvb::CvBlob*> vec;
    cv::Mat newEyeImg;
    if (blobs.size() > 1)
    {
        CvLabel largest = cvb::cvGreaterBlob(blobs);
        vec.push_back(blobs.at(largest));
        blobs.erase(largest);
        largest = cvb::cvGreaterBlob(blobs);
        vec.push_back(blobs.at(largest));
        blobs.erase(largest);
        int newY = std::max(vec[0]->miny, vec[1]->miny);
        int newY2 = std::max(vec[0]->maxy, vec[1]->maxy);
        cv::Rect newRoi(0, newY, store->eyesRoi.width, newY2-newY-1);

        newEyeImg = clonedImage(newRoi);
//        newEyeImg = cv::Mat(store->eyesImg(newRoi));
//        grayscaleFilter->filter(newEyeImg, newEyeImg, store->ignore);
//        equaliseFilter->filter(newEyeImg, newEyeImg, store->ignore);
//        medianBlur(newEyeImg, newEyeImg, 7);
    }
    else
        return;

    // DUPLICATE CODE FOR LEFT EYE PCA
    cv::Mat clonedImageL;
    cvtColor(store->eyesImgL, clonedImageL, CV_BGR2GRAY);
    equalizeHist(clonedImageL, clonedImageL);
    medianBlur(clonedImageL, clonedImageL, 7);

    cv::Mat clonedMaskL;
    cv::inRange(clonedImageL, cv::Scalar(0), cv::Scalar(60), clonedMaskL);
    erodeDilateFilter->filter(clonedMaskL, clonedMaskL, store->ignore);

    IplImage binaryL = IplImage(clonedMaskL);
    IplImage *labelImgL = cvCreateImage(clonedMaskL.size(), IPL_DEPTH_LABEL, 1);
    cvb::CvBlobs blobsL;
    cvb::cvLabel(&binaryL, labelImgL, blobsL);
    std::vector<cvb::CvBlob*> vecL;
    cv::Mat newEyeImgL;
    if (blobsL.size() > 1)
    {
        CvLabel largest = cvb::cvGreaterBlob(blobsL);
        vecL.push_back(blobsL.at(largest));
        blobsL.erase(largest);
        largest = cvb::cvGreaterBlob(blobsL);
        vecL.push_back(blobsL.at(largest));
        blobsL.erase(largest);
        int newY = std::max(vecL[0]->miny, vecL[1]->miny);
        int newY2 = std::max(vecL[0]->maxy, vecL[1]->maxy);
        cv::Rect newRoi(0, newY, store->eyesRoiL.width, newY2-newY-1);

        newEyeImgL = clonedImageL(newRoi);
//        newEyeImgL = cv::Mat(store->eyesImgL(newRoi));
//        grayscaleFilter->filter(newEyeImgL, newEyeImgL, store->ignore);
//        equaliseFilter->filter(newEyeImgL, newEyeImgL, store->ignore);
//        medianBlur(newEyeImgL, newEyeImgL, 7);
    }
    else
        return;
    // END DUPLICATE CODE
#endif

    // TEMPLATE MATCHING
//    cv::Mat tmpSrc;
//    cvtColor(store->gazeImg, tmpSrc, CV_BGR2GRAY);
//    cv::Mat tmpLate(7,7,CV_8UC1,cv::Scalar(24)), tmpLateResult;
//    tmpLate(cv::Rect(2,2,2,2)).setTo(cv::Scalar(64));
//    matchTemplate(tmpSrc, tmpLate, tmpLateResult, CV_TM_SQDIFF);


//    double a, b; cv::Point c, d;
//    minMaxLoc(tmpLateResult, &a, &b, &c, &d);

//        static cv::Mat test;
//        cvtColor(tmpSrc, test, CV_GRAY2BGR);
//        cv::rectangle(test, cv::Rect(c.x - tmpLate.cols/2, c.y - tmpLate.rows/2, tmpLate.cols, tmpLate.rows), cv::Scalar(128,128,128), 1);
//        store->dispImg = &test;
    // TEMPLATE MATCHING

    // FAKE EYES
#if 0
    store->eyesLocated = true;
    store->eyesLocatedL = true;
    cv::Mat newEyeImg(12, 50, CV_8UC3, cv::Scalar(0,0,0));
    cv::Mat newEyeImgL(12, 50, CV_8UC3, cv::Scalar(0,0,0));
#endif
    // FAKE EYES

    // INPUT CONVERSION - to fixed size and compatible type for training
    static cv::Mat tmpGazeImg, gazeImg(1, 20, CV_16UC1);
    cv::resize(newEyeImg, gazeImg, cv::Size(20,14), cv::INTER_AREA);
//    for(int i = 0; i < tmpGazeImg.cols; i++)
//        gazeImg(cv::Rect(i,0,1,1)) = sum(tmpGazeImg(cv::Rect(i,0,1,tmpGazeImg.rows)));
//    cv::normalize(gazeImg, gazeImg, 0, 255, cv::NORM_MINMAX);
    gazeImg.convertTo(gazeImg, CV_32FC1, 1./255);

    static cv::Mat tmpGazeImgL, gazeImgL(1, 20, CV_16UC1);
    cv::resize(newEyeImgL, gazeImgL, cv::Size(20,14), cv::INTER_AREA);
//    for(int i = 0; i < tmpGazeImgL.cols; i++)
//        gazeImgL(cv::Rect(i,0,1,1)) = sum(tmpGazeImgL(cv::Rect(i,0,1,tmpGazeImg.rows)));
//    cv::normalize(gazeImgL, gazeImgL, 0, 255, cv::NORM_MINMAX);
    gazeImgL.convertTo(gazeImgL, CV_32FC1, 1./255);
    // INPUT CONVERSION

    // Calibration mode involves automated data collection and subsequent training on that data
    int size = gazeImg.rows*gazeImg.cols;
    if(store->calibMode)
    {
        if(inputPerPointCount < IGNORED_SAMPLES_PER_POINT) // give user a break
        {
            inputPerPointCount++;
        }
        else if(inputPerPointCount == IGNORED_SAMPLES_PER_POINT)
        {
            store->calibNow = true; // used by UI to notify user - about to start grabbing inputs
            inputPerPointCount++;
        }
        else if(inputPerPointCount < IGNORED_SAMPLES_PER_POINT + SAMPLES_PER_POINT + 1) // grab the frame for input
        {
            if(store->eyesLocated && store->eyesLocatedL)
            {
                // Save this image for future use

                // Add gaze input image as a single row in the feature matrix
                for(int i = 0; i < size; i++)
                {
                    store->gazeFeatures.at<float>(inputTotalCount,i) = gazeImg.at<float>(0,i);
                    store->gazeFeatures.at<float>(inputTotalCount,size+i) = gazeImgL.at<float>(0,i);
                }
                // Add calibration point of gaze to outputs
                store->gazeCoords.at<float>(inputTotalCount,0) = (float)store->calibX;
                store->gazeCoords.at<float>(inputTotalCount,1) = (float)store->calibY;

                // Update total
                inputTotalCount++;
                inputPerPointCount++;
                qDebug() << "Added sample number:" << inputTotalCount-1 << "@" << store->calibX << store->calibY;
            }
        }
        else if(inputPerPointCount == IGNORED_SAMPLES_PER_POINT + SAMPLES_PER_POINT + 1) // move to next point
        {
            store->calibNow = false; // used by UI to notify user - not getting inputs anymore
            inputPerPointCount++;
        }
        else
        {
            // Other method for next point
            if(store->calibX == store->gazeOuterX)
                store->calibX = 0;
            else if(store->calibX < 0)
                store->calibX *= -1;
            else
                store->calibX = -store->calibX - store->gazeDeltaX;

            // Figure out the next point for training, if any
//            store->calibX = store->calibX == store->gazeOuterX ? -store->gazeOuterX : store->calibX + store->gazeDeltaX;
//            store->calibY = store->calibX == -store->gazeOuterX ? store->calibY + store->gazeDeltaY : store->calibY;
//            store->calibY = store->calibY > store->gazeOuterY ? -store->gazeOuterY : store->calibY;
            // Check if we're done sampling gaze features for every point
            if(!store->calibX && !store->calibY)
//            if(store->calibX == -store->gazeOuterX && store->calibY == -store->gazeOuterY)
            {
                if(timesPerPoint == CALIBRATION_PASSES)
                {
                    // Disable red dot so user doesn't get confused
                    store->calibMode = false;

                    // Start training all the data
                    mLearningDetector->train(store->gazeFeatures(cv::Rect(0,0,2*size,inputTotalCount)),
                                             store->gazeCoords(cv::Rect(0,0,1,inputTotalCount)));

                    // Done learning this sample
                    inputTotalCount = 0;
                    timesPerPoint = 1;
                }
                else
                {
                    timesPerPoint++;
                }
            }
            inputPerPointCount = 0; // reset for next point
        }
    }
    else
    {
        // Prepare prediction sample
        cv::Mat gazeSample(1, 2*size, CV_32FC1);
        for(int i = 0; i < size; i++)
        {
            gazeSample.at<float>(0,i) = gazeImg.at<float>(0,i);
            gazeSample.at<float>(0,size+i) = gazeImgL.at<float>(0,i);
        }

        // Predict and store results
#if(TIME_GAZE_TRACKERS)
        double t = (double)cv::getTickCount();
#endif /* TIME_GAZE_TRACKERS */
        located = mLearningDetector->locate(gazeSample(cv::Rect(0,0,2*size,1)), store->gazePoint);
#if(TIME_GAZE_TRACKERS)
        t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
        qDebug() << someDetector->name().c_str() << "speed:" << 1000*t << "ms";
#endif /* TIME_GAZE_TRACKERS */

        if(located) {
            // Postprocessing (none atm)
            store->gazeX = (int)(store->gazePoint.at<float>(0,0));
            store->gazeY = (int)(store->gazePoint.at<float>(0,1));
//            qDebug() << store->gazePoint.at<float>(0,0) << store->gazePoint.at<float>(0,1);
        }
        // Updating store bool after attempting to ensures ROIs are valid
        store->gazeLocated = located;
    }
}
