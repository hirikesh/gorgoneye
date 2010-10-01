#include <cv.h>
#include <highgui.h>
#include <QLabel>
#include <QGroupBox>
#include <QComboBox>
#include <QTimer>
#include "geyetracker.h"
#include "ui_geyetracker.h"
#include "parameter.h"
#include "guiparam.h"
#include "trackers/basetracker.h"
#include "detectors/basedetector.h"
#include "glview.h"

#define USE_OPENGL 1

using cv::Mat;
using namespace std;

GEyeTracker::GEyeTracker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GEyeTracker),
    timer(new QTimer(this)),
    model(new Model(0)),
    opengl(new GLView()),
    imgModeGroup(new QButtonGroup())
{
    initGUI();
}

GEyeTracker::~GEyeTracker()
{
    delete model;
    delete ui;
}

void GEyeTracker::initGUI()
{
    ui->setupUi(this);
    vector<BaseTracker*> trackers = model->getTrackers();
    for (unsigned int i = 0; i < trackers.size(); i++) {
        createTrackerGUI(trackers[i]);
    }

#if(USE_OPENGL)
    ui->viewLayout->insertWidget(0, opengl);
#else
    ui->viewFrame->hide();
    ui->mainLayout->deleteLater();
#endif

    timer->setInterval(30); // timer expires every N ms
    connect(timer, SIGNAL(timeout()), this, SLOT(procFrame()));
    connect(ui->startBtn, SIGNAL(clicked()), timer, SLOT(start()));
    connect(ui->stopBtn, SIGNAL(clicked()), timer, SLOT(stop()));
}
// Event Handlers

