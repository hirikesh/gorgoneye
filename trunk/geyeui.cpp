#include "geyeui.h"
#include "ui_geyeui.h"

geyeui::geyeui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::geyeui)
{
    ui->setupUi(this);
}

geyeui::~geyeui()
{
    delete ui;
}

void geyeui::changeEvent(QEvent *e)
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
