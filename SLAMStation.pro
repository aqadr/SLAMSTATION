#-------------------------------------------------
#
# Project created by QtCreator 2015-12-25T11:23:04
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SLAMStation
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Socket.cpp \
    myglwidget.cpp

HEADERS  += mainwindow.h \
    Socket.h \
    myglwidget.h
LIBS += -lGLU

FORMS    += mainwindow.ui
INCLUDEPATH +=/usr/local/include/eigen3
