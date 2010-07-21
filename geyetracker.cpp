#include "geyetracker.h"
#include "ui_geyetracker.h"
#include "geyedetector.h"
GEyeTracker::GEyeTracker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GEyeTracker)
{
    ui->setupUi(this);

    capture = VideoCapture(0);
    capture >> image;
    m_i = QImage(image.data,
                 image.size().width,
                 image.size().height,
                 QImage::Format_RGB888);

    m_timer = new QTimer(this);
    m_timer->setInterval(30);

    connect(m_timer, SIGNAL(timeout()), this, SLOT(procFrame()));
    connect(ui->startBtn, SIGNAL(clicked()), m_timer, SLOT(start()));
    connect(ui->stopBtn, SIGNAL(clicked()), m_timer, SLOT(stop()));
    ged = GEyeDetector(&image, 1.1, 4, Size(96,132));
}

GEyeTracker::~GEyeTracker()
{
    delete ui;
}

void GEyeTracker::procFrame()
{
    capture >> image;
    cvtColor(image, image, CV_BGR2RGB);
    //Rect r = ged.detect();
    //m_faceLoc = QRect(QPoint(r.x,r.y),QSize(r.width,r.height));
    this->update();
}

void GEyeTracker::paintEvent(QPaintEvent* e)
{
    QPainter p(this);
    p.drawImage(QPoint(ui->trackView->x(),ui->trackView->y()), m_i);

    if(m_faceLoc.x() > 0 && m_faceLoc.y() > 0)
    {
        p.setBrush(Qt::NoBrush);
        p.setPen(QColor(255,0,0));
        p.drawRect(QRect(m_faceLoc.x()+ui->trackView->x(),
                         m_faceLoc.y()+ui->trackView->y(),
                         m_faceLoc.width(),
                         m_faceLoc.height()));
    }
}
