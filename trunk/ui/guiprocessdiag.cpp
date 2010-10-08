#include <vector>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QDebug>
#include "guiprocessdiag.h"
#include "guiparamdiag.h"
#include "filters/basefilter.h"

GUIProcessDiag::GUIProcessDiag(std::vector<BaseFilter*>* ft, QWidget *parent) :
    QFrame(parent),
    mainLayout(new QHBoxLayout(this)),
    auxLayout(new QVBoxLayout()),
    buttonLayout(new QHBoxLayout()),
    listTitle(new QLabel("Filters:")),
    processList(new QListWidget(this)),
    pbAdd(new QPushButton("+", this)),
    pbRemove(new QPushButton("-", this)),
    pbMoveUp(new QPushButton("Up", this)),
    pbMoveDown(new QPushButton("Down", this)),
    paramBox(new QGroupBox("Selected Item Parameters", this)),
    paramLayout(new QVBoxLayout()),
    filters(ft)
{
    init();
}

void GUIProcessDiag::init()
{
    for(unsigned int i = 0; i < filters->size(); i++)
    {
        BaseFilter* currFilter = filters->at(i);
        QListWidgetItem* currItem = new QListWidgetItem(currFilter->name().c_str());
        processList->addItem(currItem);
        currItem->setCheckState(Qt::Unchecked);
    }

    paramBox->setLayout(paramLayout);

    auxLayout->addWidget(listTitle);
    auxLayout->addWidget(processList);
    auxLayout->addLayout(buttonLayout);

    buttonLayout->addWidget(pbAdd);
    buttonLayout->addWidget(pbRemove);
    buttonLayout->addWidget(pbMoveUp);
    buttonLayout->addWidget(pbMoveDown);

    paramBox->setMinimumWidth(200);
    paramBox->setMinimumHeight(200);

    mainLayout->addLayout(auxLayout);
    mainLayout->addWidget(paramBox);


    QObject::connect(pbAdd, SIGNAL(clicked()), this, SLOT(addFilterItem()));
    QObject::connect(pbRemove, SIGNAL(clicked()), this, SLOT(removeFilterItem()));
    QObject::connect(pbMoveUp, SIGNAL(clicked()), this, SLOT(moveUpFilterItem()));
    QObject::connect(pbMoveDown, SIGNAL(clicked()), this, SLOT(moveDownFilterItem()));

    QObject::connect(processList, SIGNAL(itemChanged(QListWidgetItem*)),
                     this, SLOT(filterItemToggled(QListWidgetItem*)));
    QObject::connect(processList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
                     this, SLOT(changeParamBox(QListWidgetItem*,QListWidgetItem*)));
}

void GUIProcessDiag::addFilterItem()
{
    qDebug() << "You wanted to add a filter, but there are no filters to add yet.";
    // TODO: PopUp Menu
    // TODO: Create Item + possibly Create Params
    // NOTE: do not select new item
}

void GUIProcessDiag::removeFilterItem()
{
    if (filters->size() > 0)
    {
        int currIndex = processList->currentRow();
        if (currIndex < 0)
            return;
        delete processList->takeItem(currIndex);

        BaseFilter* currFilter = filters->at(currIndex);
        currFilter->disable();
        qDebug() << "You removed: " << currFilter->name().c_str();
        filters->erase(filters->begin() + currIndex);
        qDebug() << "No. of remaining filters: " << filters->size();
        delete currFilter;
        currFilter = NULL;
    } else
    {
        qDebug() << "Nothing happened";
    }
}

void GUIProcessDiag::moveUpFilterItem()
{
    int currIndex = processList->currentRow();
    if(currIndex < 0)
        return;
    int newIndex;
    if(currIndex == 0)
        newIndex = 0;
    else
        newIndex = currIndex - 1;
    swapFilterItems(currIndex, newIndex);
}

void GUIProcessDiag::moveDownFilterItem()
{
    int currIndex = processList->currentRow();
    if (currIndex < 0)
            return;
    int newIndex;
    int endIndex = processList->count()-1;
    if (currIndex == endIndex)
    {   newIndex = endIndex;  }
    else
    {   newIndex = currIndex + 1;  }
    swapFilterItems(currIndex, newIndex);
}

void GUIProcessDiag::swapFilterItems(int currIndex, int newIndex)
{
    QListWidgetItem* currItem = processList->takeItem(currIndex);
    processList->insertItem(newIndex, currItem);
    processList->setCurrentRow(newIndex);

    BaseFilter* tmp = filters->at(currIndex);
    filters->erase(filters->begin() + currIndex);
    filters->insert(filters->begin() + newIndex, tmp);
    changeParamBox(currItem, new QListWidgetItem());
}

void GUIProcessDiag::filterItemToggled(QListWidgetItem* item)
{
    if(item->checkState() == Qt::Checked)
    {
        int currIndex = processList->currentRow();
        BaseFilter* currFilter = filters->at(currIndex);
        currFilter->enable();
    }
    else
    {
        int currIndex = processList->currentRow();
        BaseFilter* currFilter = filters->at(currIndex);
        currFilter->disable();
    }
}

void GUIProcessDiag::changeParamBox(QListWidgetItem* currItem, QListWidgetItem* prevItem)
{
    if (currItem!= NULL)
    {
        int currIndex = processList->currentRow();

        BaseFilter* currFilter = filters->at(currIndex);
        if (paramDialog != NULL)
        {
            paramLayout->removeWidget(paramDialog);
            delete paramDialog;
            paramDialog = NULL;
        }
//        qDebug() << "Current Index:" << currIndex << "Current Filter" << currFilter->name().c_str();
        paramDialog = new GUIParamDiag(currFilter->name(), currFilter->params());
        paramLayout->addWidget(paramDialog);
    }
}
