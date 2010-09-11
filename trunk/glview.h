#ifndef GLVIEW_H
#define GLVIEW_H

#include <cv.h>
#include <qgl.h>

using namespace cv;

class GLView : public QGLWidget
{
    Q_OBJECT
public:
  GLView(QWidget *parent = 0);
  ~GLView();

  QSize minimumSizeHint() const;
  QSize sizeHint() const;
  void loadGLTextures(const Mat&);
  void drawROIs();
  void setCurrROI(int x, int y, int w, int h);
protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int w, int h);

private:
  void drawLine();
  QRect* currROI;
  GLuint texture[1];

};

#endif // GLVIEW_H
