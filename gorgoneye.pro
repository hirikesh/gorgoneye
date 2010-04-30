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
    geye.cpp
HEADERS += geye.h
FORMS += 
