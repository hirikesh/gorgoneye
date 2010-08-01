#include "geyetracker.h"
#include "ui_geyetracker.h"
#include "geyedetector.h"
GEyeTracker::GEyeTracker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GEyeTracker)
{
    #define HAAR_CC_FACE_DEFAULT "c:\\opencv2.1\\data\\haarcascades\\haarcascade_frontalface_default.xml"

    ui->setupUi(this);

    capture = VideoCapture(0);
    capture >> image;

    ged = GEyeDetector(&image, 1.2, 2, Size(160,160));
    ged.setCC(new CascadeClassifier(HAAR_CC_FACE_DEFAULT));

    qImage = QImage(image.data,
                 image.size().width,
                 image.size().height,
                 QImage::Format_RGB888);

    timer = new QTimer(this);
    timer->setInterval(40); // timer signals every N ms
    connect(timer, SIGNAL(timeout()), this, SLOT(procFrame()));

    connect(ui->startBtn, SIGNAL(clicked()), timer, SLOT(start()));
    connect(ui->startBtn, SIGNAL(clicked()), this, SLOT(disableParams()));
    connect(ui->stopBtn, SIGNAL(clicked()), timer, SLOT(stop()));
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

    ui->heightSpinBox->setValue(ged.getHeight());
    ui->widthSpinBox->setValue(ged.getWidth());
    ui->minNSpinBox->setValue(ged.getMinNeighbours());
    ui->scaleSpinBox->setValue(ged.getScale());
}

GEyeTracker::~GEyeTracker()
{
    delete ui;
}

// Event Handlers

void GEyeTracker::procFrame()
{
    capture >> image;
    Rect r = ged.detect();
    faceLoc = QRect(QPoint(r.x,r.y), QSize(r.width,r.height));
    this->update();
}

void GEyeTracker::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);
    painter.drawImage(QPoint(ui->trackView->x(),ui->trackView->y()), qImage);

    if(faceLoc.x() > 0 && faceLoc.y() > 0)
    {
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QColor(255,0,0));
        painter.drawRect(QRect(faceLoc.x()+ui->trackView->x(),
                         faceLoc.y()+ui->trackView->y(),
                         faceLoc.width(),
                         faceLoc.height()));
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
    ged.setWidth(w);
}

void GEyeTracker::setHeight(int h)
{
    ged.setHeight(h);
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