#include "parameter.h"

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
ModeParam::ModeParam(const string& s, bool* val, bool en) :
        Param(s, val, Param::MODE),
        enabled(en)
{}

bool ModeParam::isEnabled()
{
    return enabled;
}
