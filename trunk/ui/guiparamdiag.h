#ifndef GUIPARAMDIAG_H
#define GUIPARAMDIAG_H

#include <QFrame>
#include <string>
#include <vector>

class QScrollArea;
class QVBoxLayout;
class QGroupBox;
class QFrame;
class Param;

class GUIParamDiag: public QFrame
{
    Q_OBJECT
public:
    GUIParamDiag(const std::string, std::vector<Param*>, QWidget *parent = 0);
    ~GUIParamDiag();

signals:

public slots:

private:
    void initUI();

    QVBoxLayout* auxLayout;
    QVBoxLayout* layout;

    //QFrame* frame;
    QGroupBox* groupBox;
    std::vector<QWidget*> gparams;

    QScrollArea* scrollArea;
    QWidget *scrollableContents;
};

#endif // GUIPARAMDIAG_H
