#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QApplication>
#include <QWidget>
#include <QLayout>

#include "qcustomplot.h"

/* A widget containing QCustomPlot */
class PlotWidget : public QWidget {
private:
	QHBoxLayout *layout;
	QCustomPlot *plot;
public:
	PlotWidget(QWidget *parent = 0);
	void makePlot();

};

#endif // PLOTWIDGET_H
