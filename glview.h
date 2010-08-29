#ifndef GLVIEW_H
#define GLVIEW_H

#include <qgl.h>

class GLView : public QGLWidget
{
    Q_OBJECT
public:
  GLView(QImage* renderImg, QWidget *parent = 0);
  ~GLView();

  QSize minimumSizeHint() const;
  QSize sizeHint() const;
  void loadGLTextures(const QImage&);
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
