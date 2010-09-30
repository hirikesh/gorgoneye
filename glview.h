#ifndef GLVIEW_H
#define GLVIEW_H

#include <cv.h>
#include <qgl.h>

class GLView : public QGLWidget
{
    Q_OBJECT
public:
  GLView(QWidget *parent = 0);
  ~GLView();

  QSize minimumSizeHint() const;
  QSize sizeHint() const;
  void loadGLTextures(const cv::Mat&);
  void drawROIs();
  void setCurrROI(int x, int y, int w, int h);

protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int w, int h);
  void CheckGLError(const char*);

private:
  void drawLine();
  QRect* currROI;
  GLuint texture;

};

#endif // GLVIEW_H
