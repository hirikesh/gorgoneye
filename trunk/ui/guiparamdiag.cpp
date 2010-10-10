#include <QVBoxLayout>
#include <QScrollArea>
#include <QGroupBox>
#include <QDebug>
#include <QFrame>
#include "guiparamdiag.h"
#include "guiparam.h"
#include "parameter.h"
#include <QSpacerItem>
GUIParamDiag::GUIParamDiag(QWidget *parent) :
        QFrame(parent)
{

}

GUIParamDiag::GUIParamDiag(std::vector<Param*> params, QWidget *parent) :
    QFrame(parent),
    paramLayout(new QVBoxLayout(this)),
    paramSpacer(new QSpacerItem(1, 10, QSizePolicy::Expanding, QSizePolicy::Expanding))
{
    for (unsigned int i = 0; i < params.size(); i++)
    {
        if (params[i]->getType() == Param::MODE)
        {
            gparams.push_back(new GUICheckBox(static_cast<ModeParam*>(params[i])));
        }
        else if (params[i]->getType() == Param::RANGE)
        {
            gparams.push_back(new GUISlider(static_cast<RangeParam<int>*>(params[i])));
        }
        else if (params[i]->getType() == Param::RANGE_DBL)
        {
            gparams.push_back(new GUIDSpinBox(static_cast<RangeParam<double>*>(params[i])));
        }
        else if (params[i]->getType() == Param::IMG_MODE)
        {
            gparams.push_back(new GUIRadioButton(static_cast<ImageModeParam*>(params[i])));
        }
    }

    init();
}

GUIParamDiag::~GUIParamDiag()
{
    for (unsigned int i = 0; i < gparams.size(); i++)
    {
        delete gparams[i];
    }
    delete paramLayout;
}

void GUIParamDiag::init()
{
    for (unsigned int i = 0; i < gparams.size(); i++)
    {
        paramLayout->addWidget(gparams[i]);
    }
    paramLayout->addSpacerItem(paramSpacer);
}
