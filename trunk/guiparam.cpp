#include "guiparam.h"

GUICheckBox::GUICheckBox(ModeParam* mp) :
        QCheckBox(mp->getName().c_str()),
        pValue(mp->getValue())
{
    connect(this, SIGNAL(stateChanged(int)), this, SLOT(emitWithPtr(int)));
}

void GUICheckBox::emitWithPtr(int state)
{
    emit valueChanged(pValue, state);
}

GUISlider::GUISlider(RangeParam* rp) :
        QSlider(Qt::Horizontal),
        pValue(rp->getValue())
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
