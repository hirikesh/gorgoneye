#include <cv.h>
#include <highgui.h>
#include <QDebug>
#include "gaborfilter.h"
#include "parameter.h"

using cv::Mat;
using cv::Scalar;

GaborFilter::GaborFilter(Store* st) :
        BaseFilter(st, "Gabor"),
        kernelSize(3), // n x n odd kernel
        avar(50),// variance
        pulse(5),// pulsation
        degPhase(0),   // phase
        degPsi(90)
{
    //_images.push_back(new ImageModeParam("Canny edges", &visCanny, &visCannyImg, &st->dispImg));
    _params.push_back(new RangeParam<int>("Variance", Param::RANGE, &avar, 1, 200, 1));
    _params.push_back(new RangeParam<int>("Phase (Degrees)", Param::RANGE, &degPhase, 0, 360, 1));
    _params.push_back(new RangeParam<int>("Psi (Degrees)", Param::RANGE, &degPsi, 0, 360, 1));
    _params.push_back((new RangeParam<int>("Kernel Size (Odd)", Param::RANGE, &kernelSize, 3, 7, 2)));

}
void GaborFilter::filter(const Mat& srcImg, Mat& dstImg, Mat& dstMsk)
{
    if (!enabled)
        return;

    // TODO: Only refresh kernel on parameter change;
    float var = 0.1*avar;
    float w = 0.1*pulse;
    float phase = degPhase*CV_PI/180;
    float psi = degPsi*CV_PI/180;

    if (kernelSize % 2 == 0) // prevent crash on even size
        kernelSize++;

    kernel = Mat(kernelSize, kernelSize, CV_32F, Scalar(0));
    float kernelVal;

    for (int x = -kernelSize/2; x <= kernelSize/2; x++)
    {
        for (int y = -kernelSize/2; y <= kernelSize/2; y++)
        {
            kernelVal = exp(-((x*x)+(y*y))/(2*var))*cos(w*x*cos(phase)+w*y*sin(phase)+psi);
            kernel.at<float>(x + kernelSize/2,y + kernelSize/2) = kernelVal;
//            qDebug().nospace() << "(" << x << "," << y << ") : " << kernelVal;
        }

    }

    _filter(srcImg);
    _store(dstImg, dstMsk);
    _visualise();
}

void GaborFilter::_filter(const Mat& src)
{
    cv::filter2D(src, tmpGabor, src.depth(), kernel);


}

void GaborFilter::_store(Mat& dstImg, Mat& dstMsk)
{
    tmpGabor.copyTo(dstImg, dstMsk);
}

void GaborFilter::_visualise()
{
    // should be at least able to see a picture of the kernel.
    Mat tmp(cv::Size(400, 400), kernel.depth());
    Mat kernelVis;
    cv::add(kernel, Scalar(0.5), kernelVis);
    cv::resize(kernelVis, tmp, tmp.size());
    imshow("Kernel Visualisation", tmp);
}
