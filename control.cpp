#include <cv.h>
#include <highgui.h>
#include <QGroupBox>
#include <QComboBox>
#include <QTimer>
#include <QGridLayout>
#include <QDebug>
#include "control.h"
#include "ui_control.h"
#include "parameter.h"
#include "ui/guiparam.h"
#include "ui/glview.h"
#include "ui/guiprocessdiag.h"
#include "ui/guitrackerdiag.h"
#include "ui/guiparamdiag.h"
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
//    vector<bool*> is_valid_roi;
//    is_valid_roi.push_back(&model->getStore()->faceLocated);
//    is_valid_roi.push_back(&model->getStore()->eyesLocated);
//    opengl = new GLView(is_valid_roi);
    opengl = new GLView(model->getStore());

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
    GUIProcessDiag* filterList = new GUIProcessDiag("Filters:" , model, this);
    GUITrackerDiag* trackerList = new GUITrackerDiag("Trackers:", model, this);
    ui->auxLayout->insertWidget(0, filterList);
    ui->auxLayout->insertWidget(1, trackerList);

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

//    cv::Rect f = model->getStore()->faceRoi;
//    cv::Rect e = model->getStore()->eyesRoi + f.tl();

#if(USE_OPENGL)
//    opengl->setFaceROI(f.x, f.y, f.width, f.height);
//    opengl->setEyesROI(e.x, e.y, e.width, e.height);
//    opengl->loadGLTextures(*model->getDispImg());
    opengl->loadGLTextures();
    opengl->updateGL();
#else
    cv::rectangle(model->getStore()->sceneImg, f, cv::Scalar(0,200,0), 2);
    cv::rectangle(model->getStore()->faceImg, e, cv::Scalar(200,0,0), 2);
    imshow("Display", *model->getDispImg());
#endif
}
