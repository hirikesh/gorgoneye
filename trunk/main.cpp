#include <QtGui/QApplication>
#include "control.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /* Control - latest UI + tracking */
    Control w;
//    w.setWindowState(Qt::WindowMaximized);

    // QT UI initialisation
    w.show();
    return a.exec();
}
