#include "guitreewidgetitem.h"
#include "ui/guiparamdiag.h"
#include <QTreeWidgetItem>
#include "parameter.h"

GUITreeWidgetItem::GUITreeWidgetItem(QTreeWidgetItem * parent, std::vector<Param*> pm, QButtonGroup* buttonGroup) :
        QTreeWidgetItem(parent),
        params(pm),
        paramDialog(new GUIParamDiag(pm, buttonGroup))
{

}

//GUITreeWidgetItem::GUITreeWidgetItem(QTreeWidgetItem * parent) :
//        QTreeWidgetItem(parent)
//{
//}

GUITreeWidgetItem::GUITreeWidgetItem(QTreeWidget* parent, std::vector<Param*> pm, QButtonGroup* buttonGroup) :
        QTreeWidgetItem(parent),
        params(pm),
        paramDialog(new GUIParamDiag(pm, buttonGroup))
{
}

//GUITreeWidgetItem::GUITreeWidgetItem(QTreeWidget* parent) :
//        QTreeWidgetItem(parent)
//{
//}


std::vector<Param*> GUITreeWidgetItem::getParams() const
{
    return params;
}

bool GUITreeWidgetItem::hasParams() const
{
    return params.size() > 0;
}

GUIParamDiag* GUITreeWidgetItem::getParamDialog()
{
    return paramDialog;
}
