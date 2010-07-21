#include "geyeview.h"
#include "ui_geyeview.h"

geyeview::geyeview(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::geyeview)
{
    ui->setupUi(this);

    // create video-from-webcam capture object
    ge_cap = VideoCapture(0);

    // create opencv mat image using single frame capture
    ge_cap >> ge_img;

    // create qt image container using mat image data
    ge_img_qt = QImage(ge_img.data,
                       ge_img.size().width,
                       ge_img.size().height,
                       QImage::Format_RGB888);

    // Webcam frame-grab timer
    ge_cap_timer = new QTimer(this);
    connect(ge_cap_timer,SIGNAL(timeout()),this,SLOT(geProcess()));
    ge_cap_timer->start(GRAB_MS);
}

geyeview::~geyeview()
{
    delete ui;
}

void geyeview::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void geyeview::geProcess()
{
    // Grab frame and put into shared buffer
    ge_cap >> ge_img;

    // Process the frames
    // *methods from gorgoneye detector class go here*

    // Signal for a repaint event
    this->update();
}

void geyeview::paintEvent(QPaintEvent *e)
{
    // Paint image centered inside the frame widget
    QPainter p(this);
    //p.drawImage(QPoint(ui->frame->x()+3,ui->frame->y()+3),ge_img_qt);
}
