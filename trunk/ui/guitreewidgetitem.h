#ifndef GUITREEWIDGETITEM_H
#define GUITREEWIDGETITEM_H

#include <QTreeWidgetItem>
#include <vector>
#include "guiparamdiag.h"

class Param;
class QTreeWidget;
//class GUIParamDiag;

class GUITreeWidgetItem : public QTreeWidgetItem
{
public:
//    explicit GUITreeWidgetItem(QTreeWidgetItem*);
//    explicit GUITreeWidgetItem(QTreeWidget*);
    GUITreeWidgetItem(QTreeWidgetItem*, std::vector<Param*>, QButtonGroup*);
    GUITreeWidgetItem(QTreeWidget*, std::vector<Param*>, QButtonGroup*);
    bool hasParams() const;
    std::vector<Param*> getParams() const;
    GUIParamDiag* getParamDialog();
private:
    std::vector<Param*> params;
    GUIParamDiag* paramDialog;
};

#endif // GUITREEWIDGETITEM_H
