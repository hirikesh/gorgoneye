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
    BaseFilter(const std::string& nm, Store* st);
    const std::string name() const;
    bool isEnabled() const;
    void enable();
    void disable();

    virtual bool hasParams() const = 0;
    std::vector<Param*> params() const;

    virtual void filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Mat& srcMsk, cv::Mat& dstMsk) = 0;
//    virtual void filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Rect& srcRoi, cv::Rect& dstRoi) = 0;

protected:
    virtual void _filter(const cv::Mat& src, cv::Mat& dst) = 0;
    virtual void _visualise() = 0;

    const std::string filterName;
    Store* imageStore;
    bool enabled;
    std::vector<Param*> filterParams;
};

#endif // BASEFILTER_H
