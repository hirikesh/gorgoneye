#include <QImage>
#include <QDebug>
#include "glview.h"
#include <GL/glext.h>

GLView::GLView(QWidget *parent) :
    QGLWidget(parent),
    currROI(new QRect(-1, -1, 0, 0))
{
    qDebug() << "Inside Constructor";
}

GLView::~GLView()
{
}

QSize GLView::minimumSizeHint() const
{
    return QSize(640, 480);
}

QSize GLView::sizeHint() const
{
    return QSize(640, 480);
}

void GLView::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // set the window clear color to black
    glShadeModel(GL_FLAT);
}

void GLView::resizeGL(int width, int height)
{
    if (!height)    // Avoid Divide-By-Zero
    { height = 1;}

    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 640, 0, 480);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void GLView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT); // clears window
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBegin(GL_QUADS);  // Draw A Quad
    glTexCoord2d(0, 0); glVertex2d(0, 480); // Top Left
    glTexCoord2d(0, 1); glVertex2d(0, 0); // Bottom Left
    glTexCoord2d(1, 1); glVertex2d(640, 0); // Bottom Right
    glTexCoord2d(1, 0); glVertex2d(640, 480); // Top Right
    glEnd();
    glDisable(GL_TEXTURE_2D);
    drawROIs();
}

void GLView::loadGLTextures(const Mat& image)
{
    glGenTextures( 1, &texture[0] );
    glBindTexture( GL_TEXTURE_2D, texture[0] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, image.data);
}

void GLView::setCurrROI(int x, int y, int w, int h)
{
    currROI->setX(x);
    currROI->setY(y);
    currROI->setWidth(w);
    currROI->setHeight(h);
}

void GLView::drawROIs()
{
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0f, 1.0f, 0.5f);
    glVertex2d(currROI->x(), currROI->y());
    glVertex2d(currROI->right(), currROI->y());
    glVertex2d(currROI->right(), currROI->bottom());
    glVertex2d(currROI->x(), currROI->bottom());
    glEnd();
    glColor3f(0.0f, 0.0f, 0.0f);
}