#include <string>
#include <vector>

#include <QGridLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QButtonGroup>
#include <QDebug>
#include <QScrollArea>

#include "guiprocessdiag.h"
#include "guiparamdiag.h"
#include "filters/basefilter.h"
#include "model.h"

GUIProcessDiag::GUIProcessDiag(const std::string& title, Model* m, QWidget *parent) :
    QFrame(parent),
    mainLayout(new QGridLayout(this)),
    listTitle(new QLabel(title.c_str())),
    paramTitle(new QLabel("Filter Parameters:")),
    processList(new QListWidget()),
    buttonLayout(new QGridLayout()),
    pbAdd(new QPushButton("+")),
    pbRemove(new QPushButton("-")),
    pbMoveUp(new QPushButton("Up")),
    pbMoveDown(new QPushButton("Down")),
    scrollArea(new QScrollArea()),
    filters(m->getPtrFilters())
{
    init();
}

void GUIProcessDiag::init()
{
    // create items appearing in the list initially
    for(unsigned int i = 0; i < filters->size(); i++)
    {
        BaseFilter* currFilter = filters->at(i);
        QListWidgetItem* currItem = new QListWidgetItem(currFilter->name().c_str());
        processList->addItem(currItem);
        if (currFilter->isEnabled())
        {
            currItem->setCheckState(Qt::Checked);
        }
        else
        {
               currItem->setCheckState(Qt::Unchecked);
        }
    }
    // organise appearance and layout of widgets
    setLayout(mainLayout);
    mainLayout->addWidget(listTitle, 0, 0);
    mainLayout->addWidget(processList, 1, 0);
    mainLayout->addLayout(buttonLayout, 2, 0);
    mainLayout->addWidget(paramTitle, 3, 0);
    mainLayout->addWidget(scrollArea, 4, 0);

    buttonLayout->addWidget(pbMoveUp, 0, 0);
    buttonLayout->addWidget(pbMoveDown, 0, 1);
    buttonLayout->addWidget(pbAdd, 1, 0);
    buttonLayout->addWidget(pbRemove, 1, 1);

    scrollArea->setWidgetResizable(true);
    this->setMinimumWidth(240);

    // create event handlers
    QObject::connect(pbAdd, SIGNAL(clicked()), this, SLOT(addProcessItem()));
    QObject::connect(pbRemove, SIGNAL(clicked()), this, SLOT(removeProcessItem()));
    QObject::connect(pbMoveUp, SIGNAL(clicked()), this, SLOT(moveUpProcessItem()));
    QObject::connect(pbMoveDown, SIGNAL(clicked()), this, SLOT(moveDownProcessItem()));
    QObject::connect(processList, SIGNAL(itemChanged(QListWidgetItem*)),
                     this, SLOT(filterItemToggled(QListWidgetItem*)));
    QObject::connect(processList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
                     this, SLOT(changeParamBox(QListWidgetItem*,QListWidgetItem*)));
}

void GUIProcessDiag::addProcessItem()
{
    qDebug() << "You wanted to add a filter, but there is no menu for adding filters yet.";
    // TODO: PopUp Menu
    // TODO: Create Item + possibly Create Params
    // NOTE: do not select new item
}

void GUIProcessDiag::removeProcessItem()
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

void GUIProcessDiag::moveUpProcessItem()
{
    int currIndex = processList->currentRow();
    if(currIndex < 0)
        return;
    int newIndex;
    if(currIndex == 0)
        newIndex = 0;
    else
        newIndex = currIndex - 1;
    swapProcessItems(currIndex, newIndex);
}

void GUIProcessDiag::moveDownProcessItem()
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
    swapProcessItems(currIndex, newIndex);
}

void GUIProcessDiag::swapProcessItems(int currIndex, int newIndex)
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
        //delete paramDialog;
        paramDialog = new GUIParamDiag(currFilter->params(), new QButtonGroup());
        scrollArea->setWidget(paramDialog);
    }
}
