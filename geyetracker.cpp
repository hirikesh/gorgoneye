#include "geyetracker.h"
#include "ui_geyetracker.h"

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
}

GEyeTracker::~GEyeTracker()
{
    delete ui;
}

void GEyeTracker::procFrame()
{
    capture >> image;
    cvtColor(image, image, CV_BGR2RGB);
    this->update();
}

void GEyeTracker::paintEvent(QPaintEvent* e)
{
    QPainter p(this);
    p.drawImage(QPoint(ui->trackView->x(),ui->trackView->y()), m_i);
}
