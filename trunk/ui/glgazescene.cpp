#include <QKeyEvent>
#include <QGraphicsItem>
#include <qgl.h>

#include "glgazescene.h"
#include "store.h"

GLGazeScene::GLGazeScene(Store* st, float scale, int dpix, int dpiy, QObject* parent) :
    dpiScale(scale), dpiX(dpix), dpiY(dpiy),
    store(st)
{
}

// Called by GLGaze when window is resized
void GLGazeScene::updateCalib()
{
    // Some variables for convenience
    deltaX = dpiScale * dpiX;
    deltaY = dpiScale * dpiY;
    outerW = width()/2;
    outerH = height()/2;

    // Update 'store' with coordinate system where center pixel is (0,0)
    store->gazeDeltaX = dpiScale * dpiX;
    store->gazeDeltaY = dpiScale * dpiY;
    store->gazeOuterX = deltaX * floor(outerW/deltaX);
    store->gazeOuterY = deltaY * floor(outerH/deltaY);

    // Reset calibration starting point
    store->calibX = 0;
    store->calibY = 0;

    // Reset hysteresis initial condition
    hystThreshX = deltaX;
    hystThreshY = deltaY;
    currGazeX = 0;
    currGazeY = 0;
}


// Main drawing functions
void GLGazeScene::drawBackground(QPainter* painter, const QRectF& rect)
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

    // Black/gray background
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Hysteresis effect for gaze estimation
    if(store->gazeLocated)
    {
        if(abs(currGazeX - store->gazeX) > hystThreshX)
            currGazeX -= hystThreshX * floor((currGazeX - store->gazeX) / hystThreshX);
        if(abs(currGazeY - store->gazeY) > hystThreshY)
            currGazeY -= hystThreshX * floor((currGazeY - store->gazeY) / hystThreshY);
        glPointSize(hystThreshX);
        glColor3f(0.7f, 0.7f, 0.7f);
        glBegin(GL_POINTS);
            glVertex2i(outerW + currGazeX,
                       outerH - currGazeY);
        glEnd();
    }

    // Draw tracking info
    if(store->faceLocated)
    {
        glPointSize(24.0);
        glColor3f(0.1f, 0.1f, 0.9f);
        glBegin(GL_POINTS);
            glVertex2i(outerW + store->calibX,
                       outerH - store->calibY);
        glEnd();
    }
    if(store->eyesLocated && store->eyesLocatedL)
    {
        glPointSize(20.0);
        glColor3f(0.9f, 0.2f, 0.2f);
        glBegin(GL_POINTS);
            glVertex2i(outerW + store->calibX,
                       outerH - store->calibY);
        glEnd();
    }

    // Draw calibration grid
    glLineWidth(1.0);
    glBegin(GL_LINES);
        // Draw calibration lines - intersection is calibration coordinate
        glColor3f(0.5f, 0.5f, 0.5f);
        for(float nextx = 0; nextx <= outerW; nextx = nextx + deltaX/2)
        {
            glVertex2i(outerW-nextx, 0);
            glVertex2i(outerW-nextx, height());
            glVertex2i(outerW+nextx, 0);
            glVertex2i(outerW+nextx, height());
        }
        for(float nexty = 0; nexty <= outerH; nexty = nexty + deltaY/2)
        {
            glVertex2i(0, outerH-nexty);
            glVertex2i(width(), outerH-nexty);
            glVertex2i(0, outerH+nexty);
            glVertex2i(width(), outerH+nexty);
        }
    glEnd();

    // End drawing background
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

void GLGazeScene::drawForeground(QPainter* painter, const QRectF& rect)
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
    if(store->calibMode)
    {
        glPointSize(16.0);
        if(store->calibNow)
            glColor3f(0.3f, 0.9f, 0.1f);
        else
            glColor3f(0.7f, 0.7f, 0.0f);
        glBegin(GL_POINTS);
            glVertex2i(outerW + store->calibX,
                       outerH - store->calibY); // flip Y to match OpenCV coordinates
        glEnd();
    }

    // Draw gaze estimation
    if(store->gazeLocated)
    {
        glPointSize(12.0);
        glColor3f(0.3f, 0.9f, 0.1f);
        glBegin(GL_POINTS);
            glVertex2i(outerW + store->gazeX,
                       outerH - store->gazeY);
        glEnd();
    }

    // End drawing foreground
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}


// Shortcut keys to simplify calibration
void GLGazeScene::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Return)
        store->calibMode = !store->calibMode;

    QGraphicsScene::keyPressEvent(event);
}
