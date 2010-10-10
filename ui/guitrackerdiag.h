#ifndef GUITRACKERDIAG_H
#define GUITRACKERDIAG_H

#include <vector>
#include <QFrame>

class QGridLayout;
class QLabel;
class QScrollArea;
class QTreeWidget;
class QTreeWidgetItem;
class QButtonGroup;
class BaseTracker;
class GUIParamDiag;
class Model;
class GUITreeWidgetItem;

class GUITrackerDiag : public QFrame
{
    Q_OBJECT
public:
    GUITrackerDiag(const std::string&, Model* m, QWidget *parent = 0);

signals:

public slots:
    void trackerItemToggled(QTreeWidgetItem*, int);
    void changeParamBox(QTreeWidgetItem*, QTreeWidgetItem*);
private:
    void init();
    void initTreeList();

    QGridLayout* mainLayout;

    QLabel* listTitle;
    QLabel* paramTitle;
    QTreeWidget* trackerTree;
    QScrollArea* scrollArea;
    QButtonGroup* buttonGroup;
    GUIParamDiag* paramDialog;
    GUITreeWidgetItem* initItemAdded;
    std::vector<BaseTracker*>* trackers;
};

#endif // GUITRACKERDIAG_H
