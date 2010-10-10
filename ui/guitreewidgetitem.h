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
    GUITreeWidgetItem(QTreeWidgetItem*, std::vector<Param*>, QButtonGroup*);
    GUITreeWidgetItem(QTreeWidget*, std::vector<Param*>, QButtonGroup*);
    bool hasParams() const;
    GUIParamDiag* getParamDialog();
private:
    GUIParamDiag* paramDialog;
};

#endif // GUITREEWIDGETITEM_H
