#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QApplication>
#include <QWidget>

#include <QGroupBox>
#include <QLayout>
#include <QPushButton>
#include <QTextEdit>

#include "PlotWidget.h"
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
	QPushButton *button1;
	QTextEdit *text;
	PlotWidget *plot;

	// every QGroupBox gets it's own layout in these functions
	void createFileChooserBox();
	void createPlotBox();

	void setDefaultLayout();

public:
	explicit MyWindow(QWidget *parent = 0);

signals:
public slots:
};


#endif // MYWINDOW_H
