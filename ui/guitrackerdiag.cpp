#include <vector>
#include <string>

#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QDebug>
#include <QTreeWidget>
#include <QHeaderView>
#include <QGridLayout>
#include <QButtonGroup>
#include "trackers/basetracker.h"
#include "detectors/basedetector.h"
#include "ui/guiparamdiag.h"
#include "ui/guitreewidgetitem.h"
#include "ui/guiparam.h"
#include "parameter.h"
#include "model.h"
#include "guitrackerdiag.h"



GUITrackerDiag::GUITrackerDiag(const std::string& title, Model* m, QWidget *parent) :
    QFrame(parent),
    mainLayout(new QGridLayout(this)),
    paramLayout(new QVBoxLayout()),
    listTitle(new QLabel(title.c_str())),
    paramTitle(new QLabel("Tracking Parameters:")),
    trackerTree(new QTreeWidget()),
    scrollArea(new QScrollArea()),
    scrollContents(new QWidget()),
    buttonGroup(new QButtonGroup()),
    trackers(m->getPtrTrackers())
{
    initTreeList();
    init();
}

void GUITrackerDiag::initTreeList()
{
    //bool firstItemAdded = true;
    int firstColumn = 0;
    BaseTracker* currTracker;
    for(unsigned int i = 0; i < trackers->size(); i++)
    {
        currTracker = trackers->at(i);
        GUITreeWidgetItem* currTrackerItem = new GUITreeWidgetItem(trackerTree, currTracker->getImageModes(), buttonGroup);
//        if (firstItemAdded)
//        {
//            firstItemAdded = false;
//            initItemAdded = currTrackerItem;
//        }
//        std::string trackerEntry = "Enable " + currTracker->name() + " Tracking";
        std::string trackerEntry = currTracker->name();
        currTrackerItem->setText(firstColumn, trackerEntry.c_str());
        paramDialogs.push_back(currTrackerItem->getParamDialog());

        if (currTracker->isEnabled())
        {
            currTrackerItem->setCheckState(firstColumn, Qt::Checked);
        }
        else
        {
            currTrackerItem->setCheckState(firstColumn, Qt::Unchecked);
        }

        std::vector<BaseFilter*> filters = currTracker->getFilters();
        for(unsigned int j = 0; j < filters.size(); j++)
        {
            GUITreeWidgetItem* currDetectorItem = new GUITreeWidgetItem(currTrackerItem, filters[j]->params(), buttonGroup);
            std::string trackerItemEntry = filters[j]->name() + " Filter";
            currDetectorItem->setText(firstColumn, trackerItemEntry.c_str());
            paramDialogs.push_back(currDetectorItem->getParamDialog());
        }

        std::vector<BaseDetector*> detectors = currTracker->getDetectors();
        for(unsigned int j = 0; j < detectors.size(); j++)
        {
            GUITreeWidgetItem* currDetectorItem = new GUITreeWidgetItem(currTrackerItem, detectors[j]->params(), buttonGroup);
            std::string trackerItemEntry = detectors[j]->name() + " Algorithm";
            currDetectorItem->setText(firstColumn, trackerItemEntry.c_str());
            paramDialogs.push_back(currDetectorItem->getParamDialog());
        }
    }
    // When user chooses different tracker; destroy top-level item associated with tracker, it's parameter dialogs? and re-create it.
    //delete trackerTree->takeTopLevelItem(0);
}

void GUITrackerDiag::init()
{

    trackerTree->header()->hide();
//    trackerTree->expandAll();

    for (unsigned int i = 0; i < paramDialogs.size(); i++)
    {
        paramDialogs[i]->hide();
        paramLayout->addWidget(paramDialogs[i]);
    }

    paramLayout->setMargin(0);

    mainLayout->addWidget(listTitle, 0, 0);
    mainLayout->addWidget(trackerTree, 1, 0);
    mainLayout->addWidget(paramTitle, 2, 0);
    mainLayout->addWidget(scrollArea, 3, 0);

    scrollContents->setLayout(paramLayout);
    scrollArea->setWidget(scrollContents);
    scrollArea->setWidgetResizable(true);

    setMinimumWidth(240);

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
        if (prevItem != NULL)
        {
            GUITreeWidgetItem* previousItem = static_cast<GUITreeWidgetItem*>(prevItem);
            previousItem->getParamDialog()->hide();
        }
        GUITreeWidgetItem* currentItem = static_cast<GUITreeWidgetItem*>(currItem);
        currentItem->getParamDialog()->show();
    }
}
