#ifndef GUIPARAM_H
#define GUIPARAM_H

#include <QFrame>
#include <QCheckBox>
#include <QRadioButton>
#include "parameter.h"

class ModeParam;
class ImageModeParam;
class QFrame;
class QGridLayout;
class QLabel;
class QCheckBox;
class QDoubleSpinBox;
class QSlider;
class QSpinBox;
class QRadioButton;

namespace cv {
    class Mat;
}

class GUICheckBox : public QCheckBox
{
    Q_OBJECT
public:
    explicit GUICheckBox(const std::string& title, bool* value);
    explicit GUICheckBox(ModeParam* mp);
    void init();
private slots:
    void setParamValue(bool);
private:
    bool* const pValue;
};

class GUISlider : public QFrame
{
    Q_OBJECT
public:
    explicit GUISlider(RangeParam<int>* rp);
private slots:
    void setParamValue(int);
private:
    QSlider* slider;
    QSpinBox* spinbox;
    QLabel* title;
    QGridLayout* layout;
    int* const pValue;
};

class GUIDSpinBox : public QFrame
{
    Q_OBJECT
public:
    explicit GUIDSpinBox(RangeParam<double>* rp);
signals:
    void valueChanged(double* const, double);
private slots:
    void setParamValue(double);
private:
    QDoubleSpinBox* spinbox;
    QLabel* title;
    QGridLayout* layout;
    double* const pValue;
};

class GUIRadioButton : public QRadioButton
{
    Q_OBJECT
public:
    explicit GUIRadioButton(ImageModeParam* imp);
signals:
    void valueChanged(cv::Mat* const, bool);
    void enableChanged(bool* const, bool);
private slots:
    void setParamValues(bool);
private:
    cv::Mat* const pValue;
    bool* const enPValue;
    cv::Mat** dispImg;
};

#endif // GUIPARAM_H
