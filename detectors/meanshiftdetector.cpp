#include <cv.h>
#include <math.h>
#include <QDebug>
#include "meanshiftdetector.h"
#include "parameter.h"
#include "store.h"

using namespace cv;

MeanShiftDetector::MeanShiftDetector(Store* st) :
      BaseDetector(st, "CAMShift"),
      enBackProjImg(false),
      histCalibrate(true)
{
    _images.push_back(new ImageModeParam("Back Projected Image", &enBackProjImg, &backProjVisImg, &st->dispImg));
}

bool MeanShiftDetector::locate(const Mat& srcImg, const Mat& srcMsk, Rect& srcRoi)
{

    // Histogram properties ------------------
    const int hBins = 32; // no. of hue bins
    const float hRanges[] = {0, 180}; // OpenCV implements hue values from 0 to 180
    const float* histRanges[] = {hRanges};
    const int channels[] = {0};

    if (histCalibrate)
    {
        // Calculate histogram if last camshift failed
        Mat srcImgRoi = srcImg(srcRoi);
        Mat maskImgRoi = srcMsk(srcRoi);

        calcWeightedHist(srcImgRoi, maskImgRoi, hist, hBins);

        double newWidth = 3.0*srcRoi.width;
        double newHeight = 3.0*srcRoi.height;
        double newTLx = srcRoi.x + 0.5*srcRoi.width - newWidth/2;
        double newTLy = srcRoi.y + 0.5*srcRoi.height - newHeight/2;

        // ensure top-left of ratio roi is inside image
        newWidth = std::min(newWidth, newWidth + newTLx);
        newTLx = std::max(newTLx, 0.0);
        newHeight = std::min(newHeight, newHeight + newTLy);
        newTLy = std::max(newTLy, 0.0);
        // ensure bottom-right of ratio roi is inside image
        newWidth = std::min(newWidth, srcImg.cols - newTLx);
        newHeight = std::min(newHeight, srcImg.rows - newTLy);

        Rect ratioRoi = Rect(int(newTLx + 0.5), int(newTLy + 0.5), int(newWidth + 0.5), int(newHeight + 0.5));

        // mask out image
        Mat maskImg = Mat(srcImg.size(), CV_8UC1, Scalar(255));
        maskImg(srcRoi) = Scalar(0);

        calcRatioHist(srcImg(ratioRoi), maskImg(ratioRoi), srcRoi, ratioHist, hBins);

        for(int i = 0; i < hBins; i++)
        {
            hist.at<float>(i) = hist.at<float>(i)*_vec[i];
        }

        qDebug() << "Weighted Hist:"
                 << _vec[0]  << _vec[1]  << _vec[2]  << _vec[3]  << _vec[4]  << _vec[5]
                 << _vec[6]  << _vec[7]  << _vec[8]  << _vec[9]  << _vec[10] << _vec[11]
                 << _vec[12] << _vec[13] << _vec[14] << _vec[15] << _vec[16] << _vec[17]
                 << _vec[18] << _vec[19] << _vec[20] << _vec[21] << _vec[22] << _vec[23]
                 << _vec[24] << _vec[25] << _vec[26] << _vec[27] << _vec[28] << _vec[29]
                 << _vec[30] << _vec[31];
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
        qDebug() << "Failed to determine face using Mean Shift";
        histCalibrate = true;
        return false;
    }
}

