#ifndef GEYE_UI_H
#define GEYE_UI_H

#include <QMainWindow>

namespace Ui {
    class geye_ui;
}

class geye_ui : public QMainWindow {
    Q_OBJECT
public:
    geye_ui(QWidget *parent = 0);
    ~geye_ui();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::geye_ui *ui;
};

#endif // GEYE_UI_H
