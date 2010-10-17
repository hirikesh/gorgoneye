#ifndef CONTROL_H
#define CONTROL_H

#include <cv.h>
#include <QWidget>
#include "model.h"
#include "config.h"

class QButtonGroup;
class QKeyEvent;
class BaseDetector;
class GLView;
class GLGaze;

namespace Ui {
    class GEyeTracker;
}

class Control : public QWidget
{
    Q_OBJECT

public:
    explicit Control(QWidget *parent = 0);
    ~Control();

protected slots:
    void startGazeUi();
    void endGazeUi();
    void procViewFrame();
    void procGazeFrame();

private:
    void initGUI();
    void keyPressEvent(QKeyEvent* event);

    Ui::GEyeTracker *ui;

    QTimer* timer;
    QButtonGroup* imgModeGroup;

    Model* model;
    GLView* viewGL;
    GLGaze* gazeGL;
};

#endif // CONTROL_H
