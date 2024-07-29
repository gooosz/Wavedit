TEMPLATE = app

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

greaterThan(QT_MAJOR_VERSION, 4): CONFIG += c++17
greaterThan(QT_MAJOR_VERSION, 5): QMAKE_CXXFLAGS += -std=c++17

# FORMS += mainwindow.ui
# UI_DIR = build
MOC_DIR = build

CONFIG += link_pkgconfig
#PKGCONFIG += python3

VPATH += ./src
HEADERS += fft.h WavFourier.h qcustomplot.h PlotWidget.h MyWindow.h
SOURCES += fft.cpp WavFourier.cpp qcustomplot.cpp PlotWidget.cpp MyWindow.cpp main.cpp

#QMAKE_CXXFLAGS += D_GLIBCXX_DEBUG

OBJECTS_DIR = build
DESTDIR = build
