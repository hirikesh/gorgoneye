#include <QApplication>
#include <QDesktopWidget>
#include <QRect>

#include <QResizeEvent>
#include <QKeyEvent>

#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <qgl.h>

#include "glgaze.h"
#include "glgazescene.h"
#include "store.h"

#include <QDebug>

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

    // Initialise UI widgets
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

    // Update calibration information
    updateCalibInfo();
}

void GLGaze::updateCalibInfo()
{
    // Prepare variables for updating store and scene
    int scaleFactor = 2; // start off easy
    int dpiX = scaleFactor*logicalDpiX();
    int dpiY = scaleFactor*logicalDpiY();
    int gazeW = floor(width()/dpiX);
    int gazeH = floor(height()/dpiY);

    // Update store with new calibration parameters
    store->gazeWidth = 1 + gazeW;
    store->gazeHeight = 1 + gazeH;

    // Update scene with new calibration information
    gazeScene->setCalibInfo((width() - dpiX*gazeW) / 2,
                            (height() - dpiY*gazeH) / 2,
                            dpiX, dpiY);

    qDebug() << store->gazeWidth << store->gazeHeight;
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
    emit closed();
    event->accept();
}

void GLGaze::setTrainingMode(bool en)
{
    store->calibMode = en;
}
