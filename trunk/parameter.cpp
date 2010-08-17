#include "parameter.h"

// Param *******************************************************
Param::Param(const string& nm, void* val, paramType t) :
        name(nm),
        value(val),
        type(t)
{}

string Param::getName() const
{
    return name;
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
ModeParam::ModeParam(const string& name, bool* val, bool en) :
        Param(name, val, Param::MODE),
        enabled(en)
{}

bool ModeParam::isEnabled()
{
    return enabled;
}
