#ifndef GEYETRACKER_H
#define GEYETRACKER_H

#include <QWidget>
#include <QPainter>
#include <QTimer>

#include "model.h"
#include "detectors/basedetector.h"
#include "parameter.h"
#include "geyedetector.h"
#include <cv.h>
#include <highgui.h>
#include <QLayout>

using namespace cv;

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
    void disableParams();
    void enableParams();
    void setParam(int* const, int);
    void setParam(bool* const, bool);
    void setParam(double* const, double);
private:
    Ui::GEyeTracker *ui;
    Model model;
    QImage* qFaceImg;
    QRect faceLoc;
    QTimer* timer;
//    VideoCapture capture;
    Mat image;
//    GEyeDetector ged;
    void paintEvent(QPaintEvent* e);
};

#endif // GEYETRACKER_H
