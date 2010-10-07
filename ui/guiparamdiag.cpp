#include <QVBoxLayout>
#include <QGroupBox>
#include "guiparamdiag.h"
#include "guiparam.h"
#include "parameter.h"



GUIParamDiag::GUIParamDiag(const std::string title, std::vector<Param*> params, QWidget *parent) :
    QFrame(parent),
    layout(new QVBoxLayout(this))
{
    std::vector<QWidget*> gparams(params.size());

    for (unsigned int i = 0; i < params.size(); i++)
    {
        if (params[i]->getType() == Param::MODE)
        {
            gparams[i] = new GUICheckBox(static_cast<ModeParam*>(params[i]));
            layout->addWidget(gparams[i]);

        }
        else if (params[i]->getType() == Param::RANGE)
        {
            gparams[i] = new GUISlider(static_cast<RangeParam<int>*>(params[i]));
            layout->addWidget(gparams[i]);
        }
        else if (params[i]->getType() == Param::RANGE_DBL)
        {
            gparams[i] = new GUIDSpinBox(static_cast<RangeParam<double>*>(params[i]));
            layout->addWidget(gparams[i]);
        }
        else if (params[i]->getType() == Param::IMG_MODE)
        {
            // DO NOTHING FOR NOW
        }
    }
}

GUIParamDiag::~GUIParamDiag()
{
    // TODO: Delete all gparams
    // Delete Layout

}
