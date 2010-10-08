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

    QHBoxLayout* mainLayout;
    QVBoxLayout* auxLayout;
    QLabel* listTitle;
    QTreeWidget* trackerTree;

    QGroupBox* paramBox;
    QVBoxLayout* paramLayout;
    GUIParamDiag* paramDialog;

    QScrollArea* scrollArea;
    QWidget *scrollContents;

    std::vector<BaseTracker*>* trackers;
};

#endif // GUITRACKERDIAG_H
