#ifndef PARAMETER_H
#define PARAMETER_H
#include <string>
#include <cv.h>

using namespace cv;

class Param
{    

public:
    enum paramType {RANGE, RANGE_DBL, MODE, IMG_MODE};
    Param(const string&, void*, paramType);
    const char* getName() const;
    void* getValue() const;
    paramType getType() const;
protected:
    const string name;
    void* const value;
    const paramType type;
};

template <class T>
class RangeParam : public Param
{
public:
    RangeParam(const string& nm, paramType type, T* val, T min, T max, T stp) :
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
    ModeParam(const string&, int*, bool);
    bool isEnabled();
protected:
    bool enabled;
};

class ImageModeParam : public Param
{
public:
    ImageModeParam(const string&, Mat*);
    bool isEnabled();
    void setEnable(bool);
private:
    bool enabled;
    Mat* image;
};

#endif // PARAMETER_H