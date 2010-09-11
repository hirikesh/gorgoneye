#ifndef GEYETRACKER_H
#define GEYETRACKER_H

#include <cv.h>
#include <QWidget>
#include <QRect>
#include "model.h"

class QVBoxLayout;
class BaseDetector;
class GLView;

namespace Ui {
    class GEyeTracker;
}

class GEyeTracker : public QWidget
{
    Q_OBJECT

public:
    explicit GEyeTracker(QWidget *parent = 0);
    ~GEyeTracker();
    void createTrackerGUI(BaseTracker*);
    void createDetectorGUI(BaseDetector*, QVBoxLayout*);

public slots:
    void procFrame();  
    void setParam(int* const, int);
    void setParam(bool* const, bool);
    void setParam(double* const, double);
    void setImage(cv::Mat* const, bool);
private:
    void initGUI();
    Ui::GEyeTracker *ui;
//    QImage* qFaceImg;
    QRect faceLoc;
    QTimer* timer;
    Model model;
    GLView* opengl;
//    void paintEvent(QPaintEvent* e);
};

#endif // GEYETRACKER_H
