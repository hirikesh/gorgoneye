#ifndef CONTROL_H
#define CONTROL_H

#include <cv.h>
#include <QWidget>
#include "model.h"

#define CAP_TIMER_MS 33

class QGridLayout;
class QButtonGroup;
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

public slots:
    void procFrame();

private:
    void initGUI();
    Ui::GEyeTracker *ui;

    QTimer* timer;
    QButtonGroup* imgModeGroup;

    Model* model;
    GLView* opengl;
};

#endif // CONTROL_H
