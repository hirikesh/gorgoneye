# -------------------------------------------------
# Project created by QtCreator 2010-04-27T14:54:56
# -------------------------------------------------
QT += opengl \
    webkit
TARGET = gorgoneye
TEMPLATE = app
INCLUDEPATH += "C:\OpenCV2.0\include\opencv"
LIBS += -L"C:\OpenCV2.0\lib" \
    -lcv200 \
    -lcvaux200 \
    -lhighgui200 \
    -lcxcore200 \
    -lml200
SOURCES += main.cpp \
    geye_ui.cpp \
    geye.cpp \
    geye/ge_eye.cpp \
    geye/ge_gaze.cpp \
    geye/ge_head.cpp
HEADERS += geye_ui.h \
    geye.h \
    geye/ge_eye.h \
    geye/ge_gaze.h \
    geye/ge_head.h
FORMS += geye_ui.ui
