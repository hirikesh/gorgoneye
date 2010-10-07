#include "guiparam.h"
#include <QCheckBox>
#include <QSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include <QDebug>

GUICheckBox::GUICheckBox(const std::string& title, bool* value) :
    QFrame(),
    checkbox(new QCheckBox(title.c_str(), this)),
    layout(new QGridLayout(this)),
    pValue(value)
{
    layout->addWidget(checkbox);
    this->setLayout(layout);
    checkbox->setChecked(*pValue);
    QObject::connect(checkbox, SIGNAL(toggled(bool)), this, SLOT(setParamValue(bool)));
}

GUICheckBox::GUICheckBox(ModeParam* mp) :
    QFrame(),
    checkbox(new QCheckBox(mp->getName(), this)),
    layout(new QGridLayout(this)),
    pValue(static_cast<bool*>(mp->getValue()))
{
    layout->addWidget(checkbox);
    this->setLayout(layout);
    checkbox->setParent(this);
    checkbox->setChecked(*pValue);
    QObject::connect(checkbox, SIGNAL(toggled(bool)), this, SLOT(setParamValue(bool)));
}

void GUICheckBox::setParamValue(bool value)
{
   *pValue = value;
}

GUISlider::GUISlider(RangeParam<int>* rp) :
        QFrame(),
        slider(new QSlider(Qt::Horizontal, this)),
        spinbox(new QSpinBox(this)),
        title(new QLabel(rp->getName())),
        layout(new QGridLayout(this)),
        pValue(static_cast<int*>(rp->getValue()))
{
    slider->setRange(rp->getMinimum(), rp->getMaximum());
    slider->setSingleStep(rp->getStep());
    slider->setValue(*pValue);
    spinbox->setRange(rp->getMinimum(), rp->getMaximum());
    spinbox->setValue(slider->value());

    layout->addWidget(title, 0, 0);
    layout->addWidget(slider, 1, 0);
    layout->addWidget(spinbox, 1, 1);

    this->setLayout(layout);

    QObject::connect(spinbox, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));
    QObject::connect(slider, SIGNAL(valueChanged(int)), spinbox, SLOT(setValue(int)));
    QObject::connect(slider, SIGNAL(valueChanged(int)), this, SLOT(setParamValue(int)));
}

void GUISlider::setParamValue(int value)
{
   *pValue = value;
}

GUIDSpinBox::GUIDSpinBox(RangeParam<double>* rp) :
        QFrame(),
        spinbox(new QDoubleSpinBox(this)),
        title(new QLabel(this)),
        layout(new QGridLayout(this)),
        pValue(static_cast<double*>(rp->getValue()))
{

    spinbox->setRange(rp->getMinimum(), rp->getMaximum());
    spinbox->setSingleStep(rp->getStep());
    spinbox->setValue(*pValue);

    title->setText(rp->getName());

    layout->addWidget(title, 0, 0);
    layout->addWidget(spinbox, 0, 1);

    connect(spinbox, SIGNAL(valueChanged(double)), this, SLOT(setParamValue(double)));
}

void GUIDSpinBox::setParamValue(double value)
{
    *pValue = value;
}

GUIRadioButton::GUIRadioButton(ImageModeParam* imp) :
        QRadioButton(imp->getName()),
        pValue(static_cast<cv::Mat*>(imp->getValue())),
        enPValue(static_cast<bool*>(imp->getPtrEnabled()))
{
        connect(this, SIGNAL(toggled(bool)), this, SLOT(emitWithPtr(bool)));
}

void GUIRadioButton::emitWithPtr(bool state)
{
    emit valueChanged(pValue, state);
    emit enableChanged(enPValue, state);
}

GUITrackerComboBox::GUITrackerComboBox(BaseTracker* trkr) :
        QComboBox(),
        tracker(trkr)
{
    connect(this, SIGNAL(activated(int)), this, SLOT(setDetector(int)));
}

void GUITrackerComboBox::setDetector(int type)
{
    tracker->setDetector(type);
}
