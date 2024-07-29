#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QApplication>
#include <QWidget>

#include <QGroupBox>
#include <QLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QTextEdit>
#include <QList>
#include <QString>

#include <iomanip>
#include <cmath>

#include "PlotWidget.h"
#include "WavFourier.h"
// #include "ui_mainwindow.h"

/*
 * Customize QMainWindow to my liking regarding size, content, etc.
 *
*/
class MyWindow : public QWidget {
	Q_OBJECT

private:
	/* Layouts */
	QGridLayout *mainLayout;
	QGroupBox *fileChooserBox;
	QGroupBox *plotBox;

	/* Components of Layout */
	QString wav_filename;
	QPushButton *openFileDialogButton;
	QTextEdit *text;
	PlotWidget *plot;

	QLineEdit *mouseCoords; // for displaying mouse coords


	// Calculate Fourier stuff of WAV file
	WavFourier *wavfourier;

	// every QGroupBox gets it's own layout in these functions
	void createFileChooserBox();
	void createPlotBox();

	void showErrorDialogOpenFile();

	void setDefaultLayout();

	void setConnects();	// set all signal/slot connects

public:
	explicit MyWindow(QWidget *parent = 0);

public slots:
	void handleFileDialog();
	void plotFourierTransform();	// gets the data from WavFourier::getData(...)
	void onMouseMove(QMouseEvent *ev);	// triggered when mouse moves, then update mouse coords
};


#endif // MYWINDOW_H
