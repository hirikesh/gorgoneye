#ifndef GEYETRACKER_H
#define GEYETRACKER_H

#include <QWidget>
#include <QPainter>
#include <QTimer>

#include "geyedetector.h"
#include <cv.h>
#include <highgui.h>
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

public slots:
    void procFrame();
    void setScale(double);
    void setWidth(int);
    void setHeight(int);
    void setMinN(int);
    void disableParams();
    void enableParams();
private:
    Ui::GEyeTracker *ui;

    QImage m_i;
    QRect m_faceLoc;
    QTimer* m_timer;
    VideoCapture capture;
    Mat image;
    GEyeDetector ged;
    void paintEvent(QPaintEvent* e);
};

#endif // GEYETRACKER_H
