# -------------------------------------------------
# Project created by QtCreator 2010-04-27T14:54:56
# -------------------------------------------------
QT += opengl \
    webkit
TARGET = gorgoneye
TEMPLATE = app
unix {
    INCLUDEPATH += "/usr/include/opencv"
    LIBS += -L"/usr/lib" \
        -lcv \
        -lhighgui \
        -lcxcore \
        -lml
}
win32 {
    INCLUDEPATH += "C:\OpenCV2.1\include\opencv"
    CONFIG(debug, debug|release) {
        LIBS += -L"C:\OpenCV2.1\lib\debug" \
            -lcv210d \
            -lhighgui210d \
            -lcxcore210d \
            -lml210d
    }
    CONFIG(release, debug|release) {
        LIBS += -L"C:\OpenCV2.1\lib\release" \
            -lcv210 \
            -lhighgui210 \
            -lcxcore210 \
            -lml210
    }
}

SOURCES += main.cpp \
    control.cpp \
    model.cpp \
    store.cpp \
    parameter.cpp \
    trackers/basetracker.cpp \
    trackers/facetracker.cpp \
    trackers/eyestracker.cpp \
    filters/basefilter.cpp \
    filters/grayscalefilter.cpp \
    filters/hsvfilter.cpp \
    filters/ycbcrfilter.cpp \
    detectors/basedetector.cpp \
    detectors/haardetector.cpp \
    detectors/featuredetector.cpp \
    detectors/hybriddetector.cpp \
    detectors/testdetector.cpp \
    ui/glview.cpp \
    ui/guiparam.cpp \
    ui/guiprocessdiag.cpp \
    ui/guiparamdiag.cpp \
    ui/guitrackerdiag.cpp \
    ui/guitreewidgetitem.cpp \
    filters/erodedilatefilter.cpp \
    trackers/facehaartracker.cpp \
    trackers/facecamshifttracker.cpp \
    detectors/camshiftdetector.cpp

HEADERS += \
    control.h\
    model.h \
    store.h \
    parameter.h \
    trackers/basetracker.h \
    trackers/facetracker.h \
    trackers/eyestracker.h \
    filters/basefilter.h \
    filters/grayscalefilter.h \
    filters/hsvfilter.h \
    filters/ycbcrfilter.h \
    detectors/basedetector.h \
    detectors/haardetector.h \
    detectors/featuredetector.h \
    detectors/hybriddetector.h \
    detectors/testdetector.h \
    ui/glview.h \
    ui/guiparam.h \
    ui/guiprocessdiag.h \
    ui/guiparamdiag.h \
    ui/guitrackerdiag.h \
    ui/guitreewidgetitem.h \
    filters/erodedilatefilter.h \
    trackers/facehaartracker.h \
    trackers/facecamshifttracker.h \
    detectors/camshiftdetector.h


FORMS += ui/control.ui
