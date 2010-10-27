#include <math.h>
#include <cv.h>
#include <highgui.h>
#include "dognormfilter.h"
#include "parameter.h"
#include "store.h"
#include <QDebug>

DoGNormFilter::DoGNormFilter(Store* st, int ks, double ls, double us, int sm, int lt, int ut, double b) :
    BaseFilter(st, "Normalisation"),
    kernelSize(ks), lowerSigma(ls), upperSigma(us),
    scaleMethod(sm), lowerThresh(lt), upperThresh(ut), beta(b)
{
    _images.push_back(new ImageModeParam("Normalised image", &visDoGImg, &st->dispImg));
    _params.push_back(new RangeParam<int>("Gaussian Kernel Size", Param::RANGE, &kernelSize, 3, 9, 2));
    _params.push_back(new RangeParam<double>("Smaller Guassian Sigma", Param::RANGE_DBL, &lowerSigma, 0.5, 10, 0.5));
    _params.push_back(new RangeParam<double>("Larger Guassian Sigma", Param::RANGE_DBL, &upperSigma, 0.5, 10, 0.5));
    _params.push_back(new RangeParam<int>("Scaling method", Param::RANGE, &scaleMethod, 0, 2, 1));
    _params.push_back(new RangeParam<int>("Lower Threshold", Param::RANGE, &lowerThresh, -100, 100, 1));
    _params.push_back(new RangeParam<int>("Upper Threshold", Param::RANGE, &upperThresh, -100, 100, 1));
    _params.push_back(new RangeParam<double>("Sigmoid exponent scale", Param::RANGE_DBL, &beta, 0.1, 10, 0.1));

    // Create Difference of Gaussians kernel
    kernelSize = 7; // must be odd
    double lowVar = 1;
    double highVar = 4;

    cv::Mat lowKernel = cv::Mat(kernelSize, kernelSize, CV_64F, cv::Scalar(0));
    cv::Mat highKernel = cv::Mat(kernelSize, kernelSize, CV_64F, cv::Scalar(0));
    double lowKernelSum = 0, highKernelSum = 0;
    double kernelVal;

    for (int y = -kernelSize/2; y <= kernelSize/2; y++)
    {
        for (int x = -kernelSize/2; x <= kernelSize/2; x++)
        {
            kernelVal = std::exp(-(x*x + y*y)/(2*lowVar));
            lowKernelSum += kernelVal;
            lowKernel.at<double>(y + kernelSize/2, x + kernelSize/2) = kernelVal;
            kernelVal = std::exp(-(x*x + y*y)/(2*highVar));
            highKernelSum += kernelVal;
            highKernel.at<double>(y + kernelSize/2, x + kernelSize/2) = kernelVal;
        }
    }

    lowKernel /= lowKernelSum; // normalise to 1

    highKernel /= highKernelSum; // normalise to 1
    doGKernel = lowKernel - highKernel;
}

void DoGNormFilter::filter(const cv::Mat& srcImg, cv::Mat& dstImg, cv::Mat& dstMsk)
{
    // Stop here if disabled
    if(!enabled) return;

    // Corner detection operation
    _filter(srcImg);

    // Store result
    _store(dstImg, dstMsk);

    // Visualise on request
    _visualise();
}

void DoGNormFilter::setParams()
{

}

void DoGNormFilter::_filter(const cv::Mat& src)
{
    // Convert to normalised grayscale and gamma correct

//    cv::Mat malcFace = cv::imread("malc-face-1.png");
//    imshow("Malcolm Face Image", malcFace);
//    cv::Mat src = malcFace;
    cv::Mat grayChannel;
    if(src.type() == CV_8UC1)
        grayChannel = src;
    else
        cvtColor(src, grayChannel, CV_BGR2GRAY);

    cv::Mat tmpSrc(src.size(), CV_32FC1);
    grayChannel.convertTo(tmpSrc, CV_32FC1, 1./255);
    pow(tmpSrc, 0.2, tmpSrc);

    // convolute image with DoG kernel
    cv::filter2D(tmpSrc, doGImg, CV_32FC1, (doGKernel), cv::Point(-1, -1), 0, cv::BORDER_REPLICATE);
//    cv::Scalar meanImg;
//    meanImg = cv::mean(doGImg);
    //qDebug() << "Mean" << meanImg[0];
    // Scale pixel intensities to improve contrast
    switch(scaleMethod)
    {
    case MEDIAN:
        {
//            double min, max;
//            cv::minMaxLoc(doGImg, &min, &max);
//            qDebug() << "doGImg Before Scale: " << min << max;
            cv::Mat tmpImg;
            pow(abs(doGImg), 0.1, tmpImg);
//            cv::minMaxLoc(tmpImg, &min, &max);
//            qDebug() << "tmpImg After Scale: " << min << max;
            doGImg /= pow(mean(tmpImg)[0], 10);
//            cv::minMaxLoc(doGImg, &min, &max);
//            qDebug() << "doGImg After Scale: " << min << max;
        }
        break;
    case MEAN:
        {
            doGImg /= mean(doGImg)[0];
        }
        break;
    }

//    double min, max;
//    cv::minMaxLoc(doGImg, &min, &max);
//    qDebug() << "doGImg After Scale: " << min << max;

    // Threshold pixel values
    cv::Mat lowerMask;
    compare(doGImg, (double)lowerThresh, lowerMask, cv::CMP_LE);
    doGImg.setTo(cv::Scalar((float)lowerThresh), lowerMask);
    cv::Mat upperMask;
    compare(doGImg, (double)upperThresh, upperMask, cv::CMP_GE);
    doGImg.setTo(cv::Scalar((float)upperThresh), upperMask);
//    cv::threshold(doGImg, doGImg, (double)upperThresh, NULL, cv::THRESH_TRUNC);

    // Pass through sigmoid
    exp(doGImg, doGImg);
    pow(doGImg, -1./beta, doGImg);
    doGImg = 1./(1 + doGImg);

    // Shift and scale values into valid pixel intensities
    cv::normalize(doGImg, doGImg, 0, 255, cv::NORM_MINMAX);
    doGImg.convertTo(doGImg, CV_8UC1);
}

void DoGNormFilter::_store(cv::Mat& dstImg, cv::Mat& dstMsk)
{
    // Store conversion result
    if(dstImg.data)
    {
        if(dstImg.type() == CV_8UC1)
            doGImg.copyTo(dstImg);
        else
            cvtColor(doGImg, dstImg, CV_GRAY2BGR);
    }
}

void DoGNormFilter::_visualise()
{
    // Visualise result on request
    if(visDoG)
        cvtColor(doGImg, visDoGImg, CV_GRAY2BGR);
}
