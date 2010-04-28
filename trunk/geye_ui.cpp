#include "geye_ui.h"
#include "ui_geye_ui.h"

geye_ui::geye_ui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::geye_ui)
{
    ui->setupUi(this);
}

geye_ui::~geye_ui()
{
    delete ui;
}

void geye_ui::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
