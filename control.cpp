#include <cv.h>
#include <highgui.h>
#include <QGroupBox>
#include <QComboBox>
#include <QTimer>
#include <QGridLayout>
#include <QDesktopWidget>
#include <QRect>
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

using cv::Mat;
using namespace std;

Control::Control(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GEyeTracker),
    timer(new QTimer(this)),
    imgModeGroup(new QButtonGroup()),
    model(new Model(0)),
    opengl(new GLView(model->getStore()))
{
    // START TEST CODE
    // Determine screen res for full screen training/gaze estimation
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRes = desktopWidget->screenGeometry();
    qDebug() << screenRes.width() << screenRes.height();
    // END TEST CODE

    // Setup gorgoneye tracker and UI
    initGUI();
}

Control::~Control()
{
    delete opengl;
    delete model;
    delete ui;
}

void Control::initGUI()
{
    ui->setupUi(this);

    // Setup camera viewport
    ui->viewLayout->insertWidget(0, opengl);

    // Setup tracker and filter UI
    GUIProcessDiag* filterList = new GUIProcessDiag("Filters:" , model, this);
    GUITrackerDiag* trackerList = new GUITrackerDiag("Trackers:", model, this);
    ui->auxLayout->insertWidget(0, filterList);
    ui->auxLayout->insertWidget(1, trackerList);

    // Setup basic camera and tracker control
    timer->setInterval(CAP_TIMER_MS); // timer expires every N ms
    connect(timer, SIGNAL(timeout()), this, SLOT(procFrame()));
    connect(ui->startBtn, SIGNAL(clicked()), timer, SLOT(start()));
    connect(ui->stopBtn, SIGNAL(clicked()), timer, SLOT(stop()));
}


// Main Event Handler
void Control::procFrame()
{
    // Update the model:
    //  Grabs new frame from webcam and does tracking
    //  Updates 'store' with new results
    model->update();

    // Updates opengl view:
    //  Grabs updated image and ROI data from 'store'
    //  Draws results to OpenGL viewport
    opengl->loadGLTextures();
    opengl->updateGL();
}
