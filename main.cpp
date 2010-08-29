#include <QtGui/QApplication>

#include "geyedetector.h"
#include "geyetracker.h"
#include <QGLFormat>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!QGLFormat::hasOpenGL()) {
        qDebug() << "This system has no OpenGL support";
    return 1;
    }

    /* GEyeTracker - latest UI + tracking */
    GEyeTracker w;

    // QT UI initialisation
    w.show();
    return a.exec();
}
