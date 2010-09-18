# -------------------------------------------------
# Project created by QtCreator 2010-04-27T14:54:56
# -------------------------------------------------
QT += opengl \
    webkit
TARGET = gorgoneye
TEMPLATE = app
INCLUDEPATH += "C:\OpenCV2.1\include\opencv"
debug:LIBS += -L"C:\OpenCV2.1\lib\debug" \
    -lcv210d \
    -lcvaux210d \
    -lhighgui210d \
    -lcxcore210d \
    -lml210d
release:LIBS += -L"C:\OpenCV2.1\lib\release" \
    -lcv210 \
    -lcvaux210 \
    -lhighgui210 \
    -lcxcore210 \
    -lml210
SOURCES += main.cpp \
    geyetracker.cpp \
    #geyedetector.cpp \
    control.cpp \
    store.cpp \
    detectors/basedetector.cpp \
    detectors/haardetector.cpp \
    detectors/featuredetector.cpp \
    trackers/facetracker.cpp \
    trackers/basetracker.cpp \
    parameter.cpp \
    guiparam.cpp \
    model.cpp \
    glview.cpp \
    detectors/hybriddetector.cpp
HEADERS += geyetracker.h \
    #geyedetector.h \
    control.h \
    store.h \
    detectors/basedetector.h \
    detectors/haardetector.h \
    detectors/featuredetector.h \
    trackers/facetracker.h \
    trackers/basetracker.h \
    parameter.h \
    guiparam.h \
    model.h \
    glview.h \
    detectors/hybriddetector.h
FORMS += geyetracker.ui
