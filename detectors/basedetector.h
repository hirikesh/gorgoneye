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
    const int type() const;
    const std::string name() const;
    std::vector<Param*> params();
    std::vector<Param*> getImageModes();
    std::vector<Param*>* getPtrParams();

    bool hasParams();

protected:
    const int _type;
    const std::string _name;
    std::vector<Param*> _params;
    std::vector<Param*> imageModes;
};

#endif // DETECTOR_H
