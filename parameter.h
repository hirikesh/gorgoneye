#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>

namespace cv
{
    class Mat;
}

class Param
{

public:
    enum paramType {RANGE, RANGE_DBL, MODE, IMG_MODE};
    Param(const std::string&, void*, paramType);
    const char* getName() const;
    void* getValue() const;
    paramType getType() const;
protected:
    const std::string name;
    void* const value;
    const paramType type;
};

template <class T>
class RangeParam : public Param
{
public:
    RangeParam(const std::string& nm, paramType type, T* val, T min, T max, T stp) :
            Param(nm, val, type),
            minimum(min),
            maximum(max),
            step(stp)
    {}
    T getMinimum() {return minimum;}
    T getMaximum() {return maximum;}
    T getStep() {return step;}
private:
    T minimum;
    T maximum;
    T step;
};

class ModeParam : public Param
{
public:
    ModeParam(const std::string&, int*, bool);
    ModeParam(const std::string&, bool*, bool);
    bool isEnabled();
protected:
    bool enabled;
};

class ImageModeParam : public Param
{
public:
    ImageModeParam(const std::string&, cv::Mat*);
    ImageModeParam(const std::string&, bool* en, cv::Mat*);
    bool* getPtrEnabled();
private:
    bool ignore_enabled;
    bool* enabled;
    cv::Mat* image;
    cv::Mat* dstImg;
};

#endif // PARAMETER_H
