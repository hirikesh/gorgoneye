#include <QtGui/QApplication>
#include "geyetracker.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /* GEyeTracker - latest UI + tracking */
    GEyeTracker w;

    // QT UI initialisation
    w.show();
    return a.exec();
}
