TEMPLATE = app

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

FORMS += mainwindow.ui
UI_DIR = build
MOC_DIR = build

CONFIG += link_pkgconfig
PKGCONFIG += python3

HEADERS += PlotWidget.h MyWindow.h
SOURCES += PlotWidget.cpp MyWindow.cpp main.cpp

OBJECTS_DIR = build
DESTDIR = build


