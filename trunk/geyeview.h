#ifndef GEYEVIEW_H
#define GEYEVIEW_H

#include <QMainWindow>
#include <QPainter>
#include <QTimer>

#include <cv.h>
#include <highgui.h>
using namespace cv;

namespace Ui
{
    class geyeview;
}

// Capture class based on:
//  http://www.morethantechnical.com/2009/03/05/qt-opencv-combined-for-face-detecting-qwidgets/
class geyeview : public QMainWindow {
    Q_OBJECT
public:
    geyeview(QWidget *parent = 0);
    ~geyeview();

public slots:
    // Complete gaze tracking functionality
    //   (using methods from GE detector class)
    void geProcess();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::geyeview *ui;

    // GorgonEye detector class instance
    // STUB FOR NOW

    // Default timeout between frames
    enum { GRAB_MS = 30 };

    // Image buffers
    QImage ge_img_qt;
    Mat ge_img;
    // STUB: Other variables here


    // OpenCV capture object
    VideoCapture ge_cap;
    // Timer to grab frames
    QTimer *ge_cap_timer;

    // Draws frame to qt ui
    void paintEvent(QPaintEvent *e);
};

#endif // GEYEVIEW_H
