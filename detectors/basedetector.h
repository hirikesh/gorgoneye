/* BaseDetector:
   Polymorphic class to support multiple
   tracking algorithms under common interface.
   Follows the Strategy design pattern. */

#ifndef DETECTOR_H
#define DETECTOR_H

#include <vector>
#include "parameter.h"

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
    BaseDetector(const std::string&);

    virtual bool locate(const cv::Mat& srcImg, cv::Rect& srcRoi);
    std::vector<Param*> getParams();
    const std::string getName() const;

protected:
    const std::string name;
    std::vector<Param*> params;
};

#endif // DETECTOR_H
