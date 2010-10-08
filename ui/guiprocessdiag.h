#ifndef GUIPROCESSDIAG_H
#define GUIPROCESSDIAG_H

#include <vector>
#include <QFrame>

class BaseFilter;
class GrayscaleFilter;
class HSVFilter;
class QLabel;
class QHBoxLayout;
class QVBoxLayout;
class QPushButton;
class QListWidget;
class QGroupBox;
class QListWidgetItem;
class GUIParamDiag;

class GUIProcessDiag : public QFrame
{
    Q_OBJECT
public:
    GUIProcessDiag(std::vector<BaseFilter*>*, QWidget *parent = 0);

signals:

public slots:
    void addFilterItem();
    void removeFilterItem();
    void moveUpFilterItem();
    void moveDownFilterItem();
    void filterItemToggled(QListWidgetItem*);
    void changeParamBox(QListWidgetItem*, QListWidgetItem*);
private:
    void init();
    void swapFilterItems(int currIndex, int newIndex);

    QHBoxLayout* mainLayout;
    QVBoxLayout* auxLayout;
    QHBoxLayout* buttonLayout;

    QLabel* listTitle;
    QListWidget* processList;

    QPushButton* pbAdd;
    QPushButton* pbRemove;
    QPushButton* pbMoveUp;
    QPushButton* pbMoveDown;

    QGroupBox* paramBox;
    QVBoxLayout* paramLayout;
    GUIParamDiag* paramDialog;

    std::vector<BaseFilter*>* filters;
};

#endif // GUIPROCESSDIAG_H
