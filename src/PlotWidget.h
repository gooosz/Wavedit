#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QApplication>
#include <QWidget>
#include <QLayout>

#include <functional>

#include "qcustomplot.h"

/* A widget containing QCustomPlot */
class PlotWidget : public QWidget {
private:
	QHBoxLayout *layout;
	QCustomPlot *plot;
public:
	PlotWidget(QWidget *parent = 0);
	void makePlot(QVector<double> x, std::function<double(double)> f, int graphNr=0, bool rescale=true);
	void makePlot(QVector<double> x, QVector<double> y, int graphNr=0, bool rescale=true);

};

#endif // PLOTWIDGET_H
