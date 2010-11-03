#ifndef BLOBFILTER_H
#define BLOBFILTER_H

#include "basefilter.h"

class BlobFilter : public BaseFilter
{
public:
    BlobFilter(Store* st);

    void filter(const cv::Mat &srcImg, cv::Mat &dstImg, cv::Mat &dstMsk);

private:
    bool visBlobs;
    bool visMask;



    void _filter(const cv::Mat &src);
    void _store(cv::Mat &dstImg, cv::Mat &dstMsk);
    void _visualise();
};

#endif // BLOBFILTER_H
