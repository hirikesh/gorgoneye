#include <cv.h>
#include <highgui.h>
#include <QDebug>
#include "gaborfilter.h"
#include "parameter.h"

using cv::Mat;
using cv::Scalar;

GaborFilter::GaborFilter(Store* st) :
        BaseFilter(st, "Gabor"),
        kernelSize(7), // n x n odd kernel
        avar(50),// variance
        pulse(5),// pulsation
        degPhase(0),   // phase
        degPsi(90),
        bw(1.),
        psi(0),
        lambda(4),
        degTheta(45),
        gamma(1.25)
{
    _params.push_back(new RangeParam<double>("Bandwidth", Param::RANGE_DBL, &bw, 1, 7, 0.5));
    _params.push_back(new RangeParam<int>("Phase (Degrees)", Param::RANGE, &degTheta, 0, 360, 5));
    _params.push_back(new RangeParam<int>("Psi (Degrees)", Param::RANGE, &psi, 0, 360, 5));
    _params.push_back((new RangeParam<int>("WaveLength", Param::RANGE, &lambda, 2, 12, 1)));
    _params.push_back((new RangeParam<double>("Aspect Ratio (Gamma)", Param::RANGE_DBL, &gamma, 0.25, 4, 0.25)));

    qDebug() << "Initialising Gabor Filter ...";

    if (kernelSize % 2 == 0) // prevent crash on even size
        kernelSize++;

    kernel = Mat(kernelSize, kernelSize, CV_32F, Scalar(0));
}
void GaborFilter::filter(const Mat& srcImg, Mat& dstImg, Mat& dstMsk)
{
    if (!enabled)
        return;


    float kernelVal;

    float theta = degTheta*CV_PI/180;

    float sig = lambda/CV_PI*std::sqrt(0.5*std::log(2))*((std::pow(2, bw)+1)/(std::pow(2, bw)-1));

    float isigx  = 1/sig;
    float isigy = isigx*gamma;

    float cosT = std::cos(theta);
    float sinT = std::sin(theta);

    for (int y = -kernelSize/2; y <= kernelSize/2; y++)
    {
        for (int x = -kernelSize/2; x <= kernelSize/2; x++)
        {
            float flipy = -y;
            float x_theta = x*cosT + flipy*sinT;
            float y_theta = -x*sinT + flipy*cosT;
            float nx = x_theta*isigx;
            float ny = y_theta*isigy;
            kernelVal = std::exp(-0.5*(nx*nx+ny*ny))*std::cos(2*CV_PI/lambda*x_theta+psi);
            kernel.at<float>(y + kernelSize/2, x + kernelSize/2) = kernelVal;
//            qDebug().nospace() << "(" << y << "," << x << ") : " << kernelVal;
        }
    }

    _filter(srcImg);
    _store(dstImg, dstMsk);
//    _visualise();
}

void GaborFilter::_filter(const Mat& src)
{
    cv::flip(kernel, kernel, 1);
    cv::filter2D(src, tmpGabor, src.depth(), kernel);
}

void GaborFilter::_store(Mat& dstImg, Mat& dstMsk)
{
    tmpGabor.copyTo(dstImg, dstMsk);
}

void GaborFilter::_visualise()
{
    Mat kernelVis = 0.5*kernel + 0.5;
    cv::Mat tmp(cv::Size(100, 100), kernelVis.depth());
    cv::resize(kernelVis, tmp, tmp.size(), 0, 0, cv::INTER_NEAREST);
    imshow("Kernel Visualisation", tmp);
}
