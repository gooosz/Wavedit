QT += widgets testlib

greaterThan(QT_MAJOR_VERSION, 5): QMAKE_CXXFLAGS += -std=c++17

VPATH += ./src
SOURCES += TestWavFourier.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/test/TestWavedit
INSTALLS += target

OBJECTS_DIR = build
DESTDIR = build
