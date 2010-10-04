/* BaseDetector:
   Polymorphic class to support multiple
   tracking algorithms under common interface.
   Follows the Strategy design pattern. */

#ifndef DETECTOR_H
#define DETECTOR_H

#include <vector>
#include <string>

class Param;

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
    BaseDetector(const int&, const std::string&);

    virtual bool locate(const cv::Mat& srcImg, cv::Rect& srcRoi);
    const int getType() const;
    const std::string getName() const;
    std::vector<Param*> getParams();
    bool hasParams();

protected:
    const int type;
    const std::string name;
    std::vector<Param*> params;
};

#endif // DETECTOR_H
