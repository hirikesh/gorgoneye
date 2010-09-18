#ifndef GUIPARAM_H
#define GUIPARAM_H

#include <QCheckBox>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QRadioButton>
#include <QComboBox>
#include "parameter.h"
#include "trackers/basetracker.h"

class ModeParam;
class ImageModeParam;

namespace cv {
    class Mat;
}

class GUICheckBox : public QCheckBox
{
    Q_OBJECT
public:
    GUICheckBox(const std::string& title, bool* value);
    explicit GUICheckBox(ModeParam* mp);
signals:
    void valueChanged(bool* const, bool);
private slots:
    void emitWithPtr(int);
private:
   bool* const pValue;
};

class GUISlider : public QSlider
{
    Q_OBJECT
public:
    explicit GUISlider(RangeParam<int>* rp);
signals:
    void valueChanged(int* const, int);
private slots:
    void emitWithPtr(int);
private:
    int* const pValue;
};

class GUIDSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
public:
    explicit GUIDSpinBox(RangeParam<double>* rp);
signals:
    void valueChanged(double* const, double);
private slots:
    void emitWithPtr(double);
private:
    double* const pValue;
};

class GUIRadioButton : public QRadioButton
{
    Q_OBJECT
public:
    explicit GUIRadioButton(ImageModeParam* imp);
signals:
    void valueChanged(cv::Mat* const, bool);
private slots:
    void emitWithPtr(bool);
private:
    cv::Mat* const pValue;
};

class GUITrackerComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit GUITrackerComboBox(BaseTracker* trkr);
signals:
    void itemSelected(int);
private slots:
    void setDetector(int);
private:
    BaseTracker* tracker;
};

#endif // GUIPARAM_H
