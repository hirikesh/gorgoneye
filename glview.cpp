#include "glview.h"
#include <GL/glext.h>
#include <QImage>


GLView::GLView(QWidget *parent) :
    QGLWidget(parent),
    currROI(new QRect(-1,-1,0,0))
{}

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
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // clears window to RGB 000 ie. black
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_FLAT);
    glDisable(GL_DEPTH_TEST);    // Disables Depth Testing
    glClear(GL_COLOR_BUFFER_BIT);

}

void GLView::resizeGL(int width, int height)
{
    if (height == 0)    // Avoid Divide-By-Zero
    { height = 1;}

    glViewport(0, 0, width, height);	// Reset The Current Viewport
    glMatrixMode(GL_PROJECTION);	// Select The Projection Matrix
    glLoadIdentity();			// Reset The Projection Matrix
    gluOrtho2D(0, 640, 0, 480);
    glMatrixMode(GL_MODELVIEW);     // Select The Modelview Matrix
    glLoadIdentity();               // Reset The Modelview Matrix
}

void GLView::paintGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // clears window to RGB 000 ie. black
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_FLAT);
    glDisable(GL_DEPTH_TEST);    // Disables Depth Testing
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, this->width(), this->height());	// Reset The Current Viewport
    glMatrixMode(GL_PROJECTION);	// Select The Projection Matrix
    glLoadIdentity();			// Reset The Projection Matrix
    gluOrtho2D(0, 640, 0, 480);
    glMatrixMode(GL_MODELVIEW);     // Select The Modelview Matrix
    glLoadIdentity();               // Reset The Modelview Matrix

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glEnable2D();
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_QUADS);  // Draw A Quad
    glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 480); // Top Left
    glTexCoord2f(0.0f, 1.0f); glVertex2f(0, 0); // Bottom Left
    glTexCoord2f(1.0f, 1.0f); glVertex2f(640, 0); // Bottom Right
    glTexCoord2f(1.0f, 0.0f); glVertex2f(640, 480); // Top Right
    glEnd();
    glDisable2D();
    update();
}

void GLView::loadGLTextures(const Mat& image)
{
    glGenTextures( 1, &texture[0] );
    glBindTexture( GL_TEXTURE_2D, texture[0] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, image.data);
}

void GLView::glEnable2D()
{
    //int vPort[4];

    //glGetIntegerv(GL_VIEWPORT, vPort);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    //glOrtho(0, vPort[2], 0, vPort[3], -1, 1);
    gluOrtho2D(0, 640, 0, 480);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

void GLView::glDisable2D()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void GLView::setCurrROI(QRect * r)
{
    currROI = r;
}
