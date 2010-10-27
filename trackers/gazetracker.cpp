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
#include "filters/dognormfilter.h"
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

    doGNormFilter = new DoGNormFilter(st);
    doGNormFilter->enable();
    filters.push_back(doGNormFilter);

    mLearningDetector = new MLearningDetector(st, 3, true, false,
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
    cv::Rect refinedRoi;
    if (!refineEyeRoi(store->eyesImg, refinedRoi))
        return;
    cv::Mat newEyeImg = store->eyesImg(refinedRoi);
    doGNormFilter->filter(newEyeImg, newEyeImg, store->ignore);
//    imshow("newEyeImg", newEyeImg);

    cv::Rect refinedRoiL;
    if (!refineEyeRoi(store->eyesImgL, refinedRoiL))
        return;
    cv::Mat newEyeImgL = store->eyesImgL(refinedRoiL);
    doGNormFilter->filter(newEyeImgL, newEyeImgL, store->ignore);
//    imshow("newEyeImgL", newEyeImgL);
#endif

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
    int totalsize = 2*size + 8;
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
                store->gazeFeatures.at<float>(inputTotalCount,2*size+1) = store->faceRoi.x + store->faceRoi.width/2;
                store->gazeFeatures.at<float>(inputTotalCount,2*size+2) = store->faceRoi.y + store->faceRoi.height/2;
                store->gazeFeatures.at<float>(inputTotalCount,2*size+3) = store->faceRoi.width;
                store->gazeFeatures.at<float>(inputTotalCount,2*size+4) = store->faceRoi.height;
                store->gazeFeatures.at<float>(inputTotalCount,2*size+5) = refinedRoi.width;
                store->gazeFeatures.at<float>(inputTotalCount,2*size+6) = refinedRoi.height;
                store->gazeFeatures.at<float>(inputTotalCount,2*size+7) = refinedRoiL.width;
                store->gazeFeatures.at<float>(inputTotalCount,2*size+8) = refinedRoiL.height;

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
            // Figure out next point
            if(store->calibX == store->gazeOuterX)
                store->calibX = 0;
            else if(store->calibX < 0)
                store->calibX *= -1;
            else
                store->calibX = -store->calibX - store->gazeDeltaX;
            if(!store->calibX)
            {
                if(store->calibY == store->gazeOuterY)
                    store->calibY = 0;
                else if(store->calibY < 0)
                    store->calibY *= -1;
                else
                    store->calibY = -store->calibY - store->gazeDeltaY;
            }

            // Check if we're done sampling gaze features for every point
            if(!store->calibX && !store->calibY)
            {
                if(timesPerPoint == CALIBRATION_PASSES)
                {
                    // Disable red dot so user doesn't get confused
                    store->calibMode = false;

                    // Start training all the data
                    mLearningDetector->train(store->gazeFeatures(cv::Rect(0,0,totalsize,inputTotalCount)),
                                             store->gazeCoords(cv::Rect(0,0,2,inputTotalCount)));

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
        cv::Mat gazeSample(1, totalsize, CV_32FC1);
        for(int i = 0; i < size; i++)
        {
            gazeSample.at<float>(0,i) = gazeImg.at<float>(0,i);
            gazeSample.at<float>(0,size+i) = gazeImgL.at<float>(0,i);
        }
        gazeSample.at<float>(0,2*size+1) = store->faceRoi.x + store->faceRoi.width/2;
        gazeSample.at<float>(0,2*size+2) = store->faceRoi.y + store->faceRoi.height/2;
        gazeSample.at<float>(0,2*size+3) = store->faceRoi.width;
        gazeSample.at<float>(0,2*size+4) = store->faceRoi.height;
        gazeSample.at<float>(0,2*size+5) = refinedRoi.width;
        gazeSample.at<float>(0,2*size+6) = refinedRoi.height;
        gazeSample.at<float>(0,2*size+7) = refinedRoiL.width;
        gazeSample.at<float>(0,2*size+8) = refinedRoiL.height;

        // Predict and store results
#if(TIME_GAZE_TRACKERS)
        double t = (double)cv::getTickCount();
#endif /* TIME_GAZE_TRACKERS */
        located = mLearningDetector->locate(gazeSample, store->gazePoint);
#if(TIME_GAZE_TRACKERS)
        t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
        qDebug() << someDetector->name().c_str() << "speed:" << 1000*t << "ms";
#endif /* TIME_GAZE_TRACKERS */

        if(located) {
            // Postprocessing (none atm)
            store->gazeX = (int)(store->gazePoint.at<float>(0,0));
            store->gazeY = (int)(store->gazePoint.at<float>(0,1));
        }
        // Updating store bool after attempting to ensures ROIs are valid
        store->gazeLocated = located;
    }
}

bool GazeTracker::refineEyeRoi(const cv::Mat& eyeImg, cv::Rect& refinedROI)
{
    // --------------------------------
    // Preprocess eye image
    cv::Mat clonedImage;
    cvtColor(eyeImg, clonedImage, CV_BGR2GRAY);
    equalizeHist(clonedImage, clonedImage);
    medianBlur(clonedImage, clonedImage, 7);


    // ------------------------------
    // Threshold eye image to find Blobs
    cv::Mat clonedMask;
    cv::inRange(clonedImage, cv::Scalar(0), cv::Scalar(60), clonedMask);
    erodeDilateFilter->filter(clonedMask, clonedMask, store->ignore);


    // ------------------------------
    // Determine and label Blobs
    IplImage binary = IplImage(clonedMask);
    IplImage *labelImg = cvCreateImage(clonedMask.size(), IPL_DEPTH_LABEL, 1);
    cvb::CvBlobs blobs;
    cvb::cvLabel(&binary, labelImg, blobs);
    std::vector<cvb::CvBlob*> vec;

    // ------------------------------
    // Determine two largest blobs, and throw the rest away
    if (blobs.size() <= 1)
    {
        return false;
    }
    else
    {
        CvLabel largest = cvb::cvGreaterBlob(blobs);
        vec.push_back(blobs.at(largest));
        blobs.erase(largest);
        largest = cvb::cvGreaterBlob(blobs);
        vec.push_back(blobs.at(largest));
        blobs.erase(largest);
        // -----------------------------------------------------
        // Adjusted Eye Height - we want to include the eye lid as well
        int pupilTop = std::max(vec[0]->miny, vec[1]->miny);
        int pupilBottom = std::max(vec[0]->maxy, vec[1]->maxy);

        pupilTop -= 5;
        if (pupilTop < 0)
            pupilTop = 0;
        pupilBottom += 5;
        if (pupilBottom > clonedImage.rows)
            pupilBottom = clonedImage.rows;

        // -----------------------------------------------------
        // Determine New Roi;
        int eyeWidth = clonedImage.cols;
        int eyeHeight = pupilBottom - pupilTop;
        refinedROI = cv::Rect(0, pupilTop, eyeWidth, eyeHeight);

        // STAGE TWO - Extra Refinement;
        cv::Mat newEyeImg = clonedImage(refinedROI);
        doGNormFilter->filter(newEyeImg, newEyeImg, store->ignore);
        cv::medianBlur(newEyeImg, newEyeImg,3);
        cv::equalizeHist(newEyeImg, newEyeImg);

        cv::Mat maskCorners;
        cv::inRange(newEyeImg, cv::Scalar(0), cv::Scalar(50), maskCorners);

        int leftOffset = 0, rightOffset = maskCorners.cols-1;
        bool leftFound = findLeftCorner(maskCorners, leftOffset);
        bool rightFound = findRightCorner(maskCorners, rightOffset);
        if (leftFound && rightFound)
        {
            refinedROI = cv::Rect(leftOffset, 0, rightOffset+1-leftOffset, maskCorners.rows) + refinedROI.tl();
        }
        return true;
    }
}

bool GazeTracker::findLeftCorner(const cv::Mat &image, int& offset)
{
    for (int x = 0; (x < image.cols); x++)
    {
        for (int y = 0; (y < image.rows); y++)
        {
            const uchar& val = image.at<uchar>(y, x);
            if (val == 255 && x > 10)
            {
                offset = x;
                return true;
            }
        }
    }
    return false;
}

bool GazeTracker::findRightCorner(const cv::Mat &image, int& offset)
{

    for (int x = image.cols-1; x >= 0; x--)
    {
        for (int y = 0; y < image.rows; y++)
        {            
            const uchar& val = image.at<uchar>(y, x);
            if (val == 255 && x < image.cols-10)
            {
                offset = x;
                return true;
            }
        }
    }    
    return false;
}
