#include <QCheckBox>
#include <QSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include <QRadioButton>

#include "guiparam.h"

GUICheckBox::GUICheckBox(const std::string& title, bool* value) :
    checkbox(new QCheckBox(title.c_str(), this)),
    pValue(value)
{
    init();
}

GUICheckBox::GUICheckBox(ModeParam* mp) :
    checkbox(new QCheckBox(mp->name(), this)),
    pValue(static_cast<bool*>(mp->value()))
{
    init();
}

void GUICheckBox::init()
{
    layout = new QGridLayout(this);
    layout->addWidget(checkbox);
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
        title(new QLabel(rp->name())),
        layout(new QGridLayout(this)),
        pValue(static_cast<int*>(rp->value()))
{
    slider->setRange(rp->getMinimum(), rp->getMaximum());
    slider->setSingleStep(rp->getStep());
    slider->setValue(*pValue);

    spinbox->setRange(rp->getMinimum(), rp->getMaximum());
    spinbox->setValue(slider->value());

    layout->addWidget(title, 0, 0);
    layout->addWidget(slider, 1, 0);
    layout->addWidget(spinbox, 1, 1);

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
        title(new QLabel(rp->name())),
        layout(new QGridLayout(this)),
        pValue(static_cast<double*>(rp->value()))
{

    spinbox->setRange(rp->getMinimum(), rp->getMaximum());
    spinbox->setSingleStep(rp->getStep());
    spinbox->setValue(*pValue);

    layout->addWidget(title, 0, 0);
    layout->addWidget(spinbox, 0, 1);

    connect(spinbox, SIGNAL(valueChanged(double)), this, SLOT(setParamValue(double)));
}

void GUIDSpinBox::setParamValue(double value)
{
    *pValue = value;
}

GUIRadioButton::GUIRadioButton(ImageModeParam* imp) :
        QRadioButton(imp->name()),
        //layout(new QGridLayout(this)),
        radioButton(new QRadioButton(imp->name())),
        pValue(static_cast<cv::Mat*>(imp->value())),
        enPValue(static_cast<bool*>(imp->getPtrEnabled())),
        dispImg(imp->getDstImgPtr())
{
        //layout->addWidget(radioButton);
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
