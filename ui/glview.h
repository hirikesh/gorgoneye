#ifndef GLVIEW_H
#define GLVIEW_H

#include <cv.h>
#include <qgl.h>

class GLView : public QGLWidget
{
    Q_OBJECT
public:
    GLView(std::vector<bool*> ivr, QWidget *parent = 0);
    ~GLView();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void loadGLTextures(const cv::Mat&);
    void drawROIs(QRect* ROI);
    void setFaceROI(int x, int y, int w, int h);
    void setEyesROI(int x, int y, int w, int h);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void CheckGLError(const char*);
    int isExtensionSupported(const char *extension);

private:
    void drawLine();
    QRect* faceROI;
    QRect* eyesROI;
    GLuint texture;
    int npotCapable;
    std::vector<bool*> isValidRoi;

};

#endif // GLVIEW_H
