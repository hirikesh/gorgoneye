#ifndef GUITREEWIDGETITEM_H
#define GUITREEWIDGETITEM_H

#include <QTreeWidgetItem>
#include <vector>

class Param;
class QTreeWidget;

class GUITreeWidgetItem : public QTreeWidgetItem
{
public:
    explicit GUITreeWidgetItem(QTreeWidgetItem*);
    explicit GUITreeWidgetItem(QTreeWidget*);
    GUITreeWidgetItem(QTreeWidgetItem*, std::vector<Param*>);
    GUITreeWidgetItem(QTreeWidget*, std::vector<Param*>);
    bool hasParams() const;
    std::vector<Param*> getParams() const;
private:
    std::vector<Param*> params;
};

#endif // GUITREEWIDGETITEM_H
