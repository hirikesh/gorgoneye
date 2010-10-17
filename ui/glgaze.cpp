#include <QApplication>
#include <QDesktopWidget>
#include <QRect>
#include <QResizeEvent>
#include <QKeyEvent>

#include <qgl.h>
#include "glgaze.h"
#include "glgazescene.h"
#include "store.h"

GLGaze::GLGaze(Store* st) :
    store(st)
{
    // Make our GL Gaze widget self-contained.
    setWindowTitle("GorgonEye - Gaze Control");
    setWindowFlags(Qt::FramelessWindowHint);
    setFrameShape(QFrame::NoFrame);

    // Attempt to use full screen res for training/gaze estimation
    QRect screenRes = QApplication::desktop()->screenGeometry(); // other: availableGeometry

    // Create new scene with OpenGL capabilities
    gazeScene = new GLGazeScene(st, screenRes.width(), screenRes.height());
    setScene(gazeScene);
    setGeometry(screenRes);

    setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

void GLGaze::resizeEvent(QResizeEvent *event)
{
    // Propagate resize event to scene and view
    scene()->setSceneRect(QRect(QPoint(0,0), event->size()));
    QGraphicsView::resizeEvent(event);

    // Prepare for updating store and scene
    int dpiX = DPI_SCALE * logicalDpiX();
    int dpiY = DPI_SCALE * logicalDpiY();
    int gazeW = floor(width()/dpiX);
    int gazeH = floor(height()/dpiY);

    // Update store with new calibration parameters
    store->gazeWidth = 1 + gazeW;
    store->gazeHeight = 1 + gazeH;

    // Update scene with new calibration information
    gazeScene->setCalibInfo((width() - dpiX*gazeW) / 2,
                            (height() - dpiY*gazeH) / 2,
                            dpiX, dpiY);
}


void GLGaze::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Escape)
        close();
    if(event->key() == Qt::Key_Space)
    {
        QRect screenRes = QApplication::desktop()->screenGeometry(); // other: availableGeometry
        setGeometry(screenRes);
    }
}

void GLGaze::closeEvent(QCloseEvent* event)
{
    emit closed(); // for 'control' to pick up
    event->accept();
}
