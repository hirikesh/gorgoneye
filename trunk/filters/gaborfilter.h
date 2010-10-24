#ifndef GABORFILTER_H
#define GABORFILTER_H

#include "basefilter.h"

class GaborFilter : public BaseFilter
{
public:
    GaborFilter(Store* st);

    void filter(const cv::Mat& srcImg, cv::Mat& dstImg, cv::Mat& dstMsk);
    void _filter(const cv::Mat& src);
    void _store(cv::Mat& dstImg, cv::Mat& dstMsk);
    void _visualise();
private:
    cv::Mat kernel;
//    cv::Mat kernelVis;
    cv::Mat tmpGabor;

    int kernelSize;

    int avar;
    int pulse;
    int degPhase;
    int degPsi;

    double bw;
    int psi;
    int lambda;
    int degTheta;
    double gamma;
};

#endif // GABORFILTER_H
