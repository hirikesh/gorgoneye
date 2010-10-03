#include "glview.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <QDebug>

GLView::GLView(QWidget *parent) :
    QGLWidget(parent),
    faceROI(new QRect(-1, -1, 0, 0)),
    eyesROI(new QRect(-1, -1, 0, 0))
{
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
    glGenTextures(1, &texture);

    npotCapable = isExtensionSupported("GL_ARB_texture_non_power_of_two");
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
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBegin(GL_QUADS);  // Draw A Quad
    if(npotCapable) {
        glTexCoord2i(0, 0); glVertex2d(0, 480); // Top Left
        glTexCoord2i(1, 0); glVertex2d(640, 480); // Top Right
        glTexCoord2i(1, 1); glVertex2d(640, 0); // Bottom Right
        glTexCoord2i(0, 1); glVertex2d(0, 0); // Bottom Left
    } else {
        glTexCoord2i(0, 0); glVertex2d(0, 512); // Top Left
        glTexCoord2i(1, 0); glVertex2d(1024, 512); // Top Right
        glTexCoord2i(1, 1); glVertex2d(1024, 0); // Bottom Right
        glTexCoord2i(0, 1); glVertex2d(0, 0); // Bottom Left
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    drawROIs(faceROI);
    drawROIs(eyesROI);
}

void GLView::loadGLTextures(const cv::Mat& image)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if(npotCapable) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, image.data);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 512, 0, GL_BGR, GL_UNSIGNED_BYTE, NULL);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 32, image.cols, image.rows, GL_BGR, GL_UNSIGNED_BYTE, image.data);
    }
}

void GLView::setFaceROI(int x, int y, int w, int h)
{
    faceROI->setX(x);
    faceROI->setY(480-y-h);
    faceROI->setWidth(w);
    faceROI->setHeight(h);
}

void GLView::setEyesROI(int x, int y, int w, int h)
{
    eyesROI->setX(x);
    eyesROI->setY(480-y-h);
    eyesROI->setWidth(w);
    eyesROI->setHeight(h);
}

void GLView::drawROIs(QRect* ROI)
{
    glLineWidth(2.0);
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0f, 1.0f, 0.5f);
    glVertex2i(ROI->left(), ROI->top());
    glVertex2i(ROI->right(), ROI->top());
    glVertex2i(ROI->right(), ROI->bottom());
    glVertex2i(ROI->left(), ROI->bottom());
    glEnd();
    glColor3f(0.0f, 0.0f, 0.0f);
}

void GLView::CheckGLError(const char* msg)
{
    GLuint err = glGetError();
    if (err != GL_NO_ERROR)
        qDebug() << err << msg;
}


int GLView::isExtensionSupported(const char *extension)
{
    const GLubyte *extensions = NULL;
    const GLubyte *start;

    GLubyte *where, *terminator;
    /* Extension names should not have spaces. */

    where = (GLubyte *) strchr(extension, ' ');

    if (where || *extension == '\0')
        return 0;

    extensions = glGetString(GL_EXTENSIONS);

    /* It takes a bit of care to be fool-proof about parsing the
     OpenGL extensions string. Don't be fooled by sub-strings,
     etc. */
    start = extensions;

    while(1) {
        where = (GLubyte *) strstr((const char *) start, extension);
        if (!where)
            break;
        terminator = where + strlen(extension);
        if (where == start || *(where - 1) == ' ')
            if (*terminator == ' ' || *terminator == '\0')
                return 1;

        start = terminator;
    }

    return 0;
}
