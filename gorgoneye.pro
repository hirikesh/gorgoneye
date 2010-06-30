# -------------------------------------------------
# Project created by QtCreator 2010-04-27T14:54:56
# -------------------------------------------------
QT += opengl \
    webkit
TARGET = gorgoneye
TEMPLATE = app
INCLUDEPATH += "C:\OpenCV2.1\include\opencv"

CONFIG(debug, debug|release) {
   LIBS += -L"C:\OpenCV2.1\lib\debug" \
        -lcv210d \
        -lcvaux210d \
        -lhighgui210d \
        -lcxcore210d \
        -lml210d
} else {
   LIBS += -L"C:\OpenCV2.1\lib\release" \
        -lcv210 \
        -lcvaux210 \
        -lhighgui210 \
        -lcxcore210 \
        -lml210
}
SOURCES += main.cpp \
    geye.cpp \
    geyeui.cpp \
    geyeview.cpp
HEADERS += geye.h \
    geyeui.h \
    geyeview.h
FORMS += geyeui.ui \
    geyeview.ui
