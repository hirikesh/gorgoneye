#include <QVBoxLayout>
#include <QScrollArea>
#include <QGroupBox>
#include <QDebug>
#include <QFrame>
#include "guiparamdiag.h"
#include "guiparam.h"
#include "parameter.h"
#include <QSpacerItem>

GUIParamDiag::GUIParamDiag(const std::string title, std::vector<Param*> params, QWidget *parent) :
    QFrame(parent),
    auxLayout(new QVBoxLayout()),
    layout(new QVBoxLayout()),
    groupBox(new QGroupBox()),
    scrollArea(new QScrollArea()),
    scrollableContents(new QWidget())
{
    for (unsigned int i = 0; i < params.size(); i++)
    {
        if (params[i]->getType() == Param::MODE)
        { gparams.push_back(new GUICheckBox(static_cast<ModeParam*>(params[i]))); }
        else if (params[i]->getType() == Param::RANGE)
        { gparams.push_back(new GUISlider(static_cast<RangeParam<int>*>(params[i]))); }
        else if (params[i]->getType() == Param::RANGE_DBL)
        { gparams.push_back(new GUIDSpinBox(static_cast<RangeParam<double>*>(params[i]))); }
        else // if (params[i]->getType() == Param::IMG_MODE)
        {  /* Do nothing */ }
    }

    initUI();
}

GUIParamDiag::~GUIParamDiag()
{
    for (unsigned int i = 0; i < gparams.size(); i++)
    {
        delete gparams[i];
        gparams.erase(gparams.begin() + i);
    }
    delete layout;
    if (scrollArea != NULL)
        delete scrollArea;
}

void GUIParamDiag::initUI()
{

    // set size policy
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(scrollableContents->sizePolicy().hasHeightForWidth());
    scrollableContents->setSizePolicy(sizePolicy);

    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollableContents);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollableContents->setLayout(layout);
    this->setLayout(auxLayout);
    auxLayout->addWidget(scrollArea);



    for (unsigned int i = 0; i < gparams.size(); i++)
    {
        layout->addWidget(gparams[i]);
    }
    layout->addSpacerItem(new QSpacerItem(1, 50));

}
