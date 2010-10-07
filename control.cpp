#include <cv.h>
#include <highgui.h>
#include <QGroupBox>
#include <QComboBox>
#include <QTimer>
#include <QGridLayout>
#include "control.h"
#include "ui_control.h"
#include "parameter.h"
#include "ui/guiparam.h"
#include "ui/glview.h"
#include "ui/guiprocessdiag.h"
#include "ui/guiparamdiag.h"
//#include "trackers/basetracker.h"
#include "detectors/basedetector.h"


#define USE_OPENGL 1

using cv::Mat;
using namespace std;

Control::Control(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GEyeTracker),
    timer(new QTimer(this)),
    model(new Model(0)),
    imgModeGroup(new QButtonGroup())
{
    vector<bool*> is_valid_roi;
    is_valid_roi.push_back(&model->getStore()->faceLocated);
    is_valid_roi.push_back(&model->getStore()->eyesLocated);
    opengl = new GLView(is_valid_roi);

    initGUI();
}

Control::~Control()
{
    delete model;
    delete ui;
}

void Control::initGUI()
{
    ui->setupUi(this);
    vector<BaseTracker*> trackers = model->getTrackers();
    GUIProcessDiag* filterList = new GUIProcessDiag(model->getFilters(), this);
    ui->auxLayout->insertWidget(0, filterList);
    for (unsigned int i = 0; i < trackers.size(); i++) {
        createTrackerGUI(trackers[i]);
    }

#if(USE_OPENGL)
    ui->viewLayout->insertWidget(0, opengl);
#else
    ui->viewFrame->hide();
    ui->mainLayout->deleteLater();
#endif

    timer->setInterval(35); // timer expires every N ms
    connect(timer, SIGNAL(timeout()), this, SLOT(procFrame()));
    connect(ui->startBtn, SIGNAL(clicked()), timer, SLOT(start()));
    connect(ui->stopBtn, SIGNAL(clicked()), timer, SLOT(stop()));
}
// Event Handlers

void Control::procFrame()
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

void Control::setParam(bool* const param, bool value)
{
   *param = value;
}

void Control::setImage(Mat* const img, bool value)
{
    if (value)
    {
        model->getStore()->dispImg = img;
    }
}

void Control::createTrackerGUI(BaseTracker* tracker)
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

    trackerTitle->addWidget(trackerEnable, 0, 0);
    QComboBox* detectorSelection = new GUITrackerComboBox(tracker);
    trackerTitle->addWidget(detectorSelection, 0, 1);
    vector<BaseDetector*> detectors = tracker->getDetectors();
    for (unsigned int i = 0; i < detectors.size(); i++)
    {
        detectorSelection->addItem(detectors[i]->getName().c_str());
        if (detectors[i]->hasParams()) createDetectorGUI(detectors[i], trackerLayout, trackerTitle);
    }
    detectorSelection->setCurrentIndex(tracker->getCurrDetectorType());
}

void Control::createDetectorGUI(BaseDetector* detector, QVBoxLayout* layout, QGridLayout* mode)
{
    // Detector Level
    vector<Param*> params = detector->getParams();
    QVBoxLayout *paramLayout = new QVBoxLayout();
    string title = detector->getName() + " Algorithm Parameters:";
    QGroupBox *groupBox = new QGroupBox(title.c_str());
    groupBox->setLayout(paramLayout);
    vector<QWidget*> gparams(params.size());

    GUIParamDiag* pDialog = new GUIParamDiag(title, params);
    paramLayout->addWidget(pDialog);

    for (unsigned int i = 0; i < params.size(); i++)
    {
        if (params[i]->getType() == Param::IMG_MODE)
        {
            gparams[i] = new GUIRadioButton(static_cast<ImageModeParam*>(params[i])); // create widget
            mode->addWidget(gparams[i], mode->rowCount(), 0, 1, 2); // add to title layout
            connect(gparams[i], SIGNAL(valueChanged(cv::Mat* const, bool)), this, SLOT(setImage(cv::Mat* const, bool)));
            connect(gparams[i], SIGNAL(enableChanged(bool* const, bool)), this, SLOT(setParam(bool* const, bool)));
            QAbstractButton* imgModeButton = qobject_cast<QAbstractButton*>(gparams[i]);
            imgModeGroup->addButton(imgModeButton); // add to global radio button group
        }
    }

    layout->addWidget(groupBox);
    groupBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
}
