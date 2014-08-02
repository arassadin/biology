#-------------------------------------------------
#
# Project created by QtCreator 2014-07-28T12:42:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = biology
TEMPLATE = app


SOURCES += main.cpp\
        mainWindow.cpp \
    data.cpp \
    math/ols_polynom.cpp \
    math/ols_sin.cpp \
    math/processing.cpp \
    koefs.cpp

HEADERS  += mainWindow.h \
    data.h \
    math/ols_polynom.h \
    math/ols_sin.h \
    math/processing.h \
    koefs.h \
    defines.h

FORMS    += mainWindow.ui
