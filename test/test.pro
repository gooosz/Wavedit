QT += widgets testlib

greaterThan(QT_MAJOR_VERSION, 5): QMAKE_CXXFLAGS += -std=c++17

HEADERS += ../Wavedit/src/fft.h ../Wavedit/src/WavFourier.h
SOURCES += ../Wavedit/src/fft.cpp ../Wavedit/src/WavFourier.cpp ./src/TestWavFourier.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/test/TestWavedit
INSTALLS += target

OBJECTS_DIR = build
DESTDIR = build
