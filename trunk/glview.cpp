#include <QImage>
#include "glview.h"
#include <GL/glext.h>

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
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // set the window clear color to black
    glEnable(GL_TEXTURE_2D);
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
    gluLookAt(0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void GLView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT); // clears window
    glLoadIdentity();
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_QUADS);  // Draw A Quad
    glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 480); // Top Left
    glTexCoord2f(0.0f, 1.0f); glVertex2f(0, 0); // Bottom Left
    glTexCoord2f(1.0f, 1.0f); glVertex2f(640, 0); // Bottom Right
    glTexCoord2f(1.0f, 0.0f); glVertex2f(640, 480); // Top Right
    glEnd();
    update();
}

void GLView::loadGLTextures(const Mat& image)
{
    glGenTextures( 1, &texture[0] );
    glBindTexture( GL_TEXTURE_2D, texture[0] );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, image.data);
}

void GLView::setCurrROI(QRect * r)
{
    currROI = r;
}
