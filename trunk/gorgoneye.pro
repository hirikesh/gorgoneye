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
    geyedetector.cpp \
    control.cpp \
    store.cpp \
    detector.cpp \
    deprecated/geyeview.cpp \
    deprecated/geye.cpp \
    detectors/haar.cpp \
    detectors/feature.cpp
HEADERS += geyetracker.h \
    geyedetector.h \
    control.h \
    store.h \
    detector.h \
    deprecated/geyeview.h \
    deprecated/geye.h \
    detectors/haar.h \
    detectors/feature.h
FORMS += geyetracker.ui \
    deprecated/geyeview.ui