void MeanShiftDetector::calcWeightedHist(const cv::Mat& image, const cv::Mat& maskImg, MatND& hist, const int nBins)
{
    int index;
    const int histSize[] = {nBins};
    hist.create(1, histSize, CV_32F);
    hist = Scalar(0);

    const Mat* mask = &maskImg;

    double rangeMin = 0, rangeMax = 180;

    double t = static_cast<double>(nBins)/(rangeMax - rangeMin);
    double a = t;
    double b = -t*rangeMin;

    if(!mask)
    {
        for (int y = 0; y < image.rows; y++)
        {
            for (int x = 0; x < image.cols; x++)
            {
                // determine weight of current pix
                double nx = 2*static_cast<double>(x)/image.cols - 1;
                double ny = 2*static_cast<double>(y)/image.rows - 1;
                double r = nx*nx + ny*ny;
                double weight = std::max(1-r, 0.0);
                const uchar& pix = image.at<uchar>(y, x);
                index = static_cast<int>(static_cast<double>(pix)*a + b);
                hist.at<float>(index) += weight;
            }
        }
    }
    else
    {
        for (int y = 0; y < image.rows; y++)
        {
            for (int x = 0; x < image.cols; x++)
            {
                const uchar& maskPix = maskImg.at<uchar>(y,x);
                bool maskI = static_cast<bool>(maskPix);
                if (maskI)
                {
                    const uchar& pix = image.at<uchar>(y, x);
                    index = static_cast<int>(static_cast<double>(pix)*a + b);
                    double nx = 2*static_cast<double>(x)/image.cols - 1;
                    double ny = 2*static_cast<double>(y)/image.rows - 1;
                    double r = nx*nx + ny*ny;
                    double weight = std::max(1-r, 0.0);
                    hist.at<float>(index) += weight;
                }
            }
        }

    }

    std::vector<double> _vtec;
    for (int i = 0; i < nBins ; i++)
    {
        _vtec.push_back(hist.at<float>(i));
    }

//    qDebug() << "Weighted Hist:"
//             << _vtec[0]  << _vtec[1]  << _vtec[2]  << _vtec[3]  << _vtec[4]  << _vtec[5]
//             << _vtec[6]  << _vtec[7]  << _vtec[8]  << _vtec[9]  << _vtec[10] << _vtec[11]
//             << _vtec[12] << _vtec[13] << _vtec[14] << _vtec[15] << _vtec[16] << _vtec[17]
//             << _vtec[18] << _vtec[19] << _vtec[20] << _vtec[21] << _vtec[22] << _vtec[23]
//             << _vtec[24] << _vtec[25] << _vtec[26] << _vtec[27] << _vtec[28] << _vtec[29]
//             << _vtec[30] << _vtec[31];
}

void MeanShiftDetector::calcRatioHist(const Mat& image, const Mat& maskImg, const Rect& fgRoi, MatND& hist, const int nBins)
{
    int index;
    const int histSize[] = {nBins};
    hist.create(1, histSize, CV_32F);
    hist = Scalar(0);

    const Mat* mask = &maskImg;

    // loop is assuming image is 8-bit 1ch
    // CV_Assert(image.type() == CV_8UC1);

    double rangeMin = 0, rangeMax = 180;

    double t = static_cast<double>(nBins)/(rangeMax - rangeMin);
    double a = t;
    double b = -t*rangeMin;

    if(!mask)
    {
        for (int y = 0; y < image.rows; y++)
        {
            for (int x = 0; x < image.cols; x++)
            {
                const uchar& pix = image.at<uchar>(y, x);
                index = static_cast<int>(static_cast<double>(pix)*a + b);
                double nx = 2*static_cast<double>(x)/fgRoi.width - 1;
                double ny = 2*static_cast<double>(y)/fgRoi.height - 1;
                double r = nx*nx + ny*ny;
                double weight;
                if (r <= 1)
                    weight = 0;
                else
                    weight = r;
                hist.at<float>(index) += weight;
            }
        }
    }
    else
    {
        for (int y = 0; y < image.rows; y++)
        {
            for (int x = 0; x < image.cols; x++)
            {
                const uchar& maskPix = maskImg.at<uchar>(y,x);
                bool maskI = static_cast<bool>(maskPix);
                if (maskI)
                {
                    const uchar& pix = image.at<uchar>(y, x);
                    index = static_cast<int>(static_cast<double>(pix)*a + b);
                    double nx = 2*static_cast<double>(x)/fgRoi.width - 1;
                    double ny = 2*static_cast<double>(y)/fgRoi.height - 1;
                    double r = nx*nx + ny*ny;
                    double weight;
                    if (r <= 1)
                        weight = 0;
                    else
                        weight = r;

                    hist.at<float>(index) += weight;
                }
            }
        }
    }

    _vec.push_back(hist.at<float>(0));
    double min = _vec[0];
    for (int i = 1; i < nBins ; i++)
    {
        _vec.push_back(hist.at<float>(i));
        if (_vec[i] < min)
        {
            min = _vec[i];
        }
    }

    for (int i = 0; i < nBins ; i++)
    {
        if (_vec[i])
            _vec[i] = min/_vec[i];
        else
            _vec[i] = 1;
    }

//    qDebug() << "Ratio Hist Weights:"
//             << _vec[0]  << _vec[1]  << _vec[2]  << _vec[3]  << _vec[4]  << _vec[5]
//             << _vec[6]  << _vec[7]  << _vec[8]  << _vec[9]  << _vec[10] << _vec[11]
//             << _vec[12] << _vec[13] << _vec[14] << _vec[15] << _vec[16] << _vec[17]
//             << _vec[18] << _vec[19] << _vec[20] << _vec[21] << _vec[22] << _vec[23]
//             << _vec[24] << _vec[25] << _vec[26] << _vec[27] << _vec[28] << _vec[29]
//             << _vec[30] << _vec[31];
//    qDebug() << "Minimum Value: " << min;
}
