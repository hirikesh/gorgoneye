#include <QDebug>
#include <QCheckBox>
#include <QSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include <QRadioButton>

#include "guiparam.h"

GUICheckBox::GUICheckBox(const std::string& title, bool* value) :
    QCheckBox(title.c_str()),
    pValue(value)
{
    init();
}

GUICheckBox::GUICheckBox(ModeParam* mp) :
    QCheckBox(mp->name()),
    pValue(static_cast<bool*>(mp->value()))
{
    init();
}

void GUICheckBox::init()
{
    setChecked(*pValue);
    connect(this, SIGNAL(toggled(bool)), this, SLOT(setParamValue(bool)));
}

void GUICheckBox::setParamValue(bool value)
{
   *pValue = value;
}

GUISlider::GUISlider(RangeParam<int>* rp) :
        QFrame(),
        slider(new QSlider(Qt::Horizontal, this)),
        spinbox(new QSpinBox(this)),
        title(new QLabel(rp->name())),
        layout(new QGridLayout(this)),
        pValue(static_cast<int*>(rp->value()))
{
    slider->setRange(rp->getMinimum(), rp->getMaximum());
    slider->setSingleStep(rp->getStep());
    slider->setValue(*pValue);

    spinbox->setRange(rp->getMinimum(), rp->getMaximum());
    spinbox->setSingleStep(rp->getStep());
    spinbox->setValue(slider->value());

    layout->addWidget(title, 0, 0, 1, 2);
    layout->addWidget(slider, 1, 0);
    layout->addWidget(spinbox, 1, 1);

    layout->setMargin(0);
    layout->setSpacing(2);

    connect(spinbox, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));
    connect(slider, SIGNAL(valueChanged(int)), spinbox, SLOT(setValue(int)));
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(setParamValue(int)));
}

void GUISlider::setParamValue(int value)
{
   *pValue = value;
}

GUIDSpinBox::GUIDSpinBox(RangeParam<double>* rp) :
        QFrame(),
        spinbox(new QDoubleSpinBox(this)),
        title(new QLabel(rp->name())),
        layout(new QGridLayout(this)),
        pValue(static_cast<double*>(rp->value()))
{
    spinbox->setRange(rp->getMinimum(), rp->getMaximum());
    spinbox->setSingleStep(rp->getStep());
    spinbox->setDecimals(3);
    spinbox->setValue(*pValue);

    layout->addWidget(title, 0, 0);
    layout->addWidget(spinbox, 1, 0);

    layout->setMargin(0);
    layout->setSpacing(2);

    connect(spinbox, SIGNAL(valueChanged(double)), this, SLOT(setParamValue(double)));
}

void GUIDSpinBox::setParamValue(double value)
{
    *pValue = value;
}

GUIRadioButton::GUIRadioButton(ImageModeParam* imp) :
        QRadioButton(imp->name()),
        pValue(static_cast<cv::Mat*>(imp->value())),
        enPValue(static_cast<bool*>(imp->getPtrEnabled())),
        dispImg(imp->getDstImgPtr())
{
    if (*dispImg == pValue)
    {
        this->setChecked(true);
    }
    else
    {
        this->setChecked(false);
    }
    connect(this, SIGNAL(toggled(bool)), this, SLOT(setParamValues(bool)));
}

void GUIRadioButton::setParamValues(bool state)
{
    *enPValue = state;
    if (state)
    {
        *dispImg = pValue;
    }
}
