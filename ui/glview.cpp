#include <QDebug>
#include <GL/gl.h>
#include <GL/glext.h>
#include "glview.h"
#include "store.h"

GLView::GLView(Store *st, QWidget *parent) :
    QGLWidget(parent),
    store(st)
{
    setMinimumSize(FRAME_WIDTH, FRAME_HEIGHT);
}

void GLView::initializeGL()
{
    // Generate a single simple texture, setup its environment for use
    // and use it for the entire session, because we can.
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Check for NPOT extension support and workaround it if missing
//    npotCapable = isExtensionSupported("GL_ARB_texture_non_power_of_two");
}

void GLView::resizeGL(int width, int height)
{
    height = height < 1 ? 1 : height; // avoid divide by 0
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, FRAME_WIDTH, 0, FRAME_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void GLView::paintGL()
{
    drawGLTexture();
    drawGLRois();
}


// Drawing functions

void GLView::drawGLTexture()
{
    // Update texture with new frame data
    updateGLTexture();

    // Render the texture
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);  // Draw A Quad
        glTexCoord2i(0, 0); glVertex2d(0, FRAME_HEIGHT); // Top Left
        glTexCoord2i(1, 0); glVertex2d(FRAME_WIDTH, FRAME_HEIGHT); // Top Right
        glTexCoord2i(1, 1); glVertex2d(FRAME_WIDTH, 0); // Bottom Right
        glTexCoord2i(0, 1); glVertex2d(0, 0); // Bottom Left
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void GLView::updateGLTexture()
{
    // Draw scene first and always
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 store->sceneImg.cols, store->sceneImg.rows,
                 0, GL_BGR, GL_UNSIGNED_BYTE, store->sceneImg.data);

    // Draw visualisations over scene if requested
    if(store->dispImg->data)
    {
        if(store->dispImg->size() == store->faceImg.size())
        {
            // Don't draw face visualisation if face wasn't found
            if(store->faceLocated)
                glTexSubImage2D(GL_TEXTURE_2D, 0,
                                store->faceRoi.x, store->faceRoi.y,
                                store->dispImg->cols, store->dispImg->rows,
                                GL_BGR, GL_UNSIGNED_BYTE, store->dispImg->data);
        }
        else if(store->dispImg->size() == store->eyesImg.size())
        {
            // Don't draw eyes visualisation if face wasn't found
            if(store->eyesLocated)
                glTexSubImage2D(GL_TEXTURE_2D, 0,
                                store->faceRoi.x + store->eyesRoi.x,
                                store->faceRoi.y + store->eyesRoi.y,
                                store->dispImg->cols, store->dispImg->rows,
                                GL_BGR, GL_UNSIGNED_BYTE, store->dispImg->data);
        }
        else
        {
            // Always draw this - it's probably a downsampled scene visualisation
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                         store->dispImg->cols, store->dispImg->rows,
                         0, GL_BGR, GL_UNSIGNED_BYTE, store->dispImg->data);
        }
    }
}

void GLView::drawGLRois()
{
    // Calculate frame positions
    int tlx = store->faceRoi.tl().x;
    int tly = FRAME_HEIGHT-store->faceRoi.tl().y;
    int brx = store->faceRoi.br().x;
    int bry = FRAME_HEIGHT-store->faceRoi.br().y;

    // Draw face rect. only if located
    if(store->faceLocated)
        drawGLRoi(tlx, tly, brx, bry);

    // Draw eyes rect. only if located
    if(store->eyesLocated)
        drawGLRoi(tlx + store->eyesRoi.tl().x,
                tly - store->eyesRoi.tl().y,
                tlx + store->eyesRoi.br().x,
                tly - store->eyesRoi.br().y);
}

void GLView::drawGLRoi(int tlx, int tly, int brx, int bry)
{
    glLineWidth(1.0);
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2i(tlx, tly); // top left
        glVertex2i(brx, tly); // top right
        glVertex2i(brx, bry); // bottom right
        glVertex2i(tlx, bry); // bottom left
    glEnd();
}


// Debugging functions

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
