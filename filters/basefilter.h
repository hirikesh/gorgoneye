#ifndef BASEFILTER_H
#define BASEFILTER_H

#include <cv.h>
#include <vector>
#include <string>

class Store;
class Param;

namespace cv
{
    class Mat;
    template <class T>
    class Rect_;
    typedef Rect_<int> Rect;
}

class BaseFilter
{
public:
    BaseFilter(Store* st, const std::string& nm);
    const std::string name() const;
    bool isEnabled() const;
    void enable();
    void disable();

    std::vector<Param*> params() const;
    std::vector<Param*> images() const;

    virtual void filter(const cv::Mat& srcImg, cv::Mat& dstImg, cv::Mat& dstMsk) = 0;
//    virtual void filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Rect& srcRoi, cv::Rect& dstRoi) = 0;

protected:
    // performs filtering operations on the source image
    // storing result in temporary matrix
    virtual void _filter(const cv::Mat& src) = 0;
    // stores the temporary result of the filtering operation
    // into the destination image and/or masks
    virtual void _store(cv::Mat& dstImg, cv::Mat& dstMsk) = 0;
    // performs necessary colour-space conversions in order
    // to display channels or images in BGR or store the
    // resulting images in the correct image depth
    virtual void _visualise() = 0;
    // helper function used by _visualise() and filter() to
    // achieve the same aim as _visualise()
//    virtual void _visualise3ch();

    Store* imageStore;
    const std::string filterName;
    bool enabled;

    std::vector<Param*> _params;
    std::vector<Param*> _images;
};

#endif // BASEFILTER_H
