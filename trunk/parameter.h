#ifndef PARAMETER_H
#define PARAMETER_H
#include <string>

using namespace std;
class Param
{    

public:
    enum paramType {RANGE, MODE};
    Param(const string&, int*, paramType);
    string getName() const;
    int* getValue() const;
    paramType getType() const;
protected:
    const string pName;
    int* const pValue;
    const paramType pType;
};


class RangeParam : public Param
{
public:
    RangeParam(const string&, int*, int, int, int);
    int getMinimum();
    int getMaximum();
    int getStep();
private:
    int minimum;
    int maximum;
    int step;
};

class ModeParam : public Param
{
public:
    ModeParam(const string&, int*, bool);
    bool isEnabled();
private:
    bool enabled;
};

#endif // PARAMETER_H
