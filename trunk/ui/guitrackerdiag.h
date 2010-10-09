#ifndef GUITRACKERDIAG_H
#define GUITRACKERDIAG_H

#include <QTreeView>
#include <vector>
class QHBoxLayout;
class QLabel;
class QTreeWidget;
class GUIParamDiag;
class QVBoxLayout;
class QGroupBox;
class BaseTracker;
class QTreeWidgetItem;
class QScrollArea;

class GUITrackerDiag : public QFrame
{
    Q_OBJECT
public:
    GUITrackerDiag(const std::string&, std::vector<BaseTracker*>*, QWidget *parent = 0);

signals:

public slots:
    void trackerItemToggled(QTreeWidgetItem*, int);
    void changeParamBox(QTreeWidgetItem*, QTreeWidgetItem*);
private:
    void init();
    void initTreeList();

    QHBoxLayout* mainLayout;
    QVBoxLayout* leftLayout;
    QVBoxLayout* rightLayout;
    QLabel* listTitle;
    QTreeWidget* trackerTree;

    QVBoxLayout* paramLayout;
    QGroupBox* paramBox;

    GUIParamDiag* paramDialog;

    QScrollArea* scrollArea;
    QWidget *scrollContents;

    std::vector<BaseTracker*>* trackers;
};

#endif // GUITRACKERDIAG_H
