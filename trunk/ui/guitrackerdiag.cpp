#include <vector>
#include <string>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QTreeWidget>
#include <QPushButton>
#include <QHeaderView>
#include <QGridLayout>
#include "trackers/basetracker.h"
#include "detectors/basedetector.h"
#include "ui/guiparamdiag.h"
#include "ui/guitreewidgetitem.h"
#include "parameter.h"
#include "guitrackerdiag.h"
#include "model.h"

GUITrackerDiag::GUITrackerDiag(const std::string& title, Model* m, QWidget *parent) :
    QFrame(parent),
    mainLayout(new QGridLayout(this)),
    listTitle(new QLabel(title.c_str())),
    paramTitle(new QLabel("Tracking Parameters:")),
    trackerTree(new QTreeWidget()),
    scrollArea(new QScrollArea()),
    trackers(m->getPtrTrackers())
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
        GUITreeWidgetItem* currTrackerItem = new GUITreeWidgetItem(trackerTree, currTracker->getImageModes());
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
                GUITreeWidgetItem* currDetectorItem = new GUITreeWidgetItem(currTrackerItem, detectors[j]->params());
                std::string trackerItemEntry = detectors[j]->name() + " Algorithm";
                currDetectorItem->setText(firstColumn, trackerItemEntry.c_str());
            }
        }
    }
}

void GUITrackerDiag::init()
{
    trackerTree->header()->hide();
    trackerTree->expandAll();

    mainLayout->addWidget(listTitle, 0, 0);
    mainLayout->addWidget(paramTitle, 0, 1);
    mainLayout->addWidget(trackerTree, 1, 0);
    mainLayout->addWidget(scrollArea, 1, 1);

    scrollArea->setWidgetResizable(true);
    scrollArea->setMinimumHeight(300);

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
        GUITreeWidgetItem* currentItem = static_cast<GUITreeWidgetItem*>(currItem);

//        if (paramDialog != NULL)
//        {
//            delete paramDialog;
//            paramDialog = NULL;
//        }
        paramDialog = new GUIParamDiag(currentItem->getParams());
        scrollArea->setWidget(paramDialog);
    }

}
