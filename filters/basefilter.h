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
    virtual void filter(const cv::Mat& src, cv::Mat& dst, const cv::Mat& mask) = 0;
    virtual void filter(const cv::Mat& src, cv::Mat& dst, const cv::Rect& roi) = 0;
private:
    const std::string filterName;
    Store* imageStore;
    std::vector<Param*> filterParams;

};

#endif // BASEFILTER_H
