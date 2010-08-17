#include "guiparam.h"

GUICheckBox::GUICheckBox(ModeParam* mp) :
        QCheckBox(mp->getName().c_str()),
        pValue((bool*)mp->getValue())
{
    connect(this, SIGNAL(stateChanged(int)), this, SLOT(emitWithPtr(int)));
}

void GUICheckBox::emitWithPtr(int state)
{
    emit valueChanged(pValue, (bool)state);
}

GUISlider::GUISlider(RangeParam<int>* rp) :
        QSlider(Qt::Horizontal),
        pValue((int*)rp->getValue())
{
    setRange(rp->getMinimum(), rp->getMaximum());
    setSingleStep(rp->getStep());
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(emitWithPtr(int)));
}

int* GUISlider::getPtr()
{
    return pValue;
}

void GUISlider::emitWithPtr(int state)
{
    emit valueChanged(pValue, state);
}

GUIDSpinBox::GUIDSpinBox(RangeParam<double>* rp) :
        QDoubleSpinBox(),
        pValue((double*)rp->getValue())
{
    setRange(rp->getMinimum(), rp->getMaximum());
    setSingleStep(rp->getStep());
    connect(this, SIGNAL(valueChanged(double)), this, SLOT(emitWithPtr(double)));
}

double* GUIDSpinBox::getPtr()
{
    return pValue;
}

void GUIDSpinBox::emitWithPtr(double state)
{
    emit valueChanged(pValue, state);
}
