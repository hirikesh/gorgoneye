#include "parameter.h"

using std::string;
using cv::Mat;

// Param *******************************************************
Param::Param(const string& s, void* val, paramType t) :
    name(s),
    value(val),
    type(t)
{}

const char* Param::getName() const
{
    return name.c_str();
}

void* Param::getValue() const
{
    return value;
}

Param::paramType Param::getType() const
{
    return type;
}

// ModeParam *******************************************************
ModeParam::ModeParam(const string& s, int* val, bool en) :
        Param(s, val, Param::MODE),
        enabled(en)
{}

bool ModeParam::isEnabled()
{
    return enabled;
}

// ImageModeParam **************************************************
ImageModeParam::ImageModeParam(const string &s, Mat* val) :
        Param(s, val, Param::IMG_MODE)
{
    bool ignore_enabled;
    enabled = &ignore_enabled;
}

ImageModeParam::ImageModeParam(const string &s, bool* en, Mat* val) :
        Param(s, val, Param::IMG_MODE),
        enabled(en)
{}

bool* ImageModeParam::getPtrEnabled()
{
    return enabled;
}
