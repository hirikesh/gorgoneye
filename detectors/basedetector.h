/* BaseDetector:
   Polymorphic class to support multiple
   tracking algorithms under common interface.
   Follows the Strategy design pattern. */

#ifndef DETECTOR_H
#define DETECTOR_H

#include <vector>
#include <string>

class Param;
class Store;

namespace cv
{
    class Mat;
    template <class T>
    class Rect_;
    typedef Rect_<int> Rect;
}

class BaseDetector
{
public:
    BaseDetector(Store *st, const std::string&);

    virtual bool locate(const cv::Mat& srcImg, const cv::Mat& srcMsk, cv::Rect& srcRoi);
    const std::string name() const;
    std::vector<Param*> params() const;
    std::vector<Param*> images() const;

protected:
    const std::string _name;
    std::vector<Param*> _params;
    std::vector<Param*> _images;
};

#endif // DETECTOR_H
