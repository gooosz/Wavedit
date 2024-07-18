TEMPLATE = app

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

greaterThan(QT_MAJOR_VERSION, 4): CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 5): QMAKE_CXXFLAGS += -std=c++11

# FORMS += mainwindow.ui
# UI_DIR = build
MOC_DIR = build

CONFIG += link_pkgconfig
PKGCONFIG += python3

VPATH += ./src
HEADERS += qcustomplot.h PlotWidget.h MyWindow.h
SOURCES += qcustomplot.cpp PlotWidget.cpp MyWindow.cpp main.cpp

OBJECTS_DIR = build
DESTDIR = build
