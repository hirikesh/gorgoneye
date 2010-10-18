#include <QTimer>
#include <QKeyEvent>
#include <QPushButton>
#include <QGraphicsItem>

#include <qgl.h>
#include <QDebug>

#include "glgazescene.h"
#include "store.h"

GLGazeScene::GLGazeScene(Store* st, QObject* parent) :
    calibModeBtn(new QPushButton("Calibration Mode")),
    calibPointBtn(new QPushButton("Calibrate Point")),
    calibPointTimer(new QTimer()),
    store(st)
{
    // Initialise UI widgets
    calibModeBtn->setCheckable(true);
    calibPointBtn->setCheckable(true);
    calibPointBtn->setEnabled(false);
    addWidget(calibModeBtn);
    addWidget(calibPointBtn);
    foreach(QGraphicsItem* item, items())
        item->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

    // Make calib mode buttons work
    connect(calibModeBtn, SIGNAL(toggled(bool)), this, SLOT(setCalibMode(bool)));
    connect(calibPointBtn, SIGNAL(toggled(bool)), this, SLOT(setPointCalibMode(bool)));
    connect(calibPointTimer, SIGNAL(timeout()), calibPointBtn, SLOT(toggle()));
}

// Called by GLGaze when window is resized
void GLGazeScene::setCalibInfo(int dx, int dy, int cw, int ch)
{
    calibW = cw;
    calibH = ch;
    deltaX = dx;
    deltaY = dy;
    botLeftX = (width() - dx*cw) / 2;
    botLeftY = (height() - dy*ch) / 2;
    topRightX = width() - botLeftX;
    topRightY = height() - botLeftY;

    calibModeBtn->setChecked(false); // stop any calibration
    store->calibX = 0; // start calibration from
    store->calibY = ch; // top left
}

// Called by GLGaze when window is resized
void GLGazeScene::updateWidgetPos()
{
    // Reposition calibration widgets
    QPointF offset(8, 8);
    foreach(QGraphicsItem* item, items())
    {
        const QRectF rect = item->boundingRect();
        item->setPos(width() - rect.width() - offset.x(),
                     height() - rect.height() - offset.y());
        offset += QPointF(rect.width() + 8, 0);
    }
}


// Main drawing functions

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
        for(int nextx = botLeftX; nextx <= topRightX; nextx = nextx + deltaX)
        {
            glVertex2i(nextx, 0);
            glVertex2i(nextx, height());
        }
        for(int nexty = botLeftY; nexty <= topRightY; nexty = nexty + deltaY)
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
    if(calibModeBtn->isChecked())
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
        if(calibPointBtn->isChecked())
            glColor3f(1.0f, 0.0f, 0.0f);
        else
            glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POINTS);
            glVertex2i(botLeftX + deltaX*store->calibX,
                       botLeftY + deltaY*store->calibY);
        glEnd();

        // End drawing foreground
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
    }
}


// SLOTS


void GLGazeScene::setCalibMode(bool en)
{
    calibPointBtn->setChecked(false);
    calibPointBtn->setEnabled(en);
}

void GLGazeScene::setPointCalibMode(bool en)
{
    // Notify gaze-tracker of calibration status
    store->calibMode = en;

    if(en)
    {
        // If calibration button checked, start the timer.
        calibPointTimer->start(CALIB_TIME_PER_POINT * 1000);
    }
    else
    {
        // Stop the timer from doing anything else and
        // set up the next calibration point.
        calibPointTimer->stop();
        store->calibX = store->calibX >= calibW ? 0 : store->calibX + 1;
        store->calibY = store->calibX ? store->calibY : store->calibY - 1;
        store->calibY = store->calibY < 0 ? calibH : store->calibY;
    }
}


// Shortcut keys to simplify calibration
void GLGazeScene::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_F4)
        calibModeBtn->toggle();
    else if(event->key() == Qt::Key_Return && calibPointBtn->isEnabled())
        calibPointBtn->toggle();

    QGraphicsScene::keyPressEvent(event);
}
