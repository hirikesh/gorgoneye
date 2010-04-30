#include <QtGui/QApplication>
#include "geyeui.h"

#include "geye.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    geyeui w;
    w.show();
    return a.exec();
}
