#include "store.h"
#include "glview.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <QDebug>

GLView::GLView(Store *st, QWidget *parent) :
    QGLWidget(parent),
    store(st)
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
    if(npotCapable)
    {
        glTexCoord2i(0, 0); glVertex2d(0, 480); // Top Left
        glTexCoord2i(1, 0); glVertex2d(640, 480); // Top Right
        glTexCoord2i(1, 1); glVertex2d(640, 0); // Bottom Right
        glTexCoord2i(0, 1); glVertex2d(0, 0); // Bottom Left
    } else
    {
//        glTexCoord2i(0, 0); glVertex2d(0, 512); // Top Left
//        glTexCoord2i(1, 0); glVertex2d(1024, 512); // Top Right
//        glTexCoord2i(1, 1); glVertex2d(1024, 0); // Bottom Right
//        glTexCoord2i(0, 1); glVertex2d(0, 0); // Bottom Left
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    drawRois();
}

void GLView::loadGLTextures()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    if(npotCapable)
    {
        // Draw scene first
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                     store->sceneImg.cols, store->sceneImg.rows,
                     0, GL_BGR, GL_UNSIGNED_BYTE, store->sceneImg.data);

        // Draw visualisations over scene if requested
        if(store->dispImg->data)
        {
            if(store->dispImg->size() == store->faceImg.size())
            {
                glTexSubImage2D(GL_TEXTURE_2D, 0,
                                store->faceRoi.x, store->faceRoi.y,
                                store->dispImg->cols, store->dispImg->rows,
                                GL_BGR, GL_UNSIGNED_BYTE, store->dispImg->data);
            }
            else if(store->dispImg->size() == store->eyesImg.size())
            {
                glTexSubImage2D(GL_TEXTURE_2D, 0,
                                store->faceRoi.x + store->eyesRoi.x,
                                store->faceRoi.y + store->eyesRoi.y,
                                store->dispImg->cols, store->dispImg->rows,
                                GL_BGR, GL_UNSIGNED_BYTE, store->dispImg->data);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                             store->dispImg->cols, store->dispImg->rows,
                             0, GL_BGR, GL_UNSIGNED_BYTE, store->dispImg->data);
            }
        }
    } else
    {
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 512, 0, GL_BGR, GL_UNSIGNED_BYTE, NULL);
//        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 32, image.cols, image.rows, GL_BGR, GL_UNSIGNED_BYTE, image.data);
    }
}

void GLView::drawRois()
{
    int tlx = store->faceRoi.tl().x;
    int tly = 480-store->faceRoi.tl().y;
    int brx = store->faceRoi.br().x;
    int bry = 480-store->faceRoi.br().y;
    if(store->faceLocated)
        drawRoi(tlx, tly, brx, bry);

    if(store->eyesLocated)
        drawRoi(tlx + store->eyesRoi.tl().x,
                tly - store->eyesRoi.tl().y,
                tlx + store->eyesRoi.br().x,
                tly - store->eyesRoi.br().y);
}

void GLView::drawRoi(int tlx, int tly, int brx, int bry)
{
//    glLineWidth(1.0);
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0f, 1.0f, 0.5f);
    glVertex2i(tlx, tly); // top left
    glVertex2i(brx, tly); // top right
    glVertex2i(brx, bry); // bottom right
    glVertex2i(tlx, bry); // bottom left
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
