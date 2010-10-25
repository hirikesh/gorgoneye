#include <QVBoxLayout>
#include <QFrame>
#include <QSpacerItem>
#include <QButtonGroup>
#include <QAbstractButton>
#include "guiparam.h"
#include "parameter.h"
#include "guiparamdiag.h"


GUIParamDiag::GUIParamDiag(QButtonGroup* buttonGroup, QWidget *parent) :
        QFrame(parent)
{
}

GUIParamDiag::GUIParamDiag(std::vector<Param*> params, QButtonGroup* buttonGroup, QWidget *parent) :
    QFrame(parent),
    paramLayout(new QVBoxLayout(this)),
    paramSpacer(new QSpacerItem(1, 10, QSizePolicy::Expanding, QSizePolicy::Expanding))
{
    for (unsigned int i = 0; i < params.size(); i++)
    {
        if (params[i]->type() == Param::MODE)
        {
            gparams.push_back(new GUICheckBox(static_cast<ModeParam*>(params[i])));
        }
        else if (params[i]->type() == Param::RANGE)
        {
            gparams.push_back(new GUISlider(static_cast<RangeParam<int>*>(params[i])));
        }
        else if (params[i]->type() == Param::RANGE_DBL)
        {
            gparams.push_back(new GUIDSpinBox(static_cast<RangeParam<double>*>(params[i])));
        }
        else if (params[i]->type() == Param::IMG_MODE)
        {
            gparams.push_back(new GUIRadioButton(static_cast<ImageModeParam*>(params[i])));
            buttonGroup->addButton(qobject_cast<QAbstractButton*>(gparams[i]));
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
    paramLayout->setMargin(8);
    paramLayout->setSpacing(8);
    for (unsigned int i = 0; i < gparams.size(); i++)
    {
        paramLayout->addWidget(gparams[i]);
    }
    paramLayout->addSpacerItem(paramSpacer);
}