void GEyeTracker::procFrame()
{
    model->update();

    cv::Rect f = model->getStore()->faceRoi;
    cv::Rect e = model->getStore()->eyesRoi + f.tl();

#if(USE_OPENGL)
    opengl->setFaceROI(f.x, f.y, f.width, f.height);
    opengl->setEyesROI(e.x, e.y, e.width, e.height);
    opengl->loadGLTextures(*model->getDispImg());
    opengl->updateGL();
#else
    cv::rectangle(model->getStore()->sceneImg, f, cv::Scalar(0,200,0), 2);
    cv::rectangle(model->getStore()->faceImg, e, cv::Scalar(200,0,0), 2);
    imshow("Display", *model->getDispImg());
#endif
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

void GEyeTracker::setImage(Mat* const img, bool value)
{
    if (value)
    {
        model->getStore()->dispImg = img;
    }
}

void GEyeTracker::createTrackerGUI(BaseTracker* tracker)
{
    // Tracker Level
    QVBoxLayout* trackerLayout = new QVBoxLayout();
    QGridLayout* trackerTitle = new QGridLayout();
    ui->paramsLayout->insertLayout(0, trackerLayout);
    ui->paramsLayout->setAlignment(trackerLayout, Qt::AlignTop);
    trackerLayout->addLayout(trackerTitle);

    QWidget* normImgMode = new GUIRadioButton(new ImageModeParam("Tracking Environment", tracker->getDispImg()));
    connect(normImgMode, SIGNAL(valueChanged(cv::Mat* const, bool)), this, SLOT(setImage(cv::Mat* const, bool)));
    static_cast<GUIRadioButton*>(normImgMode)->setChecked(true);
    trackerTitle->addWidget(normImgMode, 1, 0, 1, 2);
    QAbstractButton* normImgButton = qobject_cast<QAbstractButton*>(normImgMode);
    imgModeGroup->addButton(normImgButton); // add to global radio button group

    string title = "Enable " + tracker->getName() + " Tracking";
    GUICheckBox *trackerEnable = new GUICheckBox(title, tracker->getEnabled());
    trackerEnable->setChecked(tracker->isEnabled());
    connect(trackerEnable, SIGNAL(valueChanged(bool* const, bool)), this, SLOT(setParam(bool* const, bool)));

    trackerTitle->addWidget(trackerEnable, 0, 0);
    QComboBox* detectorSelection = new GUITrackerComboBox(tracker);
    trackerTitle->addWidget(detectorSelection, 0, 1);
    vector<BaseDetector*> detectors = tracker->getDetectors();
    for (unsigned int i = 0; i < detectors.size(); i++)
    {
        detectorSelection->addItem(detectors[i]->getName().c_str());
        if (detectors[i]->hasParams()) createDetectorGUI(detectors[i], trackerLayout);
    }
    detectorSelection->setCurrentIndex(tracker->getCurrDetectorType());
}

void GEyeTracker::createDetectorGUI(BaseDetector* detector, QVBoxLayout* layout)
{
    // Detector Level
    vector<Param*> params = detector->getParams();
    QVBoxLayout *paramLayout = new QVBoxLayout();
    string title = detector->getName() + " Algorithm Parameters:";
    QGroupBox *groupBox = new QGroupBox(title.c_str());
    groupBox->setLayout(paramLayout);
    vector<QWidget*> gparams(params.size());
    vector<QGridLayout*> guiItems(params.size()); // item wrapper
    for (unsigned int i = 0; i < params.size(); i++)
    {
        guiItems[i] = new QGridLayout(); // create gui item
        if (params[i]->getType() == Param::MODE)
        {
            gparams[i] = new GUICheckBox(static_cast<ModeParam*>(params[i])); // create widget
            guiItems[i]->addWidget(gparams[i], 0, 0); // add to gui item
            connect(gparams[i], SIGNAL(valueChanged(bool* const, bool)), this, SLOT(setParam(bool* const, bool)));
        }
        else if (params[i]->getType() == Param::RANGE)
        {
            gparams[i] = new GUISlider(static_cast<RangeParam<int>*>(params[i])); // create widget

            QSpinBox *spinbox = new QSpinBox(); // can we delete this?
            GUISlider* currSlider = static_cast<GUISlider*>(gparams[i]);
            spinbox->setRange(currSlider->minimum(), currSlider->maximum());
            spinbox->setValue(currSlider->value());
            guiItems[i]->addWidget(new QLabel(params[i]->getName()), 0, 0);
            guiItems[i]->addWidget(gparams[i], 1, 0);
            guiItems[i]->addWidget(spinbox, 1, 1);
            connect(spinbox, SIGNAL(valueChanged(int)), gparams[i], SLOT(setValue(int)));
            connect(gparams[i], SIGNAL(valueChanged(int)), spinbox, SLOT(setValue(int)));
            connect(gparams[i], SIGNAL(valueChanged(int* const, int)), this, SLOT(setParam(int* const, int)));
        }
        else if (params[i]->getType() == Param::RANGE_DBL)
        {
            gparams[i] = new GUIDSpinBox(static_cast<RangeParam<double>*>(params[i])); // create widget
            guiItems[i]->addWidget(new QLabel(params[i]->getName()), 0, 0);
            guiItems[i]->addWidget(gparams[i], 0, 1);
            connect(gparams[i], SIGNAL(valueChanged(double* const, double)), this, SLOT(setParam(double* const, double)));
        }
        else if (params[i]->getType() == Param::IMG_MODE)
        {
            gparams[i] = new GUIRadioButton(static_cast<ImageModeParam*>(params[i])); // create widget
            guiItems[i]->addWidget(gparams[i], 0, 0); // add to gui item
            connect(gparams[i], SIGNAL(valueChanged(cv::Mat* const, bool)), this, SLOT(setImage(cv::Mat* const, bool)));
            QAbstractButton* imgModeButton = qobject_cast<QAbstractButton*>(gparams[i]);
            imgModeGroup->addButton(imgModeButton); // add to global radio button group
        }
        paramLayout->addLayout(guiItems[i]);
    }
    // Here we statically create the webcam capture mode
//    QWidget* normImgMode = new GUIRadioButton(new ImageModeParam("Webcam Capture", &(model->getStore()->sceneImg)));
//    connect(normImgMode, SIGNAL(valueChanged(cv::Mat* const, bool)), this, SLOT(setImage(cv::Mat* const, bool)));
//    static_cast<GUIRadioButton*>(normImgMode)->setChecked(true);
//    paramLayout->addWidget(normImgMode);
    layout->addWidget(groupBox);
    groupBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    //QSpacerItem* vertSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    //ui->sideLayout->addSpacerItem(verticalSpacer);;
}
