#include <qgl.h>
#include "glgazescene.h"

GLGazeScene::GLGazeScene(Store* st, int w, int h, QObject* parent) :
    QGraphicsScene(0, 0, w, h, parent),
    store(st)
{

}

void GLGazeScene::drawBackground(QPainter *painter, const QRectF &)
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

    // Draw calibration visualisation
//    glLineWidth(1.0);
    glBegin(GL_LINES);
    glColor3f(0.5f, 0.5f, 0.5f);
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
    glColor3f(0.0f, 0.0f, 0.0f);

    // End drawing background
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
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
