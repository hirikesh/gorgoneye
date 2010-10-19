#include <cv.h>
#include <QDebug>
#include "gazetracker.h"
#include "filters/cannyedgefilter.h"
#include "filters/grayscalefilter.h"
#include "filters/ycbcrfilter.h"
#include "detectors/mlearningdetector.h"
#include "store.h"

GazeTracker::GazeTracker(Store* st) :
    BaseTracker(st, "Gaze-test"),
    inputTotalCount(0)
{
    cannyEdgeFilter = new CannyEdgeFilter(st, 800, 800);
//    cannyEdgeFilter->enable();
//    filters.push_back(cannyEdgeFilter);

    grayscaleFilter = new GrayscaleFilter(st, 18, 256);
//    grayscaleFilter->enable();
//    filters.push_back(grayscaleFilter);

    ycbcrFilter = new YCbCrFilter(st);
//    ycbcrFilter->enable();
//    filters.push_back(ycbcrFilter);

    mLearningDetector = new MLearningDetector(st);
    detectors.push_back(mLearningDetector);

    BaseTracker::initImageModes();
}

void GazeTracker::track()
{

    if(!enabled) return;

    // Preprocessing

    // Filtering
//    cannyEdgeFilter->filter(store->eyesImg, store->ignore, store->ignore);
//    grayscaleFilter->filter(store->eyesImg, store->ignore, store->ignore);
//    ycbcrFilter->filter(store->eyesImg, store->ignore, store->ignore);


    // FAKE EYES
#if 0
    store->gazeImg = cv::Mat(24, 48, CV_8UC1, cv::Scalar(store->calibX*255./store->calibW));
#endif
    // FAKE EYES

    // START raw input conversion to compatible type + continous matrix
    static cv::Mat tmpGazeImg, gazeImg;
    cv::resize(store->gazeImg, tmpGazeImg, cv::Size(48,48), cv::INTER_LANCZOS4); // fixed input size
    gazeImg = tmpGazeImg(cv::Rect(0, 20, 48, 24)); // chop off eyebrow
    gazeImg.convertTo(gazeImg, CV_32FC1, 1./255); // NxM matrix
    // END raw input conversion

    // Calibration mode involves automated data collection and subsequent training on that data
    if(store->calibMode)
    {
#define FRAME_BREAK 50
        if(inputPerPointCount < FRAME_BREAK) // give user a break
        {
            inputPerPointCount++;
        }
#define FRAME_INPUT 150
        else if(inputPerPointCount == FRAME_BREAK)
        {
            store->calibNow = true; // used by UI to notify user - about to start grabbing inputs
            inputPerPointCount++;
        }
        else if(inputPerPointCount < FRAME_BREAK + FRAME_INPUT + 1) // grab the frame for input
        {
            // Add gaze input image as a single row in the feature matrix
            for(int i = 0; i < 48*24; i++)
                store->gazeFeatures.ptr<float>(inputTotalCount)[i] = gazeImg.ptr<float>(0)[i];
            // Add calibration point of gaze to outputs
            store->gazeCoords.ptr<float>(inputTotalCount)[0] = (float)store->calibX/(float)store->calibW;
//            gazeCoord.ptr(featIdx)[1] = store->calibY;
            // Update total
            inputTotalCount++;
            inputPerPointCount++;
            qDebug() << "Added sample number:" << inputTotalCount-1 << "@" << store->calibX << store->calibY;
        }
#define INPUT_DIM 24
#define OUTPUT_DIM 24
        else // move to next point
        {
            inputPerPointCount = 0; // reset for next point if there are points remaining
            store->calibNow = false; // used by UI to notify user - not getting inputs anymore

            // Figure out the next point for training
            store->calibX = store->calibX >= store->calibW ? 0 : store->calibX + 1;
            store->calibY = store->calibX ? store->calibY : store->calibY - 1;
            store->calibY = store->calibY < 0 ? store->calibH : store->calibY;

            // Check if we're done sampling gaze features for every point
            if(!store->calibX && store->calibY == store->calibH)
            {
                // Start training all the data
                mLearningDetector->train(store->gazeFeatures(cv::Rect(0,0,INPUT_DIM,inputTotalCount)),
                                         store->gazeCoords(cv::Rect(0,0,OUTPUT_DIM,inputTotalCount)));
                // Done learning this sample
                inputTotalCount = 0;
                store->calibMode = false;
            }
        }
    }
    else
    {
        // Prepare prediction sample
        cv::Mat gazeSample(1, 48*24, CV_32FC1);
        for(int i = 0; i < 48*24; i++)
            gazeSample.ptr(0)[i] = gazeImg.ptr(0)[i];

        // Predict and store results
#if(TIME_GAZE_TRACKERS)
        double t = (double)cv::getTickCount();
#endif /* TIME_GAZE_TRACKERS */
        located = mLearningDetector->locate(gazeSample(cv::Rect(0,0,INPUT_DIM,1)), store->gazePoint);
#if(TIME_GAZE_TRACKERS)
        t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
        qDebug() << someDetector->name().c_str() << "speed:" << 1000*t << "ms";
#endif /* TIME_GAZE_TRACKERS */

        if(located) {
            // DEBUGGING
            store->gazeX = store->calibW*store->gazePoint.ptr<float>(0)[0];
            qDebug() << store->calibW*store->gazePoint.ptr<float>(0)[0];
            // Postprocessing (none atm)
        }
    //     Updating store bool after attempting to ensures ROIs are valid
        store->gazeLocated = located;
    }
}
