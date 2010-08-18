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
GEyeTracker::GEyeTracker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GEyeTracker)
{
    #define HAAR_CC_FACE_DEFAULT "c:\\opencv2.1\\data\\haarcascades\\haarcascade_frontalface_default.xml"

    ui->setupUi(this);


    model = new Model(0);

    vector<BaseDetector*> detectors = model->getTrackerParams();
    for (unsigned i = 0; i < detectors.size(); i++)
    {
        createDynamicGUI(detectors[i]);
    }


//    capture = VideoCapture(0);
    /***** TEST CODE START *****/
    Store* store = model->getStore();
    image = store->sceneImg;
    /***** TEST CODE END *****/
//    capture >> image;


//    ged = GEyeDetector(&image);
//    ged.setCC(new CascadeClassifier(HAAR_CC_FACE_DEFAULT));

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
    model->update();
    Rect r = model->getStore()->faceRoi;
    faceLoc = QRect(QPoint(r.x,r.y), QSize(r.width,r.height));
    /**** TEST CODE ****/
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

void GEyeTracker::disableParams()
{
}

void GEyeTracker::enableParams()
{
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

void GEyeTracker::createDynamicGUI(BaseDetector* detector)
{
    // Dynamic GUI Generator
    // Model Level
    // -- Model Level GUI Code Here --
    // Tracker Level
    // -- Tracker Level GUI Code Here --
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
            // generate the other parts of the gui item
            QSpinBox *spinbox = new QSpinBox(); // every time this loops we dont have a ref back to spinbox; can we still delete?
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
    ui->sideLayout->addWidget(groupBox);
    groupBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    ui->sideLayout->setAlignment(groupBox, Qt::AlignTop);
    //QSpacerItem* vertSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    //ui->sideLayout->addSpacerItem(verticalSpacer);
}
