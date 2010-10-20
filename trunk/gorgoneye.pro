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
            -lml210d \
            -lcvblobd
    }
    CONFIG(release, debug|release) {
        LIBS += -L"C:\OpenCV2.1\lib\release" \
            -lcv210 \
            -lhighgui210 \
            -lcxcore210 \
            -lml210 \
            -lcvblob
    }
}

SOURCES += main.cpp \
    control.cpp \
    model.cpp \
    store.cpp \
    parameter.cpp \
    filters/basefilter.cpp \
    filters/grayscalefilter.cpp \
    filters/hsvfilter.cpp \
    filters/ycbcrfilter.cpp \
    filters/erodedilatefilter.cpp \
    filters/equalisefilter.cpp \
    filters/cannycontourfilter.cpp \
    filters/cannyedgefilter.cpp \
    filters/sobelfilter.cpp \
    filters/harriscornerfilter.cpp \
    detectors/basedetector.cpp \
    detectors/haardetector.cpp \
    detectors/camshiftdetector.cpp \
    detectors/meanshiftdetector.cpp \
    detectors/mlearningdetector.cpp \
    trackers/basetracker.cpp \
    trackers/facetracker.cpp \
    trackers/eyestracker.cpp \
    trackers/gazetracker.cpp \
    trackers/facehaartracker.cpp \
    trackers/facecamshifttracker.cpp \
    trackers/facehaarcamshifttracker.cpp \
    trackers/facehaarcamshiftactracker.cpp \
    trackers/eyeshaartracker.cpp \
    ui/glgaze.cpp \
    ui/glgazescene.cpp \
    ui/glview.cpp \
    ui/guiparam.cpp \
    ui/guiprocessdiag.cpp \
    ui/guiparamdiag.cpp \
    ui/guitrackerdiag.cpp \
    ui/guitreewidgetitem.cpp

HEADERS += \
    config.h \
    control.h\
    model.h \
    store.h \
    parameter.h \
    filters/basefilter.h \
    filters/grayscalefilter.h \
    filters/hsvfilter.h \
    filters/ycbcrfilter.h \
    filters/erodedilatefilter.h \
    filters/equalisefilter.h \
    filters/cannycontourfilter.h \
    filters/cannyedgefilter.h \
    filters/sobelfilter.h \
    filters/harriscornerfilter.h \
    detectors/basedetector.h \
    detectors/haardetector.h \
    detectors/camshiftdetector.h \
    detectors/meanshiftdetector.h \
    detectors/mlearningdetector.h \
    trackers/basetracker.h \
    trackers/facetracker.h \
    trackers/eyestracker.h \
    trackers/gazetracker.h \
    trackers/facehaartracker.h \
    trackers/facecamshifttracker.h \
    trackers/facehaarcamshifttracker.h \
    trackers/facehaarcamshiftactracker.h \
    trackers/eyeshaartracker.h \
    ui/glgaze.h \
    ui/glgazescene.h \
    ui/glview.h \
    ui/guiparam.h \
    ui/guiprocessdiag.h \
    ui/guiparamdiag.h \
    ui/guitrackerdiag.h \
    ui/guitreewidgetitem.h

FORMS += ui/control.ui
