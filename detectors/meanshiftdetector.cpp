#include <cv.h>
#include <math.h>
#include <highgui.h>
#include <QDebug>
#include "meanshiftdetector.h"
#include "parameter.h"
#include "store.h"

using namespace cv;

MeanShiftDetector::MeanShiftDetector(Store* st) :
      BaseDetector(st, "Weighted Ratio Meanshift"),
      enBackProjImg(false),
      histCalibrate(true)
{
    _images.push_back(new ImageModeParam("Back Projected Image", &enBackProjImg, &backProjVisImg, &st->dispImg));
}

bool MeanShiftDetector::locate(const Mat& srcImg, const Mat& srcMsk, Rect& srcRoi)
{

    // Histogram properties ------------------
//    const int hBins = 32; // no. of hue bins
    const float hRanges[] = {0, 180}; // OpenCV implements hue values from 0 to 180
    const float* histRanges[] = {hRanges};
    const int channels[] = {0};

    if (histCalibrate)
    {
        calcWRHist(srcImg, srcRoi, srcMsk, hist);
        // Initial search window
        searchRoi = srcRoi;
        histCalibrate = false;
    }

    // Calculate Back Projection ------------------
    double maxVal = 0;
    minMaxLoc(hist, NULL, &maxVal);
    double scaleHist = maxVal ? 255.0/maxVal : 0;

    calcBackProject(&srcImg,    // array of source images
                    1,          // no. of source images
                    channels,   // list of channels
                    hist,       // destination backproject array
                    backProjImg,// output image of back projection
                    histRanges, // array containing bin boundaries
                    scaleHist,  // scale factor to improve contrast
                    true);      // uniform histogram

    bitwise_and(backProjImg, srcMsk, backProjImg, MatND());

    if(enBackProjImg) {
        Mat backProjImg3[] = {backProjImg, backProjImg, backProjImg};
        merge(backProjImg3, 3, backProjVisImg);
    }


    // Apply Meanshift Algorithm to track face
    meanShift(backProjImg, searchRoi, TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 1.0));

    // Determine New Search Window Size ---------------------
    // width is based on the zeroth moment
    cv::Moments currMoments = moments(backProjImg(searchRoi));
    double newWidth = 1.8*std::sqrt(currMoments.m00/255);

    // height takes into account elliptical shape of head
    double newHeight = 1.2*newWidth;

    double newTLx = searchRoi.x + 0.5*searchRoi.width - newWidth/2;
    double newTLy = searchRoi.y + 0.5*searchRoi.height - newHeight/2;

    // ensure top-left of new window is inside image
    newWidth = std::min(newWidth, newWidth + newTLx);
    newTLx = std::max(newTLx, 0.0);
    newHeight = std::min(newHeight, newHeight + newTLy);
    newTLy = std::max(newTLy, 0.0);
    // ensure bottom-right of new window is inside image
    newWidth = std::min(newWidth, backProjImg.cols - newTLx);
    newHeight = std::min(newHeight, backProjImg.rows - newTLy);

    searchRoi = Rect(int(newTLx + 0.5), int(newTLy + 0.5), int(newWidth + 0.5), int(newHeight + 0.5));

    // determine whether face has been found
    if (searchRoi.area() > srcImg.size().area()/40)
    {
        srcRoi = searchRoi;
        return true;
    }
    else
    {
//        qDebug() << "Failed to determine face using Mean Shift";
        histCalibrate = true;
        return false;
    }
}

void MeanShiftDetector::calcWRHist(const Mat& image, const Rect& faceRoi, const Mat& maskImage, MatND& hist)
{

    // Step 1: Spatially-Weighted Histogram
    int index, nBins = 32;
    const int histSize[] = {nBins};
    double rangeMin = 0, rangeMax = 180;
    hist.create(1, histSize, CV_32F);
    hist = Scalar(0);

    double t = static_cast<double>(32)/(rangeMax - rangeMin);
    double a = t;
    double b = -t*rangeMin;

    Mat faceImage = image(faceRoi);
    Mat faceMask = maskImage(faceRoi);

    for (int y = 0; y < faceImage.rows; y++)
    {
        for (int x = 0; x < faceImage.cols; x++)
        {
            const uchar& maskPix = faceMask.at<uchar>(y,x);
            bool masked = static_cast<bool>(maskPix);
            if (masked)
            {
                const uchar& pix = faceImage.at<uchar>(y, x);
                index = static_cast<int>(static_cast<double>(pix)*a + b);
                double nx = 2*static_cast<double>(x)/faceImage.cols - 1;
                double ny = 2*static_cast<double>(y)/faceImage.rows - 1;
                double r = nx*nx + ny*ny;
                double weight = std::max(1-r, 0.0);
                hist.at<float>(index) += weight;
            }
        }
    }

    // Step 2: Calculate roi of background pixels
    double newWidth = 3.0*faceRoi.width;
    double newHeight = 3.0*faceRoi.height;
    double newTLx = faceRoi.x + 0.5*faceRoi.width - newWidth/2;
    double newTLy = faceRoi.y + 0.5*faceRoi.height - newHeight/2;

    // ensure top-left of roi is inside image
    newWidth = std::min(newWidth, newWidth + newTLx);
    newTLx = std::max(newTLx, 0.0);
    newHeight = std::min(newHeight, newHeight + newTLy);
    newTLy = std::max(newTLy, 0.0);
    // ensure bottom-right of roi is inside image
    newWidth = std::min(newWidth, image.cols - newTLx);
    newHeight = std::min(newHeight, image.rows - newTLy);

    Rect bgRoi = Rect(int(newTLx + 0.5), int(newTLy + 0.5), int(newWidth + 0.5), int(newHeight + 0.5));

    // Step 3: Ratio Histogram
    Mat newMask = Mat(image.size(), CV_8UC1, Scalar(255));
    newMask(faceRoi) = Scalar(0);

    Mat bgImage = image(bgRoi);
    Mat bgMask = newMask(bgRoi);

    MatND rHist;
    rHist.create(1, histSize, CV_32F);
    rHist = Scalar(0);

    for (int y = 0; y < bgImage.rows; y++)
    {
        for (int x = 0; x < bgImage.cols; x++)
        {
            const uchar& maskPix = bgMask.at<uchar>(y,x);
            bool masked = static_cast<bool>(maskPix);
            if (masked)
            {
                const uchar& pix = bgImage.at<uchar>(y, x);
                index = static_cast<int>(static_cast<double>(pix)*a + b);
                double nx = 2*static_cast<double>(x)/faceRoi.width - 1;
                double ny = 2*static_cast<double>(y)/faceRoi.height - 1;
                double r = nx*nx + ny*ny;
                double weight;
                if (r <= 1)
                    weight = 0;
                else
                    weight = r;
                rHist.at<float>(index) += weight;
            }
        }
    }

    // Step 4: Final BG-Weighted Histogram
    std::vector<double> vec(nBins, 0);
    vec[0] = rHist.at<float>(0);
    double min = vec[0];

    for (int i = 1; i < nBins ; i++)
    {
        vec[i] = rHist.at<float>(i);
        if (vec[i] < min)
            min = vec[i];
    }

    for (int i = 0; i < nBins ; i++)
    {
        if (vec[i])
            hist.at<float>(i) = hist.at<float>(i)*min/vec[i];
    }
}
