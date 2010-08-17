#ifndef GUIPARAM_H
#define GUIPARAM_H
#include <QCheckBox>
#include <QSlider>
#include <QDoubleSpinBox>
#include "parameter.h"
using namespace std;

class GUICheckBox : public QCheckBox
{
    Q_OBJECT
public:
    GUICheckBox(ModeParam* mp);
signals:
    void valueChanged(bool* const, bool);
private slots:
    void emitWithPtr(int);
private:
   bool* const pValue;
};

class GUISlider : public QSlider
{
    Q_OBJECT
public:
    GUISlider(RangeParam<int>* rp);
    int* getPtr();
signals:
    void valueChanged(int* const, int);
private slots:
    void emitWithPtr(int);
private:
    int* const pValue;
};

class GUIDSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
public:
    GUIDSpinBox(RangeParam<double>* rp);
    double* getPtr();
signals:
    void valueChanged(double* const, double);
private slots:
    void emitWithPtr(double);
private:
    double* const pValue;
};

#endif // GUIPARAM_H
