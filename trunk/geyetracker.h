#ifndef GEYETRACKER_H
#define GEYETRACKER_H

#include <QWidget>
#include <QPainter>
#include <QTimer>

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
private:
    Ui::GEyeTracker *ui;

    QImage m_i;
    QRect* m_faceLoc;
    QTimer* m_timer;
    VideoCapture capture;
    Mat image;

    void paintEvent(QPaintEvent* e);
};

#endif // GEYETRACKER_H
