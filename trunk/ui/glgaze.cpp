#include <QApplication>
#include <QDesktopWidget>
#include <QResizeEvent>
#include <QKeyEvent>

#include <cmath>
#include <qgl.h>
#include "glgaze.h"
#include "glgazescene.h"

GLGaze::GLGaze(Store* st)
{
    // Make our GL Gaze widget self-contained.
    setWindowTitle("GorgonEye - Gaze Control");
    setWindowFlags(Qt::FramelessWindowHint);
    setFrameShape(QFrame::NoFrame);

    // Create new viewport with OpenGL capabilities
    setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    // Create scene and attempt to use full screen res for training/gaze estimation
    setScene(new GLGazeScene(st));
    setGeometry(QApplication::desktop()->screenGeometry()); // other: availableGeometry
}

void GLGaze::resizeEvent(QResizeEvent *event)
{
    // Propagate resize event to scene and view
    scene()->setSceneRect(QRect(QPoint(0,0), event->size()));
    QGraphicsView::resizeEvent(event);

    // Update scene with new calibration information
    int dpiX = DPI_SCALE * logicalDpiX();
    int dpiY = DPI_SCALE * logicalDpiY();
    ((GLGazeScene*)scene())->setCalibInfo(dpiX, dpiY,
                            floor(width()/dpiX),
                            floor(height()/dpiY));

    // Update scene widget positions
    ((GLGazeScene*)scene())->updateWidgetPos();
}


void GLGaze::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Escape)
        close();
    else if(event->key() == Qt::Key_Space)
        setGeometry(QApplication::desktop()->screenGeometry()); // other: availableGeometry
    QGraphicsView::keyPressEvent(event);
}

void GLGaze::closeEvent(QCloseEvent* event)
{
    emit closed(); // for 'control' to pick up
    event->accept();
}
