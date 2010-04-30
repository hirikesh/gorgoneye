#ifndef GEYEUI_H
#define GEYEUI_H

#include <QMainWindow>

namespace Ui {
    class geyeui;
}

class geyeui : public QMainWindow {
    Q_OBJECT
public:
    geyeui(QWidget *parent = 0);
    ~geyeui();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::geyeui *ui;
};

#endif // GEYEUI_H
