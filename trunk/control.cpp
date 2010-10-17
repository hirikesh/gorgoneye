#include <cv.h>
#include <QTimer>
#include <QKeyEvent>
#include "control.h"
#include "ui_control.h"
#include "ui/glgaze.h"
#include "ui/glview.h"
#include "ui/guiprocessdiag.h"
#include "ui/guitrackerdiag.h"

using cv::Mat;
using namespace std;

Control::Control(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GEyeTracker),
    timer(new QTimer(this)),
    imgModeGroup(new QButtonGroup()),
    model(new Model(0)),
    viewGL(new GLView(model->getStore())),
    gazeGL(new GLGaze(model->getStore()))
{
    // Setup gorgoneye tracker and UI
    initGUI();
}

Control::~Control()
{
    delete gazeGL;
    delete viewGL;
    delete model;
    delete ui;
}

void Control::initGUI()
{
    ui->setupUi(this);

    // Setup camera viewport
    ui->viewLayout->insertWidget(0, viewGL);

    // Setup tracker and filter UI
    GUIProcessDiag* filterList = new GUIProcessDiag("Filters:" , model, this);
    GUITrackerDiag* trackerList = new GUITrackerDiag("Trackers:", model, this);
    ui->auxLayout->insertWidget(0, filterList);
    ui->auxLayout->insertWidget(1, trackerList);

    // Setup basic camera and tracker control
    timer->setInterval(CAP_TIMER_MS); // timer expires every CAP_TIMER_MS ms
    connect(timer, SIGNAL(timeout()), this, SLOT(procViewFrame())); // default

    connect(ui->startBtn, SIGNAL(clicked()), timer, SLOT(start()));
    connect(ui->stopBtn, SIGNAL(clicked()), timer, SLOT(stop()));

    // Gaze control and training UI
//    gazeGL->getScene()->addWidget(ui->startBtn);
//    gazeGL->getScene()->addWidget(ui->stopBtn);
//    gazeGL->getScene()->addWidget(filterList);
//    gazeGL->getScene()->addWidget(trackerList);

    // Gaze control signals to have it share the same timer
    connect(ui->gazeBtn, SIGNAL(clicked()), this, SLOT(startGazeUi()));
    connect(gazeGL, SIGNAL(closed()), this, SLOT(endGazeUi()));
}

// Switch rendering of store's contents to gaze control UI
void Control::startGazeUi()
{
    gazeGL->show();
    timer->disconnect();
    ui->gazeBtn->setEnabled(false);
    connect(timer, SIGNAL(timeout()), this, SLOT(procGazeFrame()));
}

// Switch rendering of store's contents back to debugging UI
void Control::endGazeUi()
{
    timer->disconnect();
    ui->gazeBtn->setEnabled(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(procViewFrame()));
}

// Event handler for debugging UI
void Control::procViewFrame()
{
    // Update the model:
    //  Grabs new frame from webcam and does tracking
    //  Updates 'store' with new results
    model->update();

    // Updates opengl view:
    //  Grabs updated image and ROI data from 'store'
    //  Loads results to texture and OpenGL draws
    viewGL->updateGL();
}

// Event handler for gaze control UI
void Control::procGazeFrame()
{
    // Update the model:
    //  Grabs new frame from webcam and does tracking
    //  Updates 'store' with new results
    model->update();

    // Updates opengl view:
    //  Grabs updated image and ROI data from 'store'
    //  Loads results to texture and OpenGL draws
    gazeGL->scene()->update();
}

// Add key-press handlers to quit on ESC and open gaze
// UI on F4
void Control::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Escape)
        close();
    else if(event->key() == Qt::Key_F4)
        startGazeUi();
}
