#include "guitreewidgetitem.h"
#include <QTreeWidgetItem>
#include "parameter.h"

GUITreeWidgetItem::GUITreeWidgetItem(QTreeWidgetItem * parent, std::vector<Param*> pm) :
        QTreeWidgetItem(parent),
        params(pm)
{
}

GUITreeWidgetItem::GUITreeWidgetItem(QTreeWidgetItem * parent) :
        QTreeWidgetItem(parent)
{
}

GUITreeWidgetItem::GUITreeWidgetItem(QTreeWidget* parent, std::vector<Param*> pm) :
        QTreeWidgetItem(parent),
        params(pm)
{
}

GUITreeWidgetItem::GUITreeWidgetItem(QTreeWidget* parent) :
        QTreeWidgetItem(parent)
{
}


std::vector<Param*> GUITreeWidgetItem::getParams() const
{
    return params;
}

bool GUITreeWidgetItem::hasParams() const
{
    return params.size() > 0;
}
