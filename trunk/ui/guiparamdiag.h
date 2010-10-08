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
    QVBoxLayout* auxLayout;
    QVBoxLayout* layout;
    QScrollArea* scrollArea;
    QFrame* frame;
    QGroupBox* groupBox;
    std::vector<QWidget*> gparams;
    QWidget *scrollContents;
};

#endif // GUIPARAMDIAG_H
