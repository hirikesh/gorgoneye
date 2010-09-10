#ifndef GLVIEW_H
#define GLVIEW_H

#include <qgl.h>
#include <qglobal.h>

#include <cv.h>
using namespace cv;

class GLView : public QGLWidget
{
    Q_OBJECT
public:
  GLView(QImage* renderImg, QWidget *parent = 0);
  ~GLView();

  QSize minimumSizeHint() const;
  QSize sizeHint() const;
//  void loadGLTextures(const QImage&);
  void loadGLTextures(const Mat&);
  //void drawROIs(QRect&);
  void setCurrROI(QRect *);
protected:
  void initializeGL();
  void paintEvent(QPaintEvent *event);
  void paintGL();
  void resizeGL(int w, int h);

signals:

public slots:
private:
    void glEnable2D();
    void glDisable2D();
    QImage* renderImg;
    QRect* currROI;
    GLuint texture[1];

};

#endif // GLVIEW_H
