#include "geyetracker.h"
#include "ui_geyetracker.h"
#include "geyedetector.h"
#include "parameter.h"
#include "guiparam.h"
#include "model.h"
#include "trackers/basetracker.h"
#include "trackers/facetracker.h"
#include "detectors/basedetector.h"
#include "detectors/haardetector.h"
#include <QLabel>
#include <QDebug>
#include <QGroupBox>
#include <QComboBox>
GEyeTracker::GEyeTracker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GEyeTracker),
    model(Model(0)),
    qFaceImg(new QImage())
{
//    #define HAAR_CC_FACE_DEFAULT "c:\\opencv2.1\\data\\haarcascades\\haarcascade_frontalface_default.xml"

    ui->setupUi(this);

    vector<BaseTracker*> trackers = model.getTrackers();
    for (unsigned int i = 0; i < trackers.size(); i++)
    {
        if(trackers[i]->isEnabled())
        {
           createTrackerGUI(trackers[i]);
        }
    }

//    capture = VideoCapture(0);
    /***** TEST CODE START *****/
    Store* store = model.getStore();
    image = store->sceneImg;

    /***** TEST CODE END *****/
//    capture >> image;


//    ged = GEyeDetector(&image);
//    ged.setCC(new CascadeClassifier(HAAR_CC_FACE_DEFAULT));

    timer = new QTimer(this);
    timer->setInterval(40); // timer signals every N ms
    connect(timer, SIGNAL(timeout()), this, SLOT(procFrame()));

    connect(ui->startBtn, SIGNAL(clicked()), timer, SLOT(start()));
    connect(ui->startBtn, SIGNAL(clicked()), this, SLOT(disableParams()));
    connect(ui->stopBtn, SIGNAL(clicked()), timer, SLOT(stop()));
    connect(ui->stopBtn, SIGNAL(clicked()), this, SLOT(enableParams()));
}

GEyeTracker::~GEyeTracker()
{
    delete ui;
}

// Event Handlers

void GEyeTracker::procFrame()
{
//    capture >> image;
//    Rect r = ged.detect();
//    faceLoc = QRect(QPoint(r.x,r.y), QSize(r.width,r.height));
    /**** TEST CODE START ****/
    model.update();
    if (this->qFaceImg)
    {
        delete qFaceImg;
    }
    qFaceImg = new QImage(model.getFaceDispImg()->data,
                 model.getFaceDispImg()->size().width,
                 model.getFaceDispImg()->size().height,
                 QImage::Format_RGB888);

//    Rect r = model.getStore()->faceRoi;
//    faceLoc = QRect(QPoint(r.x,r.y), QSize(r.width,r.height));
    /**** TEST CODE ****/
    this->update();
}

void GEyeTracker::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);
    painter.drawImage(QPoint(ui->trackView->x(),ui->trackView->y()), *qFaceImg);

    Rect r = model.getStore()->faceRoi;
    if(r.area())
    {
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QColor(255,0,0));
//        painter.drawRect(QRect(faceLoc.x()+ui->trackView->x(),
//                         faceLoc.y()+ui->trackView->y(),
//                         faceLoc.width(),
//                         faceLoc.height()));
        painter.drawRect(QRect(r.x+ui->trackView->x(),
                         r.y+ui->trackView->y(),
                         r.width,
                         r.height));
    }
}

void GEyeTracker::disableParams()
{
}

void GEyeTracker::enableParams()
{
}

void GEyeTracker::setParam(int* const param, int value)
{
   *param = value;
}

void GEyeTracker::setParam(bool* const param, bool value)
{
   *param = value;
}

void GEyeTracker::setParam(double* const param, double value)
{
   *param = value;
}

void GEyeTracker::createTrackerGUI(BaseTracker* tracker)
{
    // Tracker Level
    QVBoxLayout* trackerLayout = new QVBoxLayout();
    QGridLayout* trackerTitle = new QGridLayout();
    ui->sideLayout->addLayout(trackerLayout);
    ui->sideLayout->setAlignment(trackerLayout, Qt::AlignTop);
    trackerLayout->addLayout(trackerTitle);

    string title = "Enable " + tracker->getName() + " Tracking";
    GUICheckBox *faceEnable = new GUICheckBox(title, tracker->getEnabled());
    faceEnable->setChecked(tracker->isEnabled());
    connect(faceEnable, SIGNAL(valueChanged(bool* const, bool)), this, SLOT(setParam(bool* const, bool)));

    trackerTitle->addWidget(faceEnable, 0, 0);
    trackerTitle->addWidget(new QComboBox(), 0, 1);
    vector<BaseDetector*> detectors = tracker->getDetectors();
    for (unsigned int i = 0; i < detectors.size(); i++)
    {
        createDetectorGUI(detectors[i], trackerLayout);
    }
}

void GEyeTracker::createDetectorGUI(BaseDetector* detector, QVBoxLayout* layout)
{
    // Detector Level
    vector<Param*> params = detector->getParams();
    QVBoxLayout *paramLayout = new QVBoxLayout();
    string title = detector->getName() + " Algorithm Parameters:";
    QGroupBox *groupBox = new QGroupBox(title.c_str());
    groupBox->setLayout(paramLayout);
    Vector<QWidget*> gparams(params.size());
    Vector<QGridLayout*> guiItems(params.size()); // item wrapper
    for (unsigned int i = 0; i < params.size(); i++)
    {
        guiItems[i] = new QGridLayout(); // create gui item
        if (params[i]->getType() == Param::MODE)
        {
            gparams[i] = new GUICheckBox((ModeParam*)params[i]); // create widget
            guiItems[i]->addWidget(gparams[i], 0, 0); // add to gui item
            connect(gparams[i], SIGNAL(valueChanged(bool* const, bool)), this, SLOT(setParam(bool* const, bool)));
        }
        else if (params[i]->getType() == Param::RANGE)
        {
            gparams[i] = new GUISlider((RangeParam<int>*)params[i]); // create widget

            QSpinBox *spinbox = new QSpinBox(); // can we delete this?
            spinbox->setRange(((GUISlider*)gparams[i])->minimum(),((GUISlider*)gparams[i])->maximum());
            spinbox->setValue(((GUISlider*)gparams[i])->value());
            guiItems[i]->addWidget(new QLabel(params[i]->getName()), 0, 0);
            guiItems[i]->addWidget(gparams[i], 1, 0);
            guiItems[i]->addWidget(spinbox, 1, 1);
            connect(spinbox, SIGNAL(valueChanged(int)), gparams[i], SLOT(setValue(int)));
            connect(gparams[i], SIGNAL(valueChanged(int)), spinbox, SLOT(setValue(int)));
            connect(gparams[i], SIGNAL(valueChanged(int* const, int)), this, SLOT(setParam(int* const, int)));
        }
        else if (params[i]->getType() == Param::RANGE_DBL)
        {
            gparams[i] = new GUIDSpinBox((RangeParam<double>*)params[i]); // create widget
            guiItems[i]->addWidget(new QLabel(params[i]->getName()), 0, 0);
            guiItems[i]->addWidget(gparams[i], 0, 1);
            connect(gparams[i], SIGNAL(valueChanged(double* const, double)), this, SLOT(setParam(double* const, double)));
        }
        paramLayout->addLayout(guiItems[i]);
    }
    layout->addWidget(groupBox);
    groupBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    //QSpacerItem* vertSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    //ui->sideLayout->addSpacerItem(verticalSpacer);;
}
