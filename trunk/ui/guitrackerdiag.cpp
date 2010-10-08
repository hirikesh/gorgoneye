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
#include "parameter.h"
#include "trackers/basetracker.h"
#include "detectors/basedetector.h"
#include "ui/guiparamdiag.h"
#include "guitrackerdiag.h"
#include "ui/guitreewidgetitem.h"

GUITrackerDiag::GUITrackerDiag(const std::string& title, std::vector<BaseTracker*>* tr, QWidget *parent) :
    QFrame(parent),
    mainLayout(new QHBoxLayout(this)),
    auxLayout(new QVBoxLayout()),
    listTitle(new QLabel(title.c_str())),
    trackerTree(new QTreeWidget()),
    paramBox(new QGroupBox("Selected Item Parameters")),
    paramLayout(new QVBoxLayout()),
    //scrollArea(new QScrollArea()),
    //scrollContents(new QWidget()),
    trackers(tr)
{
    init();
}

void GUITrackerDiag::init()
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

    trackerTree->header()->hide();

    auxLayout->addWidget(listTitle);
    auxLayout->addWidget(trackerTree);

    paramBox->setMinimumWidth(200);
    paramBox->setMinimumHeight(400);

    //scrollContents->setGeometry(QRect(0, 0, 400, 500));
    //scrollArea->setWidget(scrollContents);

    mainLayout->addLayout(auxLayout);
    mainLayout->addWidget(paramBox);
    //mainLayout->addWidget(paramBox);
    //paramBox->setParent(scrollContents);
    //scrollContents->setLayout(paramLayout);
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
