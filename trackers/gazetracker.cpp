#include <cv.h>
#include <highgui.h>
#include <ml.h>
#include <QDebug>
#include "gazetracker.h"
#include "filters/cannyedgefilter.h"
#include "filters/grayscalefilter.h"
#include "filters/ycbcrfilter.h"
// insert gaze detection algorithm header here!
#include "store.h"

GazeTracker::GazeTracker(Store* st) :
    BaseTracker(st, "Gaze-test")
{
    cannyEdgeFilter = new CannyEdgeFilter(st, 20, 40);
    cannyEdgeFilter->enable();
    filters.push_back(cannyEdgeFilter);

    grayscaleFilter = new GrayscaleFilter(st);
    grayscaleFilter->enable();
    filters.push_back(grayscaleFilter);

    ycbcrFilter = new YCbCrFilter(st);
    ycbcrFilter->enable();
    filters.push_back(ycbcrFilter);

    BaseTracker::initImageModes();
}

void GazeTracker::track()
{

    if(!enabled) return;

    // Preprocessing

    // Filtering
    cannyEdgeFilter->filter(store->eyesImg, store->ignore, store->ignore);
    grayscaleFilter->filter(store->eyesImg, store->gazeImg, store->ignore);
    ycbcrFilter->filter(store->eyesImg, store->ignore, store->ignore);


    // START TEST DECISION TREE ML

    // Data type for gaze feature input
    cv::Mat gazeImg;

    // Convert elements to compatible data type
    store->gazeImg.convertTo(gazeImg, CV_32FC1, 1./255); // NxM matrix
    imshow("test", gazeImg);

    // Convert image to single row matrix repensenting a single gaze input
//    gazeImg.reshape(1, 1); // 1x(N*M) matrix


    // START TEST DESCISION TREE ML

    if(store->calibMode)
    {
        // Create new tree
        static CvDTree* dtree = new CvDTree;




    }
    else
    {




    }




    // END TEST DESCISION TREE ML


return;
#if(TIME_GAZE_TRACKERS)
        double t = (double)cv::getTickCount();
#endif /* TIME_GAZE_TRACKERS */
    located = someDetector->locate(store->eyesImg, store->eyesMsk, store->gazeRoi);
#if(TIME_GAZE_TRACKERS)
    t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
    qDebug() << someDetector->name().c_str() << "speed:" << 1000*t << "ms";
#endif /* TIME_GAZE_TRACKERS */

    if(located) {
        // Postprocessing (none atm)
    }
//     Updating store bool after attempting to ensures ROIs are valid
    store->gazeLocated = located;
}
