#ifndef GUIPROCESSDIAG_H
#define GUIPROCESSDIAG_H

#include <vector>
#include <QFrame>

class BaseFilter;

class QGridLayout;
class QLabel;
class QListWidget;
class QListWidgetItem;
class QPushButton;
class QScrollArea;
class GUIParamDiag;
class Model;

class GUIProcessDiag : public QFrame
{
    Q_OBJECT
public:
    GUIProcessDiag(const std::string&, Model* m, QWidget *parent = 0);

signals:

public slots:
    void addProcessItem();
    void removeProcessItem();
    void moveUpProcessItem();
    void moveDownProcessItem();
    void filterItemToggled(QListWidgetItem*);
    void changeParamBox(QListWidgetItem*, QListWidgetItem*);
private:
    void init();
    void swapProcessItems(int currIndex, int newIndex);

    QGridLayout* mainLayout;

    QLabel* listTitle;
    QLabel* paramTitle;
    QListWidget* processList;

    QGridLayout* buttonLayout;
    QPushButton* pbAdd;
    QPushButton* pbRemove;
    QPushButton* pbMoveUp;
    QPushButton* pbMoveDown;

    QScrollArea* scrollArea;
    GUIParamDiag* paramDialog;

    std::vector<BaseFilter*>* filters;
};

#endif // GUIPROCESSDIAG_H
