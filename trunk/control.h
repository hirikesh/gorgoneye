#ifndef CONTROL_H
#define CONTROL_H

#include <cv.h>
#include <QWidget>
#include <QButtonGroup>
#include "model.h"

class QGridLayout;
class BaseDetector;
class GLView;

namespace Ui {
    class GEyeTracker;
}

class Control : public QWidget
{
    Q_OBJECT

public:
    explicit Control(QWidget *parent = 0);
    ~Control();
    void createTrackerGUI(BaseTracker*);
    void createDetectorGUI(BaseDetector*, QGridLayout*);

public slots:
    void procFrame();   
    void setParam(bool* const, bool);
    void setImage(cv::Mat* const, bool);
private:
    void initGUI();
    Ui::GEyeTracker *ui;
    QTimer* timer;
    Model* model;
    GLView* opengl;
    QButtonGroup* imgModeGroup;
};

#endif // CONTROL_H
