#include "parameter.h"

// Param *******************************************************
Param::Param(const string& name, int* val, paramType type) :
        pName(name),
        pValue(val),
        pType(type)
{}

string Param::getName() const
{
    return pName;
}

int* Param::getValue() const
{
    return pValue;
}

Param::paramType Param::getType() const
{
    return pType;
}

// RangeParam *******************************************************
RangeParam::RangeParam(const string& name, int* val, int min, int max, int stp) :
        Param(name, val, Param::RANGE),
        minimum(min),
        maximum(max),
        step(stp)
{}

int RangeParam::getMinimum()
{
    return minimum;
}

int RangeParam::getMaximum()
{
    return maximum;
}

int RangeParam::getStep()
{
    return step;
}

// ModeParam *******************************************************
ModeParam::ModeParam(const string& name, int* val, bool en) :
        Param(name, val, Param::MODE),
        enabled(en)
{}

bool ModeParam::isEnabled()
{
    return enabled;
}
