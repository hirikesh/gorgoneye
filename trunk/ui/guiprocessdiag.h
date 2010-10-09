#ifndef GUIPROCESSDIAG_H
#define GUIPROCESSDIAG_H

#include <vector>
#include <QFrame>

class BaseFilter;
class GrayscaleFilter;
class HSVFilter;

class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QListWidget;
class QListWidgetItem;
class QPushButton;
class QScrollArea;
class GUIParamDiag;

class GUIProcessDiag : public QFrame
{
    Q_OBJECT
public:
    GUIProcessDiag(const std::string&, std::vector<BaseFilter*>*, QWidget *parent = 0);

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
    QVBoxLayout* leftLayout; // qt crashes when this is removed.

    QLabel* listTitle;
    QListWidget* processList;

    QHBoxLayout* buttonLayout;
    QPushButton* pbAdd;
    QPushButton* pbRemove;
    QPushButton* pbMoveUp;
    QPushButton* pbMoveDown;

    QScrollArea* scrollArea;
    GUIParamDiag* paramDialog;

    std::vector<BaseFilter*>* filters;
};

#endif // GUIPROCESSDIAG_H
