#-------------------------------------------------
#
# Project created by QtCreator 2014-07-28T12:42:13
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = biology
TEMPLATE = app

SOURCES += main.cpp\
        mainWindow.cpp \
    defines/data.cpp \
    math/ols_polynom.cpp \
    math/ols_sin.cpp \
    math/processing.cpp \
    defines/koefs.cpp \
    math/mainElementMethod.cpp \
    plots/qcustomplot.cpp \
    plots/plot2d.cpp

HEADERS  += mainWindow.h \
    defines/data.h \
    math/ols_polynom.h \
    math/ols_sin.h \
    math/processing.h \
    defines/koefs.h \
    defines/defines.h \
    math/mainElementMethod.h \
    plots/qcustomplot.h \
    plots/plot2d.h

FORMS    += mainWindow.ui \
    plots/plot2d.ui
