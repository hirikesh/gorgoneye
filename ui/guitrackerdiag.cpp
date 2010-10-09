#include <vector>
#include <string>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QTreeWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QHeaderView>

#include "trackers/basetracker.h"
#include "detectors/basedetector.h"
#include "ui/guiparamdiag.h"
#include "ui/guitreewidgetitem.h"
#include "parameter.h"
#include "guitrackerdiag.h"


GUITrackerDiag::GUITrackerDiag(const std::string& title, std::vector<BaseTracker*>* tr, QWidget *parent) :
    QFrame(parent),
    mainLayout(new QHBoxLayout(this)),
    leftLayout(new QVBoxLayout()),
    rightLayout(new QVBoxLayout()),
    listTitle(new QLabel(title.c_str())),
    trackerTree(new QTreeWidget()),
    paramLayout(new QVBoxLayout()),
    paramBox(new QGroupBox("Selected Item Parameters")),
    //scrollArea(new QScrollArea()),
    //scrollContents(new QWidget()),
    trackers(tr)
{
    initTreeList();
    init();
}

void GUITrackerDiag::initTreeList()
{
    int firstColumn = 0;

    BaseTracker* currTracker;
    for(unsigned int i = 0; i < trackers->size(); i++)
    {
        currTracker = trackers->at(i);
        GUITreeWidgetItem* currTrackerItem = new GUITreeWidgetItem(trackerTree);
        std::string trackerEntry = "Enable " + currTracker->name() + " Tracking";
        currTrackerItem->setText(firstColumn, trackerEntry.c_str());
        if (currTracker->isEnabled())
        {
            currTrackerItem->setCheckState(firstColumn, Qt::Checked);
        }
        else
        {
            currTrackerItem->setCheckState(firstColumn, Qt::Unchecked);
        }
        std::vector<BaseDetector*> detectors = currTracker->getDetectors();
        for(unsigned int j = 0; j < detectors.size(); j++)
        {
            if (detectors[j]->hasParams())
            {
                GUITreeWidgetItem* currDetectorItem = new GUITreeWidgetItem(currTrackerItem, detectors[j]->getParams());
                std::string trackerItemEntry = detectors[j]->name() + " Algorithm";
                currDetectorItem->setText(firstColumn, trackerItemEntry.c_str());
            }
        }
    }
}

void GUITrackerDiag::init()
{
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    trackerTree->header()->hide();

    leftLayout->addWidget(listTitle);
    leftLayout->addWidget(trackerTree);

    rightLayout->addWidget(paramBox);

    paramBox->setMinimumWidth(200);
    paramBox->setMinimumHeight(400);
    paramBox->setLayout(paramLayout);

    QObject::connect(trackerTree, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
                     this, SLOT(trackerItemToggled(QTreeWidgetItem*, int)));
    QObject::connect(trackerTree, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
                     this, SLOT(changeParamBox(QTreeWidgetItem*,QTreeWidgetItem*)));
}

void GUITrackerDiag::trackerItemToggled(QTreeWidgetItem * item, int index)
{
    if(item->checkState(0) == Qt::Checked)

    {
        int currIndex = trackerTree->currentIndex().row();
        BaseTracker* currTracker = trackers->at(currIndex);
        currTracker->enable();
    }
    else
    {
        int currIndex = trackerTree->currentIndex().row();
        BaseTracker* currTracker = trackers->at(currIndex);
        currTracker->disable();
    }
}

void GUITrackerDiag::changeParamBox(QTreeWidgetItem *currItem, QTreeWidgetItem *prevItem)
{
    if (currItem!= NULL)
    {
        GUITreeWidgetItem* currentItem = (GUITreeWidgetItem *) currItem;

        if (paramDialog != NULL)
        {
            paramLayout->removeWidget(paramDialog);
            delete paramDialog;
            paramDialog = NULL;
        }
        paramDialog = new GUIParamDiag(currentItem->text(0).toStdString(), currentItem->getParams());
        paramLayout->addWidget(paramDialog);
    }

}
