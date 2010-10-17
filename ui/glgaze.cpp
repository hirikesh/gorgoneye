#include <QApplication>
#include <QDesktopWidget>
#include <QRect>

#include <QKeyEvent>
#include <QResizeEvent>

#include <QPushButton>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <qgl.h>

#include "glgaze.h"
#include "store.h"

GLGaze::GLGaze(Store* st) :
    store(st)
{
    // Make our GL Gaze widget self-contained.
    setWindowTitle("GorgonEye - Gaze Control");
    setWindowFlags(Qt::FramelessWindowHint);
    setFrameShape(QFrame::NoFrame);

    // Attempt to use full screen res for training/gaze estimation
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRes = desktopWidget->screenGeometry(); // other: availableGeometry
    setScene(new QGraphicsScene(0, 0, screenRes.width(), screenRes.height()));
    setGeometry(screenRes);

    // Initialise the opengl viewport with anti-aliasing capabilities
    setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    // Initialise all widgets
    trainingModeBtn = new QPushButton("Training Mode");
    trainingModeBtn->setCheckable(true);
    scene()->addWidget(trainingModeBtn);
    connect(trainingModeBtn, SIGNAL(toggled(bool)), this, SLOT(setTrainingMode(bool)));
}

void GLGaze::resizeEvent(QResizeEvent *event)
{
    // Resize scene and view
    scene()->setSceneRect(QRect(QPoint(0,0), event->size()));
    QGraphicsView::resizeEvent(event);

    // Reposition widgets
    scene()->items()[0]->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    scene()->items()[0]->setPos(scene()->width() - trainingModeBtn->width() - 8,
                                scene()->height() - trainingModeBtn->height() - 8);
}

void GLGaze::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Escape)
        close();
}

void GLGaze::closeEvent(QCloseEvent* event)
{
    emit closed();
    event->accept();
}

void GLGaze::setTrainingMode(bool en)
{
    store->calibMode = en;
}
