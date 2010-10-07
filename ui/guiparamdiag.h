#ifndef GUIPARAMDIAG_H
#define GUIPARAMDIAG_H

#include <QFrame>
#include <string>
#include <vector>


class QVBoxLayout;
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
    QVBoxLayout* layout;
};

#endif // GUIPARAMDIAG_H
