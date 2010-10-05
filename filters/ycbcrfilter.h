#ifndef YCBCRFILTER_H
#define YCBCRFILTER_H

#include "basefilter.h"

class YCbCrFilter : public BaseFilter
{
public:
    YCbCrFilter(const std::string& nm, Store* st);
};

#endif // YCBCRFILTER_H
