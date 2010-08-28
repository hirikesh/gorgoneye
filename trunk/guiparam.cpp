#include "guiparam.h"

GUICheckBox::GUICheckBox(const string& title, bool* value) :
        QCheckBox(title.c_str()),
        pValue(value)
{
    connect(this, SIGNAL(stateChanged(int)), this, SLOT(emitWithPtr(int)));
}

GUICheckBox::GUICheckBox(ModeParam* mp) :
        QCheckBox(mp->getName()),
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
    setValue(*pValue);
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(emitWithPtr(int)));
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
    setValue(*pValue);
    connect(this, SIGNAL(valueChanged(double)), this, SLOT(emitWithPtr(double)));
}

void GUIDSpinBox::emitWithPtr(double state)
{
    emit valueChanged(pValue, state);
}

GUIRadioButton::GUIRadioButton(ImageModeParam* imp) :
        QRadioButton(imp->getName()),
        pValue((Mat*)imp->getValue())
{
        connect(this, SIGNAL(toggled(bool)), this, SLOT(emitWithPtr(bool)));
}

void GUIRadioButton::emitWithPtr(bool state)
{
    emit valueChanged(pValue, state);
}
