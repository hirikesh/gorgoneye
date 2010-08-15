#ifndef GUIPARAM_H
#define GUIPARAM_H
#include <QCheckBox>
#include <QSlider>
#include "parameter.h"
using namespace std;

class GUICheckBox : public QCheckBox
{
    Q_OBJECT
public:
    GUICheckBox(ModeParam* mp);
signals:
    void valueChanged(int* const, int);
private slots:
    void emitWithPtr(int);
private:
   int* const pValue;
};

class GUISlider : public QSlider
{
    Q_OBJECT
public:
    GUISlider(RangeParam* rp);
    int* getPtr();
signals:
    void valueChanged(int* const, int);
private slots:
    void emitWithPtr(int);
private:
    int* const pValue;
};

#endif // GUIPARAM_H
