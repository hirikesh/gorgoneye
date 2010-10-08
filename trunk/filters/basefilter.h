#ifndef BASEFILTER_H
#define BASEFILTER_H

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
    virtual bool hasParams() const = 0;
    std::vector<Param*> params() const;
    const std::string name() const;
    virtual void filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Mat& srcMsk, cv::Mat& dstMsk) = 0;
    virtual void filter(const cv::Mat& srcImg, cv::Mat& dstImg, const cv::Rect& srcRoi, cv::Rect& dstRoi) = 0;
    void enable();
    void disable();
protected:
    const std::string filterName;
    Store* imageStore;
    std::vector<Param*> filterParams;
    bool enabled;
};

#endif // BASEFILTER_H
