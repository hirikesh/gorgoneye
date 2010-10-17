#include <QTimer>
#include <QPushButton>
#include <QGraphicsItem>

#include <qgl.h>

#include "glgazescene.h"
#include "store.h"

GLGazeScene::GLGazeScene(Store* st, int w, int h, QObject* parent) :
    QGraphicsScene(0, 0, w, h, parent),
    calibModeBtn(new QPushButton("Calibration Mode")),
    calibModeTimer(new QTimer()),
    store(st)
{
    // Initialise UI widgets
    calibModeBtn->setCheckable(true);
    addWidget(calibModeBtn);
    items()[0]->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

    // Make sure widgets resize/reposition on scene resize
    connect(this, SIGNAL(sceneRectChanged(QRectF)), this, SLOT(updateWidgetPos(QRectF)));

    // Make calib mode button work
    connect(calibModeBtn, SIGNAL(toggled(bool)), this, SLOT(setCalibMode(bool)));
    connect(calibModeTimer, SIGNAL(timeout()), calibModeBtn, SLOT(toggle()));
}

void GLGazeScene::setCalibInfo(int x, int y, int dx, int dy)
{
    topLeftX = x;
    topLeftY = y;
    botRightX = width() - x;
    botRightY = height() - y;
    deltaX = dx;
    deltaY = dy;
}

void GLGazeScene::drawBackground(QPainter* painter, const QRectF& rect)
{
    if (painter->paintEngine()->type() != QPaintEngine::OpenGL)
    {
        qWarning("OpenGLScene: drawBackground needs a "
                 "QGLWidget to be set as viewport on the "
                 "graphics view");
        return;
    }

    // OpenGL setup stuff
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    gluOrtho2D(0, width(), 0, height());
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // Black/gray background
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw calibration visualisation
    glLineWidth(1.0);
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINES);
        for(int nextx = topLeftX; nextx <= botRightX; nextx = nextx + deltaX)
        {
            glVertex2i(nextx, 0);
            glVertex2i(nextx, height());
        }
        for(int nexty = topLeftY; nexty <= botRightY; nexty = nexty + deltaY)
        {
            glVertex2i(0, nexty);
            glVertex2i(width(), nexty);
        }
    glEnd();

    // End drawing background
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    // VRITUAL DEPTH PERCEPTION TEST CODE
//    int x = width() * (store->faceRoi.x + store->faceRoi.width/2) / FRAME_WIDTH;
//    int y = height() * (store->faceRoi.y + store->faceRoi.height/2) / FRAME_HEIGHT;
//    items()[0]->setPos(x - calibModeBtn->width()/2,
//                       height() - y - calibModeBtn->height()/2);
}

void GLGazeScene::drawForeground(QPainter* painter, const QRectF& rect)
{
    if(store->calibMode)
    {
        // OpenGL setup stuff
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();

        gluOrtho2D(0, width(), 0, height());
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

        // Draw calibration visualisation
        glPointSize(24.0);
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POINTS);
            glVertex2i(topLeftX + deltaX*store->calibPoint.x - 1,
                       topLeftY + deltaY*store->calibPoint.y - 1);
        glEnd();

        // End drawing foreground
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
    }
}


// SLOTS

void GLGazeScene::updateWidgetPos(const QRectF& rect)
{
    // Reposition calib mode button
    items()[0]->setPos(rect.width() - calibModeBtn->width() - 8,
                       rect.height() - calibModeBtn->height() - 8);
}

void GLGazeScene::setCalibMode(bool en)
{
    // Notify gaze-tracker calibration has momentarily stopped
    store->calibMode = en;

    // If calibration button checked, start the timer.
    // Otherwise stop the timer from doing anything else.
    if(en)
        calibModeTimer->start(CALIB_TIME_PER_POINT * 1000);
    else
        calibModeTimer->stop();
}
