#include <QtGui/QApplication>

#include "geyeview.h"
#include "geyeui.h"

#include "geye.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // geyeview - gorgoneye tracking view
    geyeview w;
    // geyeui - gorgoneye user interface
    //geyeui w;
    w.show();
    return a.exec();
}
