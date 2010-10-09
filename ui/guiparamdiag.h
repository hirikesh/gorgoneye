#ifndef GUIPARAMDIAG_H
#define GUIPARAMDIAG_H

#include <QFrame>
#include <vector>

class QVBoxLayout;
class QSpacerItem;
class Param;

class GUIParamDiag: public QFrame
{
    Q_OBJECT
public:
    GUIParamDiag(std::vector<Param*>, QWidget *parent = 0);
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
