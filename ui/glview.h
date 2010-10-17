#ifndef GLVIEW_H
#define GLVIEW_H

#include <qgl.h>
#include "config.h"

class Store;

class GLView : public QGLWidget
{
    Q_OBJECT
public:
    GLView(Store* st, QWidget* parent = 0);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void updateGLTexture();
    void drawGLTexture();
    void drawGLRois();
    void drawGLRoi(int, int, int, int);

    void CheckGLError(const char*);
    int isExtensionSupported(const char *extension);

private:
    Store* store;
    GLuint texture;
};

#endif // GLVIEW_H
