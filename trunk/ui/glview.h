#ifndef GLVIEW_H
#define GLVIEW_H

#include <qgl.h>

class Store;

class GLView : public QGLWidget
{
    Q_OBJECT
public:
    GLView(Store* st, QWidget* parent = 0);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void loadGLTextures();
    void drawRois();

protected:
    void initializeGL();
    void resizeGL(int w, int h);

    void paintGL();
    void drawRoi(int, int, int, int);

    void CheckGLError(const char*);
    int isExtensionSupported(const char *extension);

private:
    Store* store;
    GLuint texture;
    int npotCapable;
};

#endif // GLVIEW_H
