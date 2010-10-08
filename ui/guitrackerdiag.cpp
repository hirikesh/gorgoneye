#include <vector>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTreeWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QDebug>

#include "parameter.h"
#include "trackers/basetracker.h"
#include "ui/guiparamdiag.h"
#include "guitrackerdiag.h"

GUITrackerDiag::GUITrackerDiag(std::vector<BaseTracker*>* tr, QWidget *parent) :
    QFrame(parent),
    mainLayout(new QHBoxLayout(this)),
    auxLayout(new QVBoxLayout()),
    listTitle(new QLabel("Filters:")),
    trackerTree(new QTreeWidget(this)),
    paramBox(new QGroupBox("Selected Item Parameters", this)),
    paramLayout(new QVBoxLayout()),
    trackers(tr)
{
    init();
}

void GUITrackerDiag::init()
{
/*    for(unsigned int i = 0; i < trackers->size(); i++)
    {
        BaseTracker* currTracker = trackers->at(i);
        QTreeWidgetItem* currItem = new QTreeWidgetItem(i, currTracker->name().c_str());
        trackerTree->addTopLevelItem(currItem);
        currItem->setCheckState(i, Qt::Unchecked);

    }*/

    paramBox->setLayout(paramLayout);

    auxLayout->addWidget(listTitle);
    auxLayout->addWidget(trackerTree);

    paramBox->setMinimumWidth(200);
    paramBox->setMinimumHeight(200);

    mainLayout->addLayout(auxLayout);
    mainLayout->addWidget(paramBox);


    QObject::connect(trackerTree, SIGNAL(itemChanged(QTreeWidgetItem*)),
                     this, SLOT(filterItemToggled(QTreeWidgetItem*)));
    QObject::connect(trackerTree, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
                     this, SLOT(changeParamBox(QTreeWidgetItem*,QTreeWidgetItem*)));
}

void GUITrackerDiag::trackerItemToggled(QTreeWidgetItem * item)
{
    // item check state broken
    /*if(item->checkState() == Qt::Checked)
    {
        int currIndex = trackerTree->currentRow();
        BaseTracker* currTracker = trackers->at(currIndex);
        currTracker->enable();
    }
    else
    {
        int currIndex = trackerTree->currentRow();
        BaseTracker* currTracker = trackers->at(currIndex);
        currTracker->disable();
    }*/
}

void GUITrackerDiag::changeParamBox(QTreeWidgetItem *currItem, QTreeWidgetItem *prevItem)
{
    /*if (currItem!= NULL)
    {

        int currIndex = trackerTree->currentRow();

        BaseTracker* currTracker = trackers->at(currIndex);
        if (paramDialog != NULL)
        {
            paramLayout->removeWidget(paramDialog);
            delete paramDialog;
            paramDialog = NULL;
        }
        qDebug() << "Current Index:" << currIndex << "Current Tracker" << currTracker->name().c_str();
        paramDialog = new GUIParamDiag(currTracker->name(), currTracker->params());
        paramLayout->addWidget(paramDialog);
    }*/

}
