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

    ged = GEyeDetector(&image, 1.1, 4, Size(96,132));

    m_i = QImage(image.data,
                 image.size().width,
                 image.size().height,
                 QImage::Format_RGB888);

    m_timer = new QTimer(this);
    m_timer->setInterval(40);

    connect(m_timer, SIGNAL(timeout()), this, SLOT(procFrame()));
    connect(ui->startBtn, SIGNAL(clicked()), m_timer, SLOT(start()));
    connect(ui->startBtn, SIGNAL(clicked()), this, SLOT(disableParams()));
    connect(ui->stopBtn, SIGNAL(clicked()), m_timer, SLOT(stop()));
    connect(ui->stopBtn, SIGNAL(clicked()), this, SLOT(enableParams()));

    connect(ui->heightSlider, SIGNAL(valueChanged(int)), ui->heightSpinBox, SLOT(setValue(int)));
    connect(ui->heightSpinBox, SIGNAL(valueChanged(int)), ui->heightSlider, SLOT(setValue(int)));
    connect(ui->widthSlider, SIGNAL(valueChanged(int)), ui->widthSpinBox, SLOT(setValue(int)));
    connect(ui->widthSpinBox, SIGNAL(valueChanged(int)), ui->widthSlider, SLOT(setValue(int)));
    connect(ui->minNSlider, SIGNAL(valueChanged(int)), ui->minNSpinBox, SLOT(setValue(int)));
    connect(ui->minNSpinBox, SIGNAL(valueChanged(int)), ui->minNSlider, SLOT(setValue(int)));

    connect(ui->heightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setHeight(int)));
    connect(ui->widthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setWidth(int)));
    connect(ui->minNSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setMinN(int)));
    connect(ui->scaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setScale(double)));

    ui->heightSpinBox->setValue(ged.size().height);
    ui->widthSpinBox->setValue(ged.size().width);
    ui->minNSpinBox->setValue(ged.minneighbours());
    ui->scaleSpinBox->setValue(ged.scale());

}

GEyeTracker::~GEyeTracker()
{
    delete ui;
}

void GEyeTracker::procFrame()
{
    capture >> image;
    cvtColor(image, image, CV_BGR2RGB);
    Rect r = ged.detect();
    m_faceLoc = QRect(QPoint(r.x,r.y),QSize(r.width,r.height));
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

void GEyeTracker::setScale(double sf)
{
    ged.setScaleFactor(sf);
}

void GEyeTracker::setMinN(int mn)
{
    ged.setMinNeighbours(mn);
}

void GEyeTracker::setWidth(int w)
{
    ged.setMinSize(Size(w, ged.size().height));
}

void GEyeTracker::setHeight(int h)
{
    ged.setMinSize(Size(ged.size().width, h));
}

void GEyeTracker::disableParams()
{
    ui->scaleSpinBox->setEnabled(false);
    ui->widthSlider->setEnabled(false);
    ui->widthSpinBox->setEnabled(false);
    ui->heightSlider->setEnabled(false);
    ui->heightSpinBox->setEnabled(false);
    ui->minNSlider->setEnabled(false);
    ui->minNSpinBox->setEnabled(false);
}

void GEyeTracker::enableParams()
{
    ui->scaleSpinBox->setEnabled(true);
    ui->widthSlider->setEnabled(true);
    ui->widthSpinBox->setEnabled(true);
    ui->heightSlider->setEnabled(true);
    ui->heightSpinBox->setEnabled(true);
    ui->minNSlider->setEnabled(true);
    ui->minNSpinBox->setEnabled(true);
}
