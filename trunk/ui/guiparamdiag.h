#ifndef GUIPARAMDIAG_H
#define GUIPARAMDIAG_H

#include <QFrame>
#include <vector>

class QVBoxLayout;
class QSpacerItem;
class Param;
class QButtonGroup;

class GUIParamDiag: public QFrame
{
    Q_OBJECT
public:
    explicit GUIParamDiag(QButtonGroup*, QWidget *parent = 0);
    GUIParamDiag(std::vector<Param*>, QButtonGroup*, QWidget *parent = 0);
    ~GUIParamDiag();

signals:

public slots:

private:
    void init();

    QVBoxLayout* paramLayout;
    QSpacerItem* paramSpacer;
    std::vector<QWidget*> gparams;
};

#endif // GUIPARAMDIAG_H
