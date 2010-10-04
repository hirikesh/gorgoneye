# -------------------------------------------------
# Project created by QtCreator 2010-04-27T14:54:56
# -------------------------------------------------
QT += opengl \
    webkit
TARGET = gorgoneye
TEMPLATE = app
unix { 
    INCLUDEPATH += "/usr/local/include/opencv"
    LIBS += -L"/usr/local/lib" \
        -lcv \
        -lhighgui \
        -lcxcore \
        -lml
}
win32 { 
    INCLUDEPATH += "C:\OpenCV2.1\include\opencv"
#    debug:LIBS += -L"C:\OpenCV2.1\lib\debug" \
#        -lcv210d \
#        -lhighgui210d \
#        -lcxcore210d \
#        -lml210d
    release:LIBS += -L"C:\OpenCV2.1\lib\release" \
        -lcv210 \
        -lhighgui210 \
        -lcxcore210 \
        -lml210
}
SOURCES += main.cpp \
    control.cpp \
    store.cpp \
    detectors/basedetector.cpp \
    detectors/haardetector.cpp \
    detectors/featuredetector.cpp \
    detectors/hybriddetector.cpp \
    detectors/testdetector.cpp \
    trackers/basetracker.cpp \
    trackers/facetracker.cpp \
    trackers/eyestracker.cpp \
    parameter.cpp \
    guiparam.cpp \
    model.cpp \
    glview.cpp \
    filters/basefilter.cpp \
    filters/grayscalefilter.cpp \
    filters/hsvfilter.cpp \
    filters/ycbcrfilter.cpp

HEADERS += \
    control.h\
    store.h \
    detectors/basedetector.h \
    detectors/haardetector.h \
    detectors/featuredetector.h \
    detectors/hybriddetector.h \
    detectors/testdetector.h \
    trackers/basetracker.h \
    trackers/facetracker.h \
    trackers/eyestracker.h \
    parameter.h \
    guiparam.h \
    model.h \
    glview.h \    
    filters/basefilter.h \
    filters/grayscalefilter.h \
    filters/hsvfilter.h \
    filters/ycbcrfilter.h
FORMS += ui/geyetracker.ui
