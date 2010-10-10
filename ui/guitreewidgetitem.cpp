#include <QTreeWidgetItem>
#include "ui/guiparamdiag.h"
#include "guitreewidgetitem.h"

GUITreeWidgetItem::GUITreeWidgetItem(QTreeWidgetItem * parent, std::vector<Param*> pm, QButtonGroup* buttonGroup) :
        QTreeWidgetItem(parent),
        paramDialog(new GUIParamDiag(pm, buttonGroup))
{

}

GUITreeWidgetItem::GUITreeWidgetItem(QTreeWidget* parent, std::vector<Param*> pm, QButtonGroup* buttonGroup) :
        QTreeWidgetItem(parent),
        paramDialog(new GUIParamDiag(pm, buttonGroup))
{
}

GUIParamDiag* GUITreeWidgetItem::getParamDialog()
{
    return paramDialog;
}
