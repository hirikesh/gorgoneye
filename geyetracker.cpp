#include "geyetracker.h"
#include "ui_geyetracker.h"
#include "geyedetector.h"
#include "parameter.h"
#include "guiparam.h"
#include "detectors/haar.h"
#include "model.h"
#include "trackers/face.h"
#include <QDebug>
GEyeTracker::GEyeTracker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GEyeTracker)
{
    #define HAAR_CC_FACE_DEFAULT "c:\\opencv2.1\\data\\haarcascades\\haarcascade_frontalface_default.xml"

    ui->setupUi(this);


    //Model* model = new Model(new Store);
    //vector<Param*> params = model->getTrackerParams();
    //Tracker* face1 = new Face(new Store);
    //face1->setDetector(Face::HAAR);
    //Detector* ccHaar = face1->getDetector();

    // This Code works.
    Detector* ccHaar = new Haar(HAAR_CC_FACE_DEFAULT, 1.1, 3, NULL, Size(96,96));
    vector<Param*> params = ccHaar->getParams();
    qDebug() << params.size();
    // Dynamic GUI Generator
    // Should accept an vector of Params

    Vector<QWidget*> gparams(params.size());
    Vector<QGridLayout*> guiItems(params.size()); // item wrapper
    for (unsigned int i = 0; i < params.size(); i++)
    {
        guiItems[i] = new QGridLayout(); // create gui item
        if (params[i]->getType() == Param::MODE)
        {
            gparams[i] = new GUICheckBox((ModeParam*)params[i]); // create widget
            guiItems[i]->addWidget(gparams[i], 0, 0); // add to gui item
        }
        else if (params[i]->getType() == Param::RANGE)
        {
            gparams[i] = new GUISlider((RangeParam*)params[i]); // create widget
            // generate the other parts of the gui item
            guiItems[i]->addWidget(new QLabel(params[i]->getName().c_str()), 0, 0);
            guiItems[i]->addWidget(gparams[i], 1, 0);
            guiItems[i]->addWidget(new QLabel(QString("SpinBox")), 1, 1);

        }
        ui->paramLayout->addLayout(guiItems[i]);
        connect(gparams[i], SIGNAL(valueChanged(int* const, int)), this, SLOT(setParam(int* const, int)));
    }

    capture = VideoCapture(0);
    capture >> image;

    ged = GEyeDetector(&image);
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
    ui->heightSlider->setValue(ged.getHeight());
    ui->widthSlider->setValue(ged.getWidth());
    ui->minNSlider->setValue(ged.getMinNeighbours());
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

void GEyeTracker::setScale(const double& sf)
{
    ged.setScaleFactor(sf);
}

void GEyeTracker::setMinN(const int& mn)
{
    ged.setMinNeighbours(mn);
}

void GEyeTracker::setWidth(const int& w)
{
    ged.setWidth(w);
}

void GEyeTracker::setHeight(const int& h)
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

void GEyeTracker::setParam(int* const param, int value)
{
   *param = value;
   qDebug() << value;
}

void GEyeTracker::setParam(bool* const param, bool value)
{
   *param = value;
   qDebug() << value;
}

void GEyeTracker::setParam(double* const param, double value)
{
   *param = value;
   qDebug() << value;
}
